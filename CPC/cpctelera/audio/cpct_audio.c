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

#include <winCpctelera.h>

void cpct_akp_musicInit(void* songdata)
{
}

void cpct_akp_musicPlay() 
{
}

void cpct_akp_stop()
{
}

void cpct_akp_SFXInit(void* sfx_song_data) 
{
}

void cpct_akp_SFXStopAll()
{
}

void cpct_akp_SFXStop(u8 stop_bitmask)
{
}

void cpct_akp_SFXPlay(u8 sfx_num, u8 volume, u8 note, u8 speed, u16 inverted_pitch, u8 channel_bitmask)
{
}

u16 cpct_akp_SFXGetInstrument(u8 channel_bitmask)
{
	return 0;
}

void cpct_akp_setFadeVolume(u8 volume) 
{
}