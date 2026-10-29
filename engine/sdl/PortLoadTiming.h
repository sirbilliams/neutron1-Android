// port: load timing; observational only, main-thread counters, always enabled.
#ifndef PORT_LOAD_TIMING_H
#define PORT_LOAD_TIMING_H
#include <SDL.h>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <sys/stat.h>
#include <ctype.h>
namespace PortLoadTiming {
inline double now() { return double(SDL_GetPerformanceCounter()) * 1000.0 / double(SDL_GetPerformanceFrequency()); }
struct DB { std::string name; double ms; double bytes; DB():ms(0),bytes(0) {} };
struct State {
 bool active, ready; double start, loaded, child, gl; std::string target;
 std::map<const void *, DB> live; std::map<std::string, DB> totals; std::set<std::string> previous, external, reopened; std::map<std::string,DB> assets; std::map<std::string,double> phases; int gl_depth;
 State():active(false),ready(false),start(0),loaded(0),child(0),gl(0),gl_depth(0) {}
};
inline State &state() { static State s; return s; }
inline void begin(const char *name) {
 State &s=state(); if(s.active) return;
 s.previous=s.external; s.external.clear(); for(std::map<const void *,DB>::iterator i=s.live.begin();i!=s.live.end();++i) s.previous.insert(i->second.name);
 s.totals.clear(); s.reopened.clear(); s.assets.clear(); s.phases.clear(); s.gl=0; s.start=now(); s.target=name; s.active=true; s.ready=false;
 SDL_Log("load: begin 0.000 target=%s",name);
}
struct Phase { const char *name; double start; bool enabled;
 Phase(const char *n):name(n),start(now()),enabled(state().active) {}
 ~Phase(){if(enabled) state().phases[name]+=now()-start;}
};
inline void opened(const void *key,const char *name,double ms) {
 DB d; d.name=name; std::replace(d.name.begin(),d.name.end(),'\\','/');
 struct stat st; if(stat(d.name.c_str(),&st)==0)d.bytes=double(st.st_size);
 std::transform(d.name.begin(),d.name.end(),d.name.begin(),::tolower);
 state().live[key]=d;
 if(state().active){state().reopened.insert(d.name);DB &t=state().totals[d.name]; t.name=d.name;t.bytes=d.bytes;t.ms+=ms;}
}
inline void closed(const void *key){state().live.erase(key);}
struct Read { const void *key; double start, child; bool enabled;
 Read(const void *k):key(k),start(now()),child(state().child),enabled(state().active){}
 ~Read(){if(!enabled)return; State &s=state();double elapsed=now()-start;double own=elapsed-(s.child-child);s.child+=own;
 std::map<const void *,DB>::iterator i=s.live.find(key);if(i!=s.live.end()){DB &t=s.totals[i->second.name];t.name=i->second.name;t.bytes=i->second.bytes;t.ms+=own;}}
};
struct Asset {std::string name;double start,bytes;bool enabled;
 Asset(const char *n,double b):name(n),start(now()),bytes(b),enabled(state().active){std::replace(name.begin(),name.end(),'\\','/');std::transform(name.begin(),name.end(),name.begin(),::tolower);state().external.insert(name);}
 ~Asset(){if(enabled){DB &d=state().assets[name];d.name=name;d.bytes=bytes;d.ms+=now()-start;}}
};
struct GL {double start;bool enabled;GL():start(now()),enabled(state().active){if(enabled)++state().gl_depth;}~GL(){if(enabled && --state().gl_depth==0)state().gl+=now()-start;}};
inline void ready(){if(state().active){state().ready=true;state().loaded=now();}}
inline void presented(){State &s=state();if(!s.active||!s.ready)return;
 SDL_Log("load: first-frame %.3f",now()-s.loaded); SDL_Log("load: texture-upload-cpu %.3f",s.gl);
 SDL_Log("load: total %.3f target=%s",now()-s.start,s.target.c_str());
 for(std::map<std::string,DB>::iterator i=s.totals.begin();i!=s.totals.end();++i)
 SDL_Log("load: database %.3f file=%s bytes=%.0f previous=%d reopened=%d",i->second.ms,i->first.c_str(),i->second.bytes,s.previous.count(i->first)?1:0,s.reopened.count(i->first)?1:0);
 for(std::map<std::string,DB>::iterator i=s.assets.begin();i!=s.assets.end();++i)
 SDL_Log("load: asset %.3f file=%s bytes=%.0f previous=%d",i->second.ms,i->first.c_str(),i->second.bytes,s.previous.count(i->first)?1:0);
 for(std::map<std::string,double>::iterator i=s.phases.begin();i!=s.phases.end();++i)SDL_Log("load: %s %.3f",i->first.c_str(),i->second);
 s.active=false;s.ready=false;
}
}
#endif
