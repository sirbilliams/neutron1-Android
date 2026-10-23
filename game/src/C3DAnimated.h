/*  C3DAnimated - a C3DObject that plays OMT morph animations built from .ase files. The base of
    every character (C3DPlayer -> C3DJimmy, C3DAI ...) and of the animated level objects.
    Reconstructed from Neutron.exe: constructor 0x40d110, destructor 0x40d300 (nothing of its own;
    deleting thunks 0x40d2d0 / 0x40e850 / 0x40e860 / 0x40e870). No factory ID of its own.

    Original layout (classes.json): as C3DObject (element vtable 0x4902bc, 72 slots: C3DObject's
    54 + 54..71; OMediaAnim at +0x90, vtable 0x49026c; listener +0x30, vtable 0x4902a8;
    CLocalGameObject at +0xc0, vtable 0x48fe1c, 275 slots), own members 0x578..0x6bc, vtordisp
    0x6bc, streamer 0x6c0. (Negatron's C3DAnimated adds Granny skeletal animation, fades, tints,
    states and culling: none of it is in Neutron.exe.)

    InitAnim takes the base shape (chunk 0) of the object's private database; CreateAnim(name,
    file) imports an .ase animation into a new morph definition kept in the AnimList chain at
    f_5a4; ev58(name, loop) switches to one ("HI" + name with the full shape).

    Element-family game slots are `ev<n>` (`// ESLOT n`, n = vtable offset / 4) unless a log
    string names them; CGameObject-family slots keep their CGameObject names (`// SLOT n`).
    Members keep the names of Negatron's offsets (f_<offset>); Neutron.exe's offset is 0x14
    lower (given in each comment). CGameObject-family code sees them a further 0xc0 lower.     */
#pragma once
#include <list>
#include <string>
#include "C3DObject.h"

class OMediaCanvas;
class OMedia3DMaterial;
class OMedia3DMorphAnimDef;
class OMediaMatrix_4x4;

// One morph animation of an OMT-animated object (0x50 bytes, "AnimList" in the logs).
struct AnimList
{
	char					name[64];		// +0x00  g_4f81a8 + animation name (63 chars max)
	AnimList				*next;			// +0x40
	int						id;				// +0x44  sequence number (f_5a0 when created)
	int						nframes;		// +0x48  frames of the current sequence (SetSequence)
	OMedia3DMorphAnimDef	*def;			// +0x4c  the imported morph definition (database 'A3dm')
};

class C3DAnimated : public C3DObject
{
	public:
	C3DAnimated();															// 0x40d110
	virtual ~C3DAnimated();													// 0x40d300

	// ---- element-family game slots (vtable offset = slot * 4)
	virtual void CreateAnim(char *name, char *file);						// ESLOT 54 0x40d4a0 "Anim3D_CreateAnim"
	virtual AnimList *GetSequence(char *name);								// ESLOT 55 0x40d9e0 "Anim3D_GetSequence"
	virtual void ev58(char *name, unsigned char loop);						// ESLOT 56 0x40dd90 play animation "HI"/"LOW" + <name>
	virtual void SetSequence(int seq, unsigned char loop);					// ESLOT 57 0x40da30 "Anim3D_SetSequence(long)"
	virtual char *GetAnim(void);											// ESLOT 58 0x40dab0 "Anim3D_GetAnim": name of the current AnimList
	virtual int  GetAnimCount(void);										// ESLOT 59 0x40db10 f_5a0: number of morph animations
	virtual void CreateTexture(char *file, short index);					// ESLOT 60 0x40db20 "Anim3D_CreateTexture": png\<file> into f_5ac/f_5d4[index]
	virtual void ApplyTexture(OMedia3DShape *shape, short index);			// ESLOT 61 0x40dd60 "Anim3D_ApplyTexture"
	virtual void InitMemDB(void);											// ESLOT 62 0x40df90 the shared in-memory database (0x4f840c / 0x4f8410)
	virtual void ev65(short index, int src, int dst);						// ESLOT 63 0x40dd40 blend of material [index]
	virtual char *ev66(void);												// ESLOT 64 0x40df80 name of the animation last asked for (f_5fc)
	virtual void ev67(void) {}												// ESLOT 65 0x472970 (empty) the animation ended (Process)
	virtual void InitAnim(void);											// ESLOT 66 0x40e270 "InitAnim": morph base shape from the object database
	virtual void ev69(void) {}												// ESLOT 67 0x472970 (empty) after InitAnim / before ev70(true)
	virtual void ev70(unsigned char show);									// ESLOT 68 0x40e3e0 show/hide with collision (level visibility)
	virtual void ev71(float alpha);											// ESLOT 69 0x40e4a0 additive glow at <alpha> (<= 0: opaque)
	virtual void AlphaSecondPass(void);										// ESLOT 70 0x40e5e0 alpha blending, second pass
	virtual void PauseAnim(unsigned char pause);							// ESLOT 71 0x40d350 pause the morph animation

	// ---- not virtual
	unsigned char IsAnim(char *name);										// 0x40e1d0 the current animation (f_5fc) is <name>

	// ---- CGameObject-family overrides
	virtual void InitObject(void);											// SLOT 7   0x40d3c0 properties
	virtual void UnInitObject(void);										// SLOT 8   0x40e670
	virtual void Process(float dt);											// SLOT 241 0x40e050
	virtual void v242(float t);												// SLOT 242 0x40d3a0 (editing) shown
	virtual void v260(void);												// SLOT 259 0x40e7b0 properties loaded: InitiallyVisible, SecondPass
	virtual void v267(int level);											// SLOT 265 0x40e340 RequiredLevel/ExactLevel/RemoveLevel -> ev70
	virtual void v268(int state);											// SLOT 266 0x40e1f0 "NextState": 1 show, 0 hide
	virtual void v277(void);												// SLOT 272 0x40e770 PauseAnim(1)
	virtual void v278(void);												// SLOT 273 0x40e790 PauseAnim(0)

	// ---- data members (Neutron.exe offsets in the comments)
	OMedia3DShape		*f_58c;			// 0x578  the LOW base shape (animations named "LOW...") - none so far
	OMedia3DShape		*f_590;			// 0x57c  the HI base shape (object database '3DSh' chunk 0)
	short				f_594;			// 0x580  detail: -1 none, 0 HI (InitAnim), 1 LOW: ev58 plays "HI"/"LOW" + name
	float				f_598;			// 0x584  time in the current animation (+= dt)
	int					f_59c;			// 0x588  current sequence (-1)
	int					f_5a0;			// 0x58c  number of animations (AnimList ids)
	AnimList			*f_5a4;			// 0x590  first animation
	int					f_5a8;			// 0x594  0
	OMediaCanvas		*f_5ac[10];		// 0x598  textures (CreateTexture)
	OMedia3DMaterial	*f_5d4[10];		// 0x5c0  their materials
	char				f_5fc[64];		// 0x5e8  name of the animation last asked for (ev58)
	unsigned char		f_63c;			// 0x570  InitAnim done (0x628 is only zeroed by the constructor)
	AnimList			*f_640;			// 0x62c  current animation
	unsigned char		f_648;			// 0x634  1: animations on
	unsigned char		f_649;			// 0x635  1: animations loaded (C3DCindy & co: 0 until their first ev69)
	int					f_64c;			// 0x638  "RequiredLevel" (-1)
	int					f_650;			// 0x63c  "ExactLevel" (-1)
	int					f_654;			// 0x640  "RemoveLevel" (-1)
	int					f_658;			// 0x644  "HasCollision" (-1: leave as is)
	int					f_65c;			// 0x648  "InitiallyVisible" (-1)
	int					f_660;			// 0x64c  "CanMove" (1; 0: kept at its start position)
	int					f_664;			// 0x650  "SecondPass" (0)
	unsigned char		f_668;			// 0x654  animation paused (PauseAnim)
	char				f_669[100];		// 0x655  "PickupLink" ("none"): hidden once that pickup is taken
	unsigned char		f_6cd;			// 0x6b9  PickupLink checked
};
