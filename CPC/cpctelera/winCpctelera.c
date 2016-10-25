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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern void CreateWindowApp();
void StartCPC();

SAmstrad _amstrad;

void CPCTeleraWin()
{
	static BOOL _isStarted = FALSE;
	if (!_isStarted)
	{
		_isStarted = TRUE;

		StartCPC();
		CreateWindowApp();
	}
}

void StartCPC()
{
	ZeroMemory(&_amstrad, sizeof(_amstrad));
	for (int i = 0; i < NB_COLORS; i++)
		_amstrad._curPal[i] = _palette[i].hw;

	memset(&_amstrad._memCPC, 0, CPC_MEM_SIZE);

	_amstrad._mode = 1;
	_amstrad._curPal[0] = HW_BRIGHT_BLUE;
	_amstrad._curPal[1] = HW_BRIGHT_YELLOW;
	_amstrad._curPal[BORDER_COLOR] = HW_BRIGHT_BLUE;

	_amstrad._currentPage = cpct_pageC0;

	StartInterrupt();
}