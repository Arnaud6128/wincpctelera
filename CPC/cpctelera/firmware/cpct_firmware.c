#include <cpcteleraWin.h>

void cpct_setInterruptHandler(void(*intHandler)(void))
{
	_amstrad._interruptFunction = intHandler;
}

void cpct_reenableFirmware(u16 firmware_ROM_code)
{
	MsgLoop();
}

u16 cpct_disableFirmware()
{
	srand(GetTickCount());
	CPCTeleraWin();
	return 0;
}
