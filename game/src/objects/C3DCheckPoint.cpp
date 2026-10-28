/*  C3DCheckPoint - see C3DCheckPoint.h. Reconstructed from Neutron.exe 0x414190..0x414c20. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "../port/Widescreen.h"
#include "C3DCheckPoint.h"
#include "../friends/C3DNick.h"
#include "../C2DInGameMenu.h"
#include "../Bridge.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../InterfaceElement.h"
#include "../JimmySlots.h"
#include "../SaveGame.h"
#include "../Sound.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include <string.h>
#include <strings.h>

using namespace neg;

// 0x474070: the first object of the global list whose ObjectTag matches (_stricmp).
static CGameObject *FindObjectByTag(const char *tag)
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (*i && strcasecmp((*i)->m_tag, tag) == 0) return *i;
	return NULL;
}

// --------------------------------------------------------------------------- construction

C3DCheckPoint::C3DCheckPoint()								// 0x414190
{
	SetName((char *)"C3DCHECKPOINT");
	Log("C3DCheckPoint()");
	C3DCheckPoint::InitObject();							// 0x414aa0 (called directly)
	SetObjectID('3CHK');									// element slot 48, 0x462d80
	strcpy(m_sprite_db, "sprites.omt");
	m_sprite_index = 42;									// 0x2a
	m_sprite_size = 100;									// 0x64
	v42(0);
	v40(0);
	v52(1);
	v107(1);
	v34(1);
	v46(0);
	strcpy(m_next, "none");
	f_008 = 1;												// element +0xd0
	m_check_avail = 0;
	v67(g_gametype->m_object_id == 'LV2B' ? 400.0f : 350.0f);	// +0x490 of the game type, 0x471240: radius
	C3DSprite::InitPhysics();								// 0x464210
	m_count_a = 0;
	m_count_b = 0;
	// (then a call to the empty 0x472970)
}

C3DCheckPoint::~C3DCheckPoint() {}							// 0x414380

namespace neg {
CGameObject *New_C3DCheckPoint(void) { return new C3DCheckPoint; }	// '3CHK' (0x660 bytes)
}

// --------------------------------------------------------------------------- helpers

void C3DCheckPoint::Enable(void)							// 0x414ae0
{
	m_check_avail = 1;
	force_show();											// (+0x70 != 0: = 1, hide(false))
}

void C3DCheckPoint::Disable(void)							// 0x414b00
{
	m_check_avail = 0;
	hide(true);
}

void C3DCheckPoint::DisableAll(void)						// 0x414b70
{
	for (std::list<CGameObject *>::iterator i = g_objects.begin(); i != g_objects.end(); ++i)
		if (*i && (*i)->m_object_id == '3CHK')				// +0x404
			static_cast<C3DCheckPoint *>(*i)->Disable();
}

// Nick sent off along the NICKWALK patrol again (0x414b00-area inline, twice).
static void NickWalksHome(C3DNick *nick)
{
	nick->m_state = 2;
	nick->ev72(nick->m_anim_run, (char *)"WALK");			// element +0x120
	strcpy(nick->m_patrol_point, "NICKWALK0");
	nick->m_dest = NULL;
	nick->m_speed = 170.0f;
}

// --------------------------------------------------------------------------- CGameObject family

void C3DCheckPoint::InitObject(void)						// 0x414aa0 (no base call, no log)
{
	RegisterProperty("CheckAvail", &m_check_avail, PROP_LONG, 0);
	RegisterProperty("Next", m_next, PROP_ALPHA, 0);
}

void C3DCheckPoint::v260(void)								// 0x414b20
{
	C3DSprite::v260();										// 0x441ac0 (C3DSprite's)
	if (m_check_avail == 0) Disable();
	else Enable();
	m_count_b = 0;
	m_count_a = 0;
	DisableAll();
}

void C3DCheckPoint::Process(float dt)						// 0x414410
{
	if (!GameIsRunning()) return;							// 0x475ca0
	C3DSprite::Process(dt);									// 0x463090
	if (strcasecmp(m_tag, "FINISHLINE") != 0) return;
	if (g_4eefc8 > 0.0f)									// the stopwatch runs
	{
		ShowLoadingPicture(true);							// 0x4073b0
		DrawText(neg::WideX(0x212, neg::WIDE_RIGHT), 0x39, 'MED', "%d", m_count_b / 2);	// 0x468660
		DrawText(neg::WideX(0x212, neg::WIDE_RIGHT), 0x58, 'MED', "%d", m_count_a / 2);
		return;
	}
	ShowLoadingPicture(false);
}

void C3DCheckPoint::AddTouching(CGameObject *other)			// 0x4144c0
{
	// (debug log "Enter cp Touch")
	CGameObject::AddTouching(other);						// 0x470a90
	if (other->NameContains((char *)"C3DNICK"))
	{
		C3DNick *nick = dynamic_cast<C3DNick *>(other);
		if (nick && nick->m_state == 3)						// patrolling
		{
			if (strcasecmp(m_tag, "FINISHLINE") == 0)
			{
				if (GetTaskState("SCENE") > 161) return;
				// (debug log "N1")
				if ((m_count_b & 0x80000001) == 1)			// odd
				{
					m_count_b++;
					if (GetTaskState("SCENE") < 130) PlayGameSoundIndex(-1, 0x66, false);	// 0x458980
					// (debug log "N2  %d")
					if (m_count_b > 5)
					{
						// (debug log "N 3")
						NickWalksHome(nick);
						// (debug log "N4")
						if (m_count_a < 6)					// the scooter has not finished its laps
						{
							CGameObject *put = FindObjectByTag("PUTNICKHERE");
							if (put)
							{
								CGameObject *jim = FindObjectByTag("JIM1");
								if (jim)
								{
									JimmyElementSlot94(jim);	// +0x178
									float p[4];
									put->v195(p);			// +0x310
									jim->v196(p[0], p[1], p[2], p[3]);	// +0x314
									m_count_a = 0;
									m_count_b = 0;
									DisableAll();
								}
							}
							const int scene = GetTaskState("SCENE");
							if (scene == 120) SetTaskState("SCENE", 125);
							else if (scene == 145) SetTaskState("SCENE", 150);
						}
					}
				}
			}
			else if (strcasecmp(m_tag, "STARTLINE") == 0)
			{
				C3DCheckPoint *fin = dynamic_cast<C3DCheckPoint *>(FindObjectByTag("FINISHLINE"));
				if (fin) fin->m_count_b++;
			}
		}
	}
	if (!other->NameContains((char *)"C3DJEEP")) return;
	if (m_check_avail == 0) return;
	if (strcasecmp(m_tag, "STARTLINE") == 0)
	{
		if (g_4eefc8 > 0.0f) g_4eefcc = g_4eefc8;			// the last lap's time
		C3DCheckPoint *fin = dynamic_cast<C3DCheckPoint *>(FindObjectByTag("FINISHLINE"));
		if (fin) fin->m_count_a++;
		g_4eefc8 = 0;										// 0x42ae80: the stopwatch starts again
	}
	if (strcasecmp(m_tag, "FINISHLINE") == 0)
	{
		if ((m_count_a & 0x80000001) == 1)
		{
			PlayGameSoundIndex(-1, 0xa3, false);
			m_count_a++;
			if (m_count_a > 5)
			{
				Disable();
				CGameObject *jim = FindObjectByTag("JIM1");
				if (jim) JimmyElementSlot94(jim);			// +0x178
				const int nick_passes = m_count_b;
				m_count_a = 0;
				m_count_b = 0;
				if (nick_passes > 5)						// Nick was first
				{
					DisableAll();
					return;
				}
				DisableAll();
				// (debug log "JW0")
				CGameObject *n = FindObjectByTag("Nick1");
				if (!n) n = FindObjectByTag("Nick2");
				if (n)
				{
					// (debug log "JW2")
					CGameObject *put = FindObjectByTag("PUTNICKHERE");
					if (put)
					{
						float p[4];
						put->v195(p);
						n->v196(p[0], p[1], p[2], p[3]);
						if (C3DNick *nick = dynamic_cast<C3DNick *>(n)) NickWalksHome(nick);
					}
				}
				const int scene = GetTaskState("SCENE");
				if (scene != 120 && scene != 125)
				{
					if (scene != 145) return;
					SetTaskState("SCENE", 160);
					return;
				}
				SetTaskState("SCENE", 130);
				return;
			}
		}
	}
	Disable();
	AddScore(10);											// 0x42adc0
	PlayGameSoundIndex(-1, 0xae, false);
	CGameObject *next = FindObjectByTag(m_next);
	if (next && next->NameContains((char *)"C3DCHECKPOINT"))
		static_cast<C3DCheckPoint *>(next)->Enable();		// 0x414ae0
}
