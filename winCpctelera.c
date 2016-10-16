#include "winCpctelera.h"

#ifdef const
	#undef const
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern void CreateWindowApp();
void StartCPC();

SAmstrad _amstrad;

void CPCTeleraWin()
{
	static BOOL _isStarted = FALSE;
	if (!_isStarted)
	{
		_isStarted = TRUE;

		StartCPC();
		CreateWindowApp();
	}
}

void StartCPC()
{
	ZeroMemory(&_amstrad, sizeof(_amstrad));
	for (int i = 0; i < NB_COLORS; i++)
		_amstrad._curPal[i] = _palette[i].hw;

	memset(&_amstrad._memCPC, 0, CPC_MEM_SIZE);

	_amstrad._mode = 1;
	_amstrad._curPal[0] = HW_BRIGHT_BLUE;
	_amstrad._curPal[1] = HW_BRIGHT_YELLOW;
	_amstrad._curPal[BORDER_COLOR] = HW_BRIGHT_BLUE;

	_amstrad._currentPage = cpct_pageC0;

	StartInterrupt();
}