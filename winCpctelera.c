//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of CPCtelera: An Amstrad CPC Game Engine
//  
//  Copyright (C) 2016 Arnaud Bouche
//  Copyright (C) 2016 ronaldo / Fremos / Cheesetea / ByteRealms (@FranGallegoBR)
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//-------------------------------------------------------------------------------

#include "winCpctelera.h"

#ifdef const
	#undef const
#endif

void wincpct_createWindowApp();
void wincpct_startCPC();

SAmstrad gAmstrad;

void wincpct_CPCTeleraWin()
{
	static BOOL _isStarted = FALSE;
	if (!_isStarted)
	{
		_isStarted = TRUE;

		wincpct_createWindowApp();
		wincpct_startCPC();
	}
}

void wincpct_startCPC()
{
	ZeroMemory(&gAmstrad, sizeof(gAmstrad));
	memset(&gAmstrad._memCPC, 0, CPC_MEM_SIZE);
	memset(&gAmstrad._bankCPC, 0, CPC_NB_BANKS*CPC_BANK_SIZE);

	for (int i = 0; i < NB_COLORS; i++)
		gAmstrad._curVideoConf.gCpcPalette[i] = gCpcPalette[i].hw;

	gAmstrad._curVideoConf._videoMode = 1;
	gAmstrad._curVideoConf.gCpcPalette[0] = HW_BLUE;
	gAmstrad._curVideoConf.gCpcPalette[1] = HW_BRIGHT_YELLOW;
	gAmstrad._curVideoConf.gCpcPalette[2] = HW_PASTEL_CYAN;
	gAmstrad._curVideoConf.gCpcPalette[3] = HW_BRIGHT_RED;
	gAmstrad._curVideoConf.gCpcPalette[4] = HW_BRIGHT_WHITE;
	gAmstrad._curVideoConf.gCpcPalette[5] = HW_BLACK;
	gAmstrad._curVideoConf.gCpcPalette[6] = HW_BRIGHT_BLUE;
	gAmstrad._curVideoConf.gCpcPalette[7] = HW_BRIGHT_MAGENTA;
	gAmstrad._curVideoConf.gCpcPalette[8] = HW_CYAN;
	gAmstrad._curVideoConf.gCpcPalette[9] = HW_YELLOW;
	gAmstrad._curVideoConf.gCpcPalette[10] = HW_PASTEL_BLUE;
	gAmstrad._curVideoConf.gCpcPalette[11] = HW_PINK;
	gAmstrad._curVideoConf.gCpcPalette[12] = HW_BRIGHT_GREEN;
	gAmstrad._curVideoConf.gCpcPalette[13] = HW_PASTEL_GREEN;
		
	gAmstrad._curVideoConf.gCpcPalette[BORDER_COLOR] = HW_BLUE;

	gAmstrad._currentPage = cpct_pageC0;
	gAmstrad._currentBank = RAMCFG_0;

	wincpct_startInterrupt();
}