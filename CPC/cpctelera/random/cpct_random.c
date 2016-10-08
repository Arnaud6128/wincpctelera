#include "winCpctelera.h"

static u32 cpct_mxor32_seed;

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

u8 cpct_getRandom_glfsr16_u8()
{
	return (u8)rand();
}

u16 cpct_getRandom_glfsr16_u16()
{
	return (u16)rand();
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