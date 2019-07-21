#include <winCpctelera.h>

static u8* GetByte(void *array, int pos, int nbBits)
{
	int offset = pos * nbBits;
	u8* byte = ((u8*)array) + offset / 8;
	return byte;
}

static u8 GetBitPos(int pos, int nbBits)
{
	int offset = pos * nbBits;
	return offset % 8;
}

static u8 GetBits(void *array, u16 pos, int size)
{
	u8 bitPos = GetBitPos(pos, size);
	u8* byte = GetByte(array, pos, size);
	
	u8 mask = 0xFF >> (8 - size);
	mask = mask << bitPos;
	return (*byte & mask) >> bitPos;
}

void SetBits(void *array, u16 pos, u16 value, int size)
{
	u8 bitPos = GetBitPos(pos, size);
	u8* byte = GetByte(array, pos, size);
	
	u8 mask = 0xFF >> (8 - size);
	mask = ~(mask << bitPos);
	*byte = *byte & mask;
	*byte |= (value << bitPos);
}

u8 cpct_getBit(void *array, u16 pos)
{
	return GetBits(array, pos, 1); 
}

u8 cpct_get2Bits(void *array, u16 pos) 
{
	return GetBits(array, pos, 2);
}

u8 cpct_get4Bits(void *array, u16 pos) 
{
	return GetBits(array, pos, 4);
}

u8 cpct_get6Bits(void *array, u16 pos) 
{
	return GetBits(array, pos, 6);
}

void cpct_setBit(void *array, u16 value, u16 pos)
{
	SetBits(array, pos, value, 1);
}

void cpct_set2Bits(void *array, u16 value, u16 pos) 
{
	SetBits(array, pos, value, 2);
}

void cpct_set4Bits(void *array, u16 value, u16 pos) 
{
	SetBits(array, pos, value, 4);
}

void cpct_set6Bits(void *array, u16 value, u16 pos) 
{
	SetBits(array, pos, value, 6);
}
