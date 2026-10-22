/*  C3DFowl - see C3DFowl.h. Reconstructed from Neutron.exe N2:0x436540..0x41b490. */
#include "C3DFowl.h"
#include "../Game.h"
#include <string>

using namespace neg;

C3DFowl::C3DFowl()											// N2:0x436540
{
	SetName((char *)"C3DFOWL");
	Log("C3DFowl()");
	f_649 = 0;												// ev69 loads the animations later
	f_6d0 = 1;												// Granny
	FriendInitObjectAnim();									// 0x415090 inlined: InitObject() called directly
	C3DObject::SetObjectID('3FOW');							// 0x462220
	m_visible_range = 500.0f;											// VisibleRange
	m_speed = 160.0f;											// speed
	m_state = 2;												// patrolling
	m_ai_state = 2;												// AIState
	m_offset.x = 0;
	m_offset.y = 0;
	m_offset.z = 0;
	C3DObject::InitPhysics();								// 0x4623d0
	C3DAI::ev72(m_anim_run, (char *)"WALK");					// running is walking
	C3DAI::ev72(m_anim_wag, (char *)"STOP");					// waiting and sitting are stopping
	C3DAI::ev72(m_anim_sit, (char *)"STOP");
	f_1d60.y = 0;											// the shadow at our origin
	// (then a call to the empty 0x472970)
}

C3DFowl::~C3DFowl()											// 0x41b250
{
}

namespace neg {
CGameObject *New_C3DFowl(void) { return new C3DFowl; }					// '3FOW' (0x1d7c bytes)
}

void C3DFowl::InitObject(void)		{ FriendInitObjectAnim(); }			// 0x415090
void C3DFowl::v260(void)			{ FriendPropsLoadedShow(); }		// N2:0x417110
void C3DFowl::v266(void)			{ FriendTaskStateRewrite(); }		// 0x419aa0
void C3DFowl::v267(int level)		{ FriendLevelScene(level); }		// N2:0x436900
void C3DFowl::ev60(void)			{ FriendNoAnimIdle(); }				// N2:0x4369f0

void C3DFowl::v270(int old_mode, int new_mode)				// N2:0x436920
{
	FriendChangeState(old_mode, new_mode, "TALK");			// "C3DFowl::ChangeState %d  %d"
}

void C3DFowl::ev69(void)									// N2:0x436760
{
	if (f_649) return;
	f_649 = 1;
	InitModel(std::string("fowlbase.grn"));					// element +0x144
	CreateAnim((char *)"TALK", (char *)"fowltalk.grn");		// +0xe0
	CreateAnim((char *)"YELL", (char *)"fowldino.grn");
	CreateAnim((char *)"STOP", (char *)"fowlstop.grn");
	CreateAnim((char *)"IDLE", (char *)"fowlidle.grn");
	CreateAnim((char *)"TALK2", (char *)"fowltalknorm.grn");
	ev58((char *)"STOP", 1);								// +0xe8
	v67(150.0f);											// CGameObject +0x10c: radius
	v64(0.05f);												// CGameObject +0x100
}
