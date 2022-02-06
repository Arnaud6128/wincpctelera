/** Functions to keep code compatibility with CPCTelera and debug */
/** Include to your project */

#include <stdio.h>

#ifdef WINCPCTELERA
	u8* wincpct_getMemory(u8* ptr);
	void wincpct_wait(int ms);
	void wincpct_msgLoop();
	void wincpct_CPCTeleraWin();

	#define	WinCpcTelera_Start()		wincpct_CPCTeleraWin()
	#define WinCpcTelera_GetMemPtr(X)	(u8*)wincpct_getMemory((u8*)X)
	#define WinCpcTelera_WaitKey()		{ while(!cpct_isAnyKeyPressed()){ wincpct_wait(10); wincpct_msgLoop(); }; }
	#define WinCpcTelera_Process()		wincpct_msgLoop()
	#define WinCpcTelera_Wait(X)		wincpct_wait(X)
	#define WinCpcTelera_Stop()			{ while(1){ wincpct_wait(10); wincpct_msgLoop(); }; }
	#define WinCpcTelera_PrintStr(N,X)	printf("%s : %s\n", N, X)
	#define WinCpcTelera_PrintInt(N,X)	printf("%s : %d\n", N, X)
	#define WinCpcTelera_PrintHex(N,X)	printf("%s : 0x%04X\n", N, X)

#else

	#define	WinCpcTelera_Start()
	#define WinCpcTelera_GetMemPtr(X)	(u8*)(X)
	#define WinCpcTelera_WaitKey()
	#define WinCpcTelera_Process()
	#define WinCpcTelera_Wait(X)	
	#define WinCpcTelera_Stop()
	#define WinCpcTelera_PrintStr(N,X)
	#define WinCpcTelera_PrintInt(N,X)
	#define WinCpcTelera_PrintHex(N,X)
#endif // WINCPCTELERA

