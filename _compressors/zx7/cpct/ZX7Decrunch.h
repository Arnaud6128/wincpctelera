void ZX7Decrunch(const u8* source, u8* target) __z88dk_callee 
{
source;
target;
	__asm
		pop af
		pop hl
		pop de

		push af
		push ix

		call _ZX7_decrunch

		pop ix
	__endasm;
}