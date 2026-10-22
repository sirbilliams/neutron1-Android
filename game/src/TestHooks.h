/*  port: test-only hooks (desktop and Android; inert unless set), see TestHooks.cpp. */
#pragma once

class CGameObject;

namespace neg {
const char *TestPlayLevel(void);			// CEditor start-up: NEG_TEST_PLAY (or NULL) is the level to start with
void TestPlayHook(const char *level);		// end of CEditor::LoadGame: NEG_TEST_PLAY=<level.gam>
void TestPosLog(CGameObject *o, float dt);	// C3DJimmy::Process: NEG_TEST_POSLOG=<seconds>
}
