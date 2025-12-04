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
#include <cpctelera.h>

#ifdef WINCPCTELERA

void cpct_fdcOn()
{
}

void cpct_fdcOff()
{
}

#define MAX_BUFFER_SIZE	65536

void cpct_loadFile(u8* fileName, u8* destBuffer, u8* sectorTable)
{
	FILE* stream = NULL;
	fopen_s(&stream, fileName, "r");

	if (stream != NULL)
	{
		fread_s(WinCpcTelera_GetMemPtr(destBuffer), MAX_BUFFER_SIZE, MAX_BUFFER_SIZE, 1, stream);
		fclose(stream);
	}
}

void asm_LoadSectorTable(u8* sectorTable)
{
}

#endif
