void LZ48Decrunch(const u8* source, u8* target) __z88dk_callee 
{
source;
target;
	__asm
		pop af
		pop hl
		pop de

		push af
		push ix

		call _LZ48_decrunch

		pop ix
	__endasm;
}