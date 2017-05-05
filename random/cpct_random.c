//-----------------------------LICENSE NOTICE------------------------------------
//  This file is part of CPCtelera: An Amstrad CPC Game Engine
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

static u32 cpct_mxor32_seed;
static u16 g_lfsr;

void cpct_setSeed_mxor(u32 newseed)
{
	cpct_mxor32_seed = newseed;
}

u32 cpct_nextRandom_mxor_u32(u32 seed)
{
	cpct_mxor32_seed = seed;
	srand(seed);

	return (u32)rand();
}

u32 cpct_nextRandom_mxor_u8(u32 seed)
{
	cpct_mxor32_seed = seed;
	srand(seed);

	return (u32)rand();
}

u32 cpct_nextRandom_mxor532_u8(u32 seed)
{
	cpct_mxor32_seed = seed;
	srand(seed);

	return (u32)rand();
}

u8 cpct_getRandom_mxor_u8()
{
	return (u8)rand();
}

u16 cpct_getRandom_mxor_u16()
{
	return (u16)rand();
}

u32 cpct_getRandom_mxor_u32()
{
	return (u32)rand();
}

void cpct_setSeed_glfsr16(u16 glfsrSeed)
{
	g_lfsr = glfsrSeed;
}

/** From http://www.cpcwiki.eu/forum/programming/fill-the-character-screen// */
u8 cpct_getRandom_glfsr16_u8()
{
	unsigned lastbit = g_lfsr & 1;

	// Shift Register by one
	g_lfsr >>= 1;

	// When last bit is one, tap the register
	if (lastbit)
		g_lfsr ^= 0x0240;

	return (u8)g_lfsr;
}

u16 cpct_getRandom_glfsr16_u16()
{
	return g_lfsr;
}

u8 cpct_getRandom_lcg_u8(u8 entropy_byte)
{
	cpct_mxor32_seed = entropy_byte;
	srand(entropy_byte);

	return (u8)rand();
}

u8 cpct_getRandom_xsp40_u8()
{
	return (u8)rand();
}

void cpct_setSeed_xsp40_u8(u16 seed8, u32 seed32)
{
	cpct_mxor32_seed = seed32;
}

void cpct_setSeed_lcg_u8(u8 newseed)
{
	cpct_mxor32_seed = newseed;
}