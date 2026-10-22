/*  C3DLibby - see C3DLibby.h. Reconstructed from Neutron.exe 0x42cd40..0x42d410. */
#include "C3DLibby.h"
#include "../Game.h"

using namespace neg;

C3DLibby::C3DLibby()										// 0x42cd40
{
	SetName((char *)"C3DLIBBY");
	Log("C3DLibby()");
	f_649 = 0;												// ev69 loads the animations later (morph: f_6d0 stays 0)
	FriendInitObjectAnim();									// 0x415090 inlined: InitObject() called directly
	C3DObject::SetObjectID('3LIB');							// 0x462220
	m_visible_range = 500.0f;											// VisibleRange
	m_speed = 200.0f;											// speed
	m_state = 3;												// patrolling only
	m_ai_state = 3;												// AIState
	m_offset.x = 0;
	m_offset.y = 0;
	m_offset.z = 300.0f;
	C3DObject::InitPhysics();								// 0x4623d0
	C3DAI::ev72(m_anim_run, (char *)"RUN");
	C3DAI::ev72(m_anim_wag, (char *)"STOP");					// waiting and sitting are stopping
	C3DAI::ev72(m_anim_sit, (char *)"STOP");
	// (then a call to the empty 0x472970)
}

C3DLibby::~C3DLibby()										// 0x42cee0
{
}

namespace neg {
CGameObject *New_C3DLibby(void) { return new C3DLibby; }				// '3LIB' (0x1d7c bytes)
}

void C3DLibby::InitObject(void)		{ FriendInitObjectAnim(); }			// 0x415090
void C3DLibby::v266(void)			{ FriendTaskStateRewrite(); }		// 0x419aa0
void C3DLibby::v267(int level)		{ FriendLevelSceneShow(level); }	// 0x4418e0
void C3DLibby::ev145(void)			{ FriendTouchScene(); }				// N2:0x44fa60

void C3DLibby::ev147(void)									// N2:0x44fb20
{
	ApplyTexture(f_590, 1);									// element +0x100: libbywire.png
}

void C3DLibby::ev148(void)									// N2:0x44fb40
{
	ApplyTexture(f_590, 0);									// libby.png
}

void C3DLibby::v260(void)									// N2:0x44fa30
{
	C3DAI::v260();											// 0x409480
	ApplyTexture(f_590, 0);									// element +0x100
}

void C3DLibby::ResetObject(void)							// 0x42d190 (as C3DSheen's, without InitPhysics)
{
	float p[4];
	v88(p);													// +0x160: the start position
	v196(p[0], p[1], p[2], p[3]);							// +0x310
	float q[4];
	v195(q); const float qz = q[2];							// +0x30c, three times (as shipped)
	v195(q); const float qy = q[1];
	v195(q);
	x = q[0];												// the element's position (2002 +0x44..)
	y = qy;
	z = qz;
	C3DFriends::ResetObject();								// 0x41b790
	m_dest = NULL;											// no destination
}

void C3DLibby::ev69(void)									// 0x42cf30
{
	if (f_649) return;
	f_649 = 1;
	CreateAnim((char *)"TALK", (char *)"libytalk.ase");		// +0xe0
	CreateAnim((char *)"WALK", (char *)"libywalk.ase");
	CreateAnim((char *)"RUN", (char *)"libyrun.ase");
	CreateAnim((char *)"DIGITIZE", (char *)"libbyvox.ase");
	CreateAnim((char *)"FREEZE", (char *)"libyvoxstop.ase");
	CreateAnim((char *)"STOP", (char *)"libystop.ase");
	CreateTexture((char *)"libby.png", 0);					// +0xfc
	CreateTexture((char *)"libbywire.png", 1);
	ApplyTexture(f_590, 0);									// +0x100
	ev58((char *)"STOP", 1);								// +0xe8
	v67(150.0f);											// CGameObject +0x10c: radius
	v64(0.5f);												// CGameObject +0x100
}
