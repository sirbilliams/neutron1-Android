/*  Weak defaults of JimmySlots.h: nothing happens until C3DJimmy (agent E) defines them. */
#include "JimmySlots.h"

#define WEAK __attribute__((weak))
namespace neg {
WEAK void JimmyElementSlot88(CGameObject *jim, float a, float b, float c, float d)	{ (void)jim; (void)a; (void)b; (void)c; (void)d; }
WEAK void JimmyElementSlot89(CGameObject *jim)						{ (void)jim; }
WEAK void JimmyElementSlot94(CGameObject *jim)						{ (void)jim; }
WEAK void JimmyElementSlot95(CGameObject *jim)						{ (void)jim; }
WEAK void JimmyElementSlot99(CGameObject *jim)						{ (void)jim; }
WEAK void JimmyElementSlot100(CGameObject *jim)						{ (void)jim; }
WEAK void JimmyElementSlot101(CGameObject *jim)						{ (void)jim; }
WEAK void JimmyElementSlot102(CGameObject *jim)						{ (void)jim; }
WEAK void JimmyElementSlot103(CGameObject *jim)						{ (void)jim; }
WEAK void JimmyElementSlot104(CGameObject *jim, float a, int b, float c)	{ (void)jim; (void)a; (void)b; (void)c; }
WEAK void JimmyElementSlot121(CGameObject *jim, int sound_set)		{ (void)jim; (void)sound_set; }
WEAK void JimmyElementSlot128(CGameObject *jim, float dt)			{ (void)jim; (void)dt; }
WEAK unsigned char &JimmyByte(CGameObject *jim, int off)
{
	(void)jim;
	static unsigned char b[3];
	return b[(off - 0x1e01) % 3];
}
}
