//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of CPCtelera: An Amstrad CPC Game Engine
//  Copyright (C) 2015 ronaldo / Fremos / Cheesetea / ByteRealms (@FranGallegoBR)
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

//
// Title: Clip sprite
//
#ifndef _CPCT_CLIP_H_
#define _CPCT_CLIP_H_

// Clipped sprite draw functions 
extern void cpct_drawSpriteClipped(u16 width_sprite_to_draw, u8* destMemory, u8 width, u8 height, u8 *sprite) __z88dk_callee;
extern void cpct_drawSpriteClippedMasked(u16 width_sprite_to_draw, u8* destMemory, u8 width, u8 height, u8 *sprite) __z88dk_callee;
extern void cpct_drawSpriteClippedMaskedAlignedTable(u16 width_sprite_to_draw, u8* destMemory, u8 width, u8 height, u8 *sprite, u8 *mask_table) __z88dk_callee;

#endif
