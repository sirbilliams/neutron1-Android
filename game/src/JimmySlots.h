/*  Neutron.exe's C3DJimmy element slots and members that core code (CJimmyGame, CLoadLevel) calls.
    The defaults here are weak no-ops (JimmySlots.cpp, TODO(E)); C3DJimmy defines the real ones
    (a strong definition replaces the weak one at link time). `jim` is Jimmy's CGameObject.     */
#pragma once

class CGameObject;

namespace neg {
void JimmyElementSlot88(CGameObject *jim, float a, float b, float c, float d);	// +0x160 0x42a7e0
void JimmyElementSlot89(CGameObject *jim);								// +0x164 0x42a8d0 on a surface 110..119 (each frame)
void JimmyElementSlot94(CGameObject *jim);								// +0x178 0x427ff0 put the item in hand away
void JimmyElementSlot95(CGameObject *jim);								// +0x17c 0x4299b0
void JimmyElementSlot99(CGameObject *jim);								// +0x18c on a surface 100..109 (each frame)
void JimmyElementSlot100(CGameObject *jim);								// +0x190 onto a surface 100..109
void JimmyElementSlot101(CGameObject *jim);								// +0x194 off it
void JimmyElementSlot102(CGameObject *jim);								// +0x198 onto a surface 110..119
void JimmyElementSlot103(CGameObject *jim);								// +0x19c off it
void JimmyElementSlot104(CGameObject *jim, float a, int b, float c);		// +0x1a0 hurt (-5, -5, 5 on surfaces 185..189)
void JimmyElementSlot121(CGameObject *jim, int sound_set);				// +0x1e4 0x42aec0 footstep sound set
void JimmyElementSlot128(CGameObject *jim, float dt);					// +0x200 0x424570 per frame (CJimmyGame::Process)
unsigned char &JimmyByte(CGameObject *jim, int off);						// +0x1e01 / +0x1e02 / +0x1e03 (surface flags)
}
