// port: seamless raw read-ahead only; original initialization remains on the main thread.
#include "Seamless.h"
#include "../Globals.h"
#include "../gen/globals.h" // port: seamless Jimmy singleton
#include "../CEditor.h"
#include "../C3DPlayer.h"
#include "../CGameType.h"
#include "../SaveGame.h" // port: seamless
#include "../levels/CLoadLevel.h"
#include <stdio.h>
#include <strings.h>
#include <string>
#include <vector>
#include <set>
#include <dirent.h>
#include <ctype.h>
namespace {
struct Warm {
 SDL_Thread *thread; SDL_atomic_t cancel, done; std::string target;
 std::vector<std::vector<char> > buffers;
 Warm():thread(0) { SDL_AtomicSet(&cancel,0); SDL_AtomicSet(&done,0); }
 ~Warm() { SDL_AtomicSet(&cancel,1); if(thread) SDL_WaitThread(thread,0); }
 void stop() { SDL_AtomicSet(&cancel,1); if(thread && !SDL_AtomicGet(&done)) return; if(thread) SDL_WaitThread(thread,0); thread=0; buffers.clear(); target.clear(); }
};
Warm warm;
std::string resolve(const char *dir,const std::string &name) {
 DIR *d=opendir(dir); if(!d) return std::string(); struct dirent *e; std::string p;
 while((e=readdir(d))) if(!strcasecmp(e->d_name,name.c_str())) { p=std::string(dir)+"/"+e->d_name; break; }
 closedir(d); return p;
}
void read(const std::string &path) {
 FILE *f=fopen(path.c_str(),"rb"); if(!f) return;
 warm.buffers.push_back(std::vector<char>()); std::vector<char> &b=warm.buffers.back(); char chunk[65536]; size_t n;
 while(!SDL_AtomicGet(&warm.cancel) && (n=fread(chunk,1,sizeof chunk,f))) b.insert(b.end(),chunk,chunk+n);
 fclose(f);
}
int worker(void *) {
 read(resolve("gam",warm.target)); std::set<std::string> names;
 if(!warm.buffers.empty()) {
 const std::vector<char> &b=warm.buffers[0]; std::string word;
 for(size_t i=0;i<b.size();++i) { unsigned char c=b[i]; if(isalnum(c)||c=='_'||c=='.') word+=(char)tolower(c); else { if(word.size()>4 && word.substr(word.size()-4)==".omt") names.insert(word); word.clear(); } }
 }
 const char *common[]={"alpha.omt","soundeffects.omt","objects.omt","sprites.omt","phone.omt","scooter.omt","bluesky3.omt","objectslevel5a.omt","doors.omt","jeep.omt","dusksky.omt","nightsky.omt"};
 for(unsigned i=0;i<sizeof common/sizeof *common;++i) names.insert(common[i]);
 names.insert(!strcasecmp(warm.target.c_str(),"level2.gam")?"level2.omt":"level1.omt");
 for(std::set<std::string>::iterator i=names.begin();i!=names.end()&&!SDL_AtomicGet(&warm.cancel);++i) { std::string path=resolve("omt",*i); if(path.empty()) path=resolve(".",*i); read(path); }
 SDL_AtomicSet(&warm.done,1);
 return 0;
}
bool match(const char *target,const char *start) {
 if(!PortSeamless::enabled() || !neg::g_editor || strcasecmp(start,"school1")) return false;
 return (!strcasecmp(neg::g_editor->m_game_name,"level1.gam")&&!strcasecmp(target,"level2.gam")) || (!strcasecmp(neg::g_editor->m_game_name,"level2.gam")&&!strcasecmp(target,"level1.gam"));
}
}
bool neg::SeamlessExit(const char *target,const char *start) {
 if(!match(target,start)) return false;
 PortSeamless::state().pending=true;
 SDL_Log("seamless: exit %.3f",PortSeamless::now()); return true;
}
void neg::SeamlessApproach() {
 if(!PortSeamless::enabled() || !g_jimmy || PortSeamless::state().pending) return;
 // Cancellation never waits for worker I/O on the main thread.
 if(warm.thread && SDL_AtomicGet(&warm.cancel) && SDL_AtomicGet(&warm.done)) warm.stop();
 float p[4]; g_jimmy->v195(p); CLoadLevel *near=0;
 for(std::list<CGameObject *>::iterator i=g_objects.begin();i!=g_objects.end();++i) {
 CLoadLevel *e=dynamic_cast<CLoadLevel *>(*i); if(!e || !match(e->m_level_name,e->m_start_point)) continue;
 if(strcasecmp(e->m_required_task,"none")) { int s=GetTaskState(e->m_required_task); if(s!=-1 && (s<e->m_required_level || (e->m_exact_level!=-1 && e->m_exact_level!=s))) continue; }
 float q[4]; e->v195(q); float dx=p[0]-q[0],dy=p[1]-q[1],dz=p[2]-q[2]; float d=dx*dx+dy*dy+dz*dz;
 if(d< (warm.thread?16000000.0f:9000000.0f)) { near=e; break; }
 }
 PortSeamless::state().armed = near != 0; // port: seamless, the frame copy runs only near an open exit
 if(!near) { if(warm.thread) warm.stop(); return; }
 if(!warm.thread) { warm.target=near->m_level_name; SDL_AtomicSet(&warm.cancel,0); SDL_AtomicSet(&warm.done,0); warm.thread=SDL_CreateThread(worker,"seamless-read",0); SDL_Log("seamless: preload %s",near->m_level_name); }
}
