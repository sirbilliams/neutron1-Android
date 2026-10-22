#include "CanvasWrapper.h"

static int s_serial = 0;							// N2:0x54a4d4

OMediaCanvasElementWrapper::OMediaCanvasElementWrapper()	// N2:0x496a40
{
	m_c8 = -1;
	m_serial = s_serial++;
}

OMediaCanvasElementWrapper::~OMediaCanvasElementWrapper() {}	// N2:0x496b50
