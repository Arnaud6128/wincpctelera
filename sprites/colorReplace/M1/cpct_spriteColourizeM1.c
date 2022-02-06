//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of Throne Legacy
//  Copyright (C) 2020 Arnaud Bouche (@Arnaud6128)
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
void cpct_spriteColourizeM1(u16 rplcPat, u16 size, void* sprite)
{
    u8 InsrPat = (u8)rplcPat;
    u8 FindPat = (u8)(rplcPat >> 8);

    u8 InsrPatXorFindPat = InsrPat ^ FindPat;

    u8* spritePtr = (u8*)sprite;

    for (u16 i = 0; i < size; i++)
    {
        u8 SpriteByteXorFindPat = *spritePtr ^ FindPat;
        u8 SpriteByte = SpriteByteXorFindPat;

        SpriteByte = SpriteByte >> 4;
        SpriteByte = SpriteByte | SpriteByteXorFindPat;
        SpriteByte = ~SpriteByte;

        SpriteByte = SpriteByte & InsrPatXorFindPat;
        SpriteByte = SpriteByte ^ *spritePtr;
      
        *spritePtr++ = SpriteByte;
    }
}
#endif