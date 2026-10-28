/*  CanvasWrapper - port-only (no counterpart in Neutron.exe): checked, not a reconstruction. */
#include "CanvasWrapper.h"

static int s_serial = 0;							

OMediaCanvasElementWrapper::OMediaCanvasElementWrapper()	// port
{
	m_c8 = -1;
	m_serial = s_serial++;
}

OMediaCanvasElementWrapper::~OMediaCanvasElementWrapper() {}	// port
