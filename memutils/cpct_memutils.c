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

extern int GetPageAddress(int pPage);
extern u8* GetVideoBufferFromAddress(int pScreenAddr);

BOOL IsCpcMem(const void* pAddress)
{
	return ((int)pAddress < CPC_MEM_SIZE);
}

BOOL IsVideoMem(const void* pAddress)
{
	int videoAdress = GetPageAddress(_amstrad._currentPage);
	return ((int)pAddress >= videoAdress && (int)pAddress <= (videoAdress + CPC_BANK_SIZE));
}

u8* GetMemory(const void* ptr)
{
	if (IsCpcMem(ptr))
		return (u8*)(_amstrad._memCPC) + (int)ptr;

	return (u8*)ptr;
}

void cpct_memcpy(void* to, const void* from, u16 size)
{
	to = GetMemory(to);
	from = GetMemory(from);

	memcpy_s(to, size, from, size);
}

void cpct_memset_f64(void *array, u16 value, u16 size)
{
	u8* data = (u8*)GetMemory(array);

	for (int i = 0; i < size; i++)
		data[i] = (u8)value;
}

void cpct_memset_f8(void *array, u16 value, u16 size)
{
	cpct_memset(array, (u8)value, size);
}

void cpct_memset(void *array, u8 value, u16 size)
{	
	if (IsVideoMem(array))
	{
		int address = (int)array;
		for (int i = 0; i < size; i++) 
		{
			u8* videoData = GetVideoBufferFromAddress(address + i);
			*videoData = value;
		}
	}
	else
	{
		u8* data = (u8*)GetMemory(array);
		memset(data, value, size);
	}
}

void cpct_setStackLocation(void* memory)
{
	CPCTeleraWin();
}