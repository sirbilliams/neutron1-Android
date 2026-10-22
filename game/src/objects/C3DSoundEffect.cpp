/*  C3DSoundEffect - see C3DSoundEffect.h. Reconstructed from Neutron.exe 0x440e20..0x441250. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"		// (double constants in OMedia3DShape.h)
#include "C3DSoundEffect.h"
#include "../CGameType.h"
#include "../Game.h"
#include "../Globals.h"
#include "../SaveGame.h"
#include "../Sound.h"
#include "../gen/globals.h"
#pragma GCC diagnostic pop
#include <string.h>

using namespace neg;

// --------------------------------------------------------------------------- construction

C3DSoundEffect::C3DSoundEffect()							// 0x440e20
{
	SetName((char *)"C3DSOUNDEFFECT");
	Log("C3DSoundEffect()");
	C3DSoundEffect::InitObject();							// 0x441020 (called directly)
	SetObjectID('3SOU');									// 0x462d80
	m_sprite_index = 8;
	m_sprite_size = 50;
	strcpy(m_sprite_db, "icons.omt");
	m_sound_index = -1;
	v67(250.0f);											// radius
	v46(0);
	v42(0);
	v40(0);
	v52(1);
	m_times_to_trigger = -1;
	m_times_triggered = 0;
	f_008 = 1;
	C3DSprite::InitPhysics();								// 0x464210
	v34(1);
	m_is_ambient = 0;
	m_channel = -1;
	m_required_level = 0;
}

C3DSoundEffect::~C3DSoundEffect()							// 0x440fe0
{
}

namespace neg {
CGameObject *New_C3DSoundEffect(void) { return new C3DSoundEffect; }	// '3SOU' (0x608 bytes)
}

// --------------------------------------------------------------------------- CGameObject family

void C3DSoundEffect::InitObject(void)						// 0x441020 (no log line)
{
	C3DSprite::InitObject();								// 0x464130
	RegisterProperty("SoundIndex", &m_sound_index, PROP_LONG, 0);
	RegisterProperty("TimesToTrigger", &m_times_to_trigger, PROP_LONG, 0);
	RegisterProperty("Radius", &f_034, PROP_FLOAT, 0);
	RegisterProperty("IsAmbient", &m_is_ambient, PROP_LONG, 0);
	RegisterProperty("RequiredLevel", &m_required_level, PROP_LONG, 0);
}

void C3DSoundEffect::AddTouching(CGameObject *other)		// 0x4410b0
{
	CGameObject::AddTouching(other);						// 0x470a90
	if (other != g_camera || m_is_ambient) return;
	if (m_times_to_trigger != -1 && m_times_to_trigger <= m_times_triggered) return;
	m_times_triggered++;
	if (m_sound_index == -1) return;
	if (m_required_level <= GetTaskState(m_task_name))		// 0x45fea0
		PlayGameSoundIndex(-1, m_sound_index, false);		// 0x458980
}

void C3DSoundEffect::Process(float dt)						// 0x441130
{
	C3DSprite::Process(dt);									// 0x463090 (no pause check)
	if (!m_is_ambient) return;
	if (!g_sfx_db_loaded)									// 0x4f8434
	{
		if (m_channel != -1)
		{
			StopChannel(m_channel, false);					// 0x47d7a0
			m_channel = -1;
		}
		return;
	}
	if (IsChannelPlaying(m_channel) || m_channel != -1) return;	// 0x47d890
	if (GetTaskState(m_task_name) < m_required_level) return;	// 0x45fea0
	if (m_sound_index == -1) return;
	m_channel = PlayGameSound3DIndex(this, -1, m_sound_index, true);	// 0x4589c0
}

void C3DSoundEffect::v260(void)								// 0x4411e0 (jmp 0x441ac0)
{
	C3DSprite::v260();
}
