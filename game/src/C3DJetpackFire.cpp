/*  C3DJetpackFire - see C3DJetpackFire.h. 
    Reconstructed from Neutron.exe  */
#include "C3DJetpackFire.h"
#include "Game.h"
#include "Bridge.h"
#include <strings.h>

using namespace neg;

namespace neg { void JimmyAddEnergy(float d); }		// 0x42a920 (C3DJimmy.cpp)

C3DJetpackFire::C3DJetpackFire()							// 0x421d80
{
	SetName((char *)"C3DJETPACKFIRE");
	C3DJetpackFire::InitObject();							// 0x421f00 (called directly)
	v64(0.3f);
	C3DObject::SetObjectID('3JFI');							// 0x462220
	v107(0);												// +0x1b0
	v36(0);
	v52(0);
	f_924 = NULL;
	f_928 = 0;
}

C3DJetpackFire::~C3DJetpackFire()							// 0x421eb0
{
}

namespace neg {
CGameObject *New_C3DJetpackFire(void)	{ return new C3DJetpackFire; }	// factory '3JFI' (0x6cc bytes)
}

void C3DJetpackFire::InitObject(void)						// 0x421f00
{
	Log("InitObject()");
	C3DAnimated::InitObject();								// 0x40d530
	InitAnim();												// element +0x108
	CreateAnim((char *)"HIDEFAULT", (char *)"fire.ase");	// element +0xd8
	CreateAnim((char *)"HINONE", (char *)"fire2.ase");
	CreateTexture((char *)"fire.png", 0);					// element +0xf0
	ApplyTexture(f_590, 0);									// element +0xf4 (BG +0x57c)
	v67(20.0f);												// +0x110
	ev58((char *)"DEFAULT", 1);								// element +0xe0
}

// Every frame: on the owner's back while the burn time lasts (it uses energy), hidden otherwise;
// landing ends the burn.
void C3DJetpackFire::Process(float dt)						// 0x421fb0
{
	if (!GameIsRunning()) return;							// 0x475ca0
	C3DAnimated::Process(dt);								// 0x40e050
	if (!f_924) return;
	if (f_924->v133()) f_928 = 0;							// on the floor (+0x218)
	f_928 -= dt;
	if (f_928 > 0)
	{
		force_show();
		JimmyAddEnergy(-dt * 0.5f);							// 0x42a920
	}
	else hide(true);
	float p[4];
	f_924->v195(p);											// +0x310
	v196(p[0], p[1], p[2], p[3]);							// +0x314
	f_924->v201(p);											// +0x328
	v202(p[0], p[1], p[2], p[3]);							// +0x32c
}
