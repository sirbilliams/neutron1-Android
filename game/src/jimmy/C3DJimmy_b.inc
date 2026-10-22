// C3DJimmy part b: slot declarations and members (inside the class). See docs/decomp-guide.md and
// the C3DPlayer.h / C3DObject.h header comments for the conventions.
// The methods of Neutron.exe 0x448b40..0x44e8f0: the gadgets (ActivateMenuItem / n2_ev139), the
// water-gun and burp-gun shots (n2_ev144 / n2_ev148 / n2_ev152), the grappling hook and rope (n2_ev140..n2_ev143),
// damage (n2_ev119 / n2_ev120 / n2_ev134), water (EnterWater / LeaveWater), the footstep sounds (n2_ev115,
// n2_ev116, n2_ev153), Jimmy's animation choice (ev58, ev67, n2_ev162), ChangeState (v270), Read/WriteState.
// Element-family slots are `ev<n>` (`// ESLOT n`); a name from a log string gets an `ev<n>` alias.

	// ---- element-family slots (overrides)
	virtual void ev58(char *name, unsigned char loop);						// ESLOT 58  0x44c3e0 play animation <name> (Jimmy's Granny names; loop unused)
	virtual void ev67(void);												// ESLOT 67  0x44e0b0 "AnimEnded": end the pickup / gadget gestures
	virtual void n2_ev115(void);												// ESLOT 115 0x44b040 start the footstep sound (before RUN)
	virtual void n2_ev116(void);												// ESLOT 116 0x44b060 stop the footstep sound
	virtual void n2_ev118(void) {}												// ESLOT 118 0x44bc30 (f_cc0's empty 0x463890: nothing)
	virtual void n2_ev119(OMedia3DVector plane);								// ESLOT 119 0x44bc40 touched an electric wall (material 120..129): SHOCK
	virtual void n2_ev120(void);												// ESLOT 120 0x44be70 touched a wall of material 110..119: hurt

	// ---- element-family slots (new in C3DJimmy)
	virtual void n2_ev127(void) {}												// ESLOT 127 0x463890 (empty) CJimmyGame: in water, every frame
	virtual void EnterWater(void);											// ESLOT 128 0x44bf20 "Calling EnterWater"
	void n2_ev128(void) { EnterWater(); }
	virtual void LeaveWater(void);											// ESLOT 129 0x44bfa0 "Calling LeaveWater"
	void n2_ev129(void) { LeaveWater(); }
	virtual void n2_ev130(void);												// ESLOT 130 0x44bfe0 fell into water: SPLASH + "pain agh ahh"
	virtual void n2_ev131(void) {}												// ESLOT 131 0x463890 (empty) CJimmyGame: left that water
	virtual void n2_ev134(float energy, int points, float time, unsigned char shield);	// ESLOT 134 0x44bd90 hurt: energy, score, invulnerable for <time> (shield: pickup 20 halves it)
	virtual void n2_ev137(int which);											// ESLOT 137 0x44de40 gadget gestures: 1/4 water gun, 2/5 burp gun, 3 PDA prop
	virtual void ActivateMenuItem(CGameObject *o, CGameObject *who, short item, float value);	// ESLOT 138 0x449df0 "C3DJimmy::ActivateMenuItem()"
	void n2_ev138(CGameObject *o, CGameObject *who, short item, float value) { ActivateMenuItem(o, who, item, value); }
	virtual void n2_ev139(void);												// ESLOT 139 0x449450 deactivate the current gadget (g_menu GetCurrentGadget)
	virtual unsigned char n2_ev140(float dt);									// ESLOT 140 0x44b490 on the rope: swing, climb, hook and shadow (returns 0)
	virtual unsigned char n2_ev141(float dt);									// ESLOT 141 0x449050 grappling hook (gadget 7): fire upwards, or reel in
	virtual void n2_ev142(void);												// ESLOT 142 0x44b3a0 let go of the rope
	virtual void n2_ev143(OMedia3DPoint anchor, OMedia3DVector pos, int swing, float len);	// ESLOT 143 0x448e80 hang on a rope at <anchor>
	virtual void n2_ev144(void);												// ESLOT 144 0x448b40 fire the current gun (water / burp / shrink "click")
	virtual void n2_ev148(float x, float y, float z, float pitch, float speed, CGameObject *target, float delay, float growth);	// ESLOT 148 0x44b090 launch the next water balloon from v224(x, y, z)
	virtual void n2_ev149(float spin, CGameObject *target, C3DObject *obj);	// ESLOT 149 0x44e420 put <obj> 300 in front of the camera towards <target> (`this` unused)
	virtual void n2_ev150(void);												// ESLOT 150 0x44c2a0 remember the level id and our position
	virtual void n2_ev151(void);												// ESLOT 151 0x44c2f0 the same + HUD picture 19
	virtual void n2_ev152(int kind);											// ESLOT 152 0x44d9e0 water balloon look: 0 water gun, 1 burp gun, 3 (small)
	virtual void n2_ev153(int material);										// ESLOT 153 0x44c080 footstep sound of the floor material
	virtual unsigned char n2_ev158(float dt);									// ESLOT 158 0x448de0 out of energy: "not available" (1), else 0
	virtual void n2_ev159(float dt);											// ESLOT 159 0x449420 n2_ev160(dt * -0.05) while there is a Goddard
	virtual void n2_ev160(float d);											// ESLOT 160 0x44bed0 f_d60 += d, kept in 0..100
	virtual void n2_ev161(unsigned char show, int which);						// ESLOT 161 0x44e3b0 which 1: show / hide Goddard's f_1b80 object
	virtual unsigned char n2_ev162(char *name, char *anim);					// ESLOT 162 0x44c390 name is anim and not the state yet: state = anim, 1

	// ---- CGameObject-family overrides
	virtual unsigned char v133(void);										// SLOT 133 0x44d650 on the floor: also while f_4d90
	virtual void v220(float a, float b, float c, float d, OMedia3DPolygon *poly);	// SLOT 220 0x44bbe0 no wall contact while on the rope
	virtual void v261(void);												// SLOT 261 0x44c360 stop + n2_ev116 + "STOP"
	virtual void v270(int old_mode, int new_mode);							// SLOT 270 0x44e590 "C3DJimmy::ChangeState"
	virtual void v271(void);												// SLOT 271 0x44e850 power-up over: hook hidden too
	virtual void WriteState(OMediaStreamOperators &s);						// SLOT 284 0x44d860
	virtual void ReadState(OMediaStreamOperators &s);						// SLOT 285 0x44d670

	// (the members are declared in C3DJimmy.h)
