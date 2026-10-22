/*  C3DCindy - see C3DCindy.h. Reconstructed from Neutron.exe N2:0x427600..0x4152a0. */
#include "C3DCindy.h"
#include "../Game.h"
#include <string>

using namespace neg;

C3DCindy::C3DCindy()										// N2:0x427600
{
	SetName((char *)"C3DCINDY");
	Log("C3DCindy()");
	f_649 = 0;												// ev69 loads the animations later
	f_6d0 = 1;												// Granny
	FriendInitObjectAnim();									// 0x415090 inlined: InitObject() called directly
	C3DObject::SetObjectID('3CIN');							// 0x462220
	m_visible_range = 500.0f;											// VisibleRange
	m_speed = 160.0f;											// speed
	m_state = 2;												// patrolling
	m_ai_state = 2;												// AIState
	m_offset.x = 0;
	m_offset.y = 0;
	m_offset.z = 300.0f;
	C3DObject::InitPhysics();								// 0x4623d0
	C3DAI::ev72(m_anim_run, (char *)"WALK");					// running is walking
	C3DAI::ev72(m_anim_wag, (char *)"STOP");					// waiting and sitting are stopping
	C3DAI::ev72(m_anim_sit, (char *)"STOP");
	f_934 = -1;												// (already -1)
	f_1d60.y = -80.0f;										// the shadow
	// (then a call to the empty 0x472970)
}

C3DCindy::~C3DCindy()										// 0x414f50
{
}

namespace neg {
CGameObject *New_C3DCindy(void) { return new C3DCindy; }				// '3CIN' (0x1d7c bytes)
}

void C3DCindy::InitObject(void)		{ FriendInitObjectAnim(); }			// 0x415090
void C3DCindy::v260(void)			{ FriendPropsLoadedShow(); }		// N2:0x417110
void C3DCindy::ev60(void)			{ FriendNoAnimIdle(); }				// N2:0x4369f0

void C3DCindy::v270(int old_mode, int new_mode)				// N2:0x427a40
{
	FriendChangeState(old_mode, new_mode, "IDLE");			// "C3DCindy::ChangeState %d  %d"
}

void C3DCindy::ev69(void)									// N2:0x427830
{
	if (f_649) return;
	f_649 = 1;
	InitModel(std::string("cindybase.grn"));				// element +0x144
	CreateAnim((char *)"WALK", (char *)"cindywalk.grn");	// +0xe0
	CreateAnim((char *)"TALK", (char *)"cindytalk.grn");
	CreateAnim((char *)"STOP", (char *)"cindystop.grn");
	CreateAnim((char *)"LOVE", (char *)"cindyinlove.grn");
	CreateAnim((char *)"LOVETALK", (char *)"cindylovetalk.grn");
	CreateAnim((char *)"LOVEIDLE", (char *)"cindyloveidle.grn");
	CreateAnim((char *)"DISGTALK", (char *)"cindytalkdis.grn");
	CreateAnim((char *)"IDLE", (char *)"cindyidle.grn");
	CreateAnim((char *)"TALKFLURP", (char *)"cindytalkflurp.grn");
	CreateAnim((char *)"DRINKFLURP", (char *)"cindydrinkflurp.grn");
	CreateAnim((char *)"HOLDFLURP", (char *)"cindyholdflurp.grn");
	ev58((char *)"STOP", 1);								// +0xe8
	v67(60.0f);												// CGameObject +0x10c: radius
	v64(1.45f);												// CGameObject +0x100
}
