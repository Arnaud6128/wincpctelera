//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of Throne Legacy
//  This file is part of CPCtelera: An Amstrad CPC Game Engine
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

/*****************************************************/
/*                                                   */
/*    cpct_spriteColourizeM1 C version               */
/*                                                   */
/*****************************************************/
#ifdef WINCPCTELERA
#define RRCA(N, D) (N = ((N >> D) | (N << (8 - D))))

void cpct_spriteColourizeM1(u16 rplcPat, u16 size, void* sprite)
{
    u8 E = (u8)rplcPat; // New pen
    u8 D = (u8)(rplcPat >> 8); // Old pen
    u8* HL = (u8*)sprite;

    u8 A;
    u8 IXL;

    A = E;
    A = A ^ D;
    E = A;

    IXL = D;

    for (u16 BC = 0; BC < size; BC++)
    {
        A = IXL;

        A = A ^ *HL;
        D = A;

        RRCA(A, 4);
        A = A | D;

        A = ~A;

        A = A & E;
        A = A ^ *HL;

        *HL = A;
        HL++;
    }
}

#endif