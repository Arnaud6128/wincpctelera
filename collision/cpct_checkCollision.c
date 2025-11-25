//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of WinCPCtelera: An Amstrad CPC Game Engine
//  Copyright (C) 2022 Arnaud Bouche (@Arnaud6128)
//  Copyright (C) 2022 ronaldo / Fremos / Cheesetea / ByteRealms (@FranGallegoBR)
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
//------------------------------------------------------------------------------

/** CPCTelera includes */
#include <cpctelera.h>

#define ValueInRange(value, min, max)	((value >= min) && (value < max))

u8 cpct_checkCollisionAxis(u8 x1, u8 w1, u8 x2, u8 w2)
{
	return	(ValueInRange(x1, x2, x2 + w2) || ValueInRange(x2, x1, x1 + w1));
}

u8 cpct_checkCollisionBoxes(u8 x1, u8 w1, u8 x2, u8 w2, u8 y1, u8 h1, u8 y2, u8 h2)
{
	return	(ValueInRange(x1, x2, x2 + w2) || ValueInRange(x2, x1, x1 + w1)) &&
		(ValueInRange(y1, y2, y2 + h2) || ValueInRange(y2, y1, y1 + h1));
}