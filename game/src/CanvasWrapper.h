/*  OMediaCanvasElementWrapper - an OMediaCanvasElement with a serial number (0xd8 bytes in the
    original). Every 2D sprite of the menus and HUD is one.
    port: Negatron's class (Neutron.exe has none: its sprites are plain OMediaCanvasElements);
    the port's own UI (port/BillsUi.cpp) uses it.                          */
#pragma once
#include "OMediaCanvasElement.h"

class OMediaCanvasElementWrapper : public OMediaCanvasElement
{
	public:
	OMediaCanvasElementWrapper();				// port
	virtual ~OMediaCanvasElementWrapper();		// port

	int		m_c8;			// 0xc8  = -1
	int		m_serial;		// 0xcc  from a counter
};
