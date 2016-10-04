#include "winCpctelera.h"

static u16 _seed;

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