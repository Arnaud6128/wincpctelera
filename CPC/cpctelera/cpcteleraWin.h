#include <windows.h>
#include <cpctelera.h>

#define WINDOW_STYLE		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE
#define NB_PAL_COLOR		27
#define NB_COLORS			16
#define BORDER_COLOR		16
#define TITLE				"WinCPCTelera"	

#define FULL_SCREEN_CX		(BORDER_CX + WIDTH_SCREEN + BORDER_CX)
#define FULL_SCREEN_CY		(BORDER_UP_CY + HEIGHT_SCREEN + BORDER_DW_CY)

#define BORDER_CX			64
#define BORDER_UP_CY		96
#define BORDER_DW_CY		80
#define WIDTH_SCREEN		640
#define HEIGHT_SCREEN		400

#define CPC_SCR_CX_BYTES	80
#define CPC_SCR_CY_LINE		200
#define CPC_MEM_LIMIT		0xFFFF

#define MODE_0				0
#define MODE_1				1
#define MODE_2				2

typedef void(*TInterrupt)(void);

typedef struct tagSAmstrad
{
	u8 _mode;
	u8 _currentPage;
	u8 _internalTimer;
	TInterrupt _interruptFunction;

	UCHAR _curPal[NB_COLORS + 1];
	UCHAR _memVideo[4][0x4000];

	UCHAR _mode1Video[0x8000];

} SAmstrad;

typedef struct tagSCPCPalette
{
	UCHAR hw;
	COLORREF rgb;
} SCPCPalette;

extern const SCPCPalette _palette[NB_PAL_COLOR];
extern HWND _hWnd;
extern BOOL _curKey;
extern SAmstrad _amstrad;
extern u8 cpct_keyboardStatusBuffer[10];

void CPCTeleraWin();
void MsgLoop();

void ScanKeyboard();
void FillBorder(HDC pHdc);
void DrawString(void* string, void* video_memory, u8 fg_pen, u8 bg_pen, int pMode);
void DrawSprite(void *sprite, void *memory, int cx, int cy, BOOL pMasked);
BOOL IsCpcMem(const void* pAddress);
void DisplayFontM0(u8* pVideo, u8 fgPen, u8 bgPen, char pChara);
void DisplayFontM1(u8* pVideo, u8 fgPen, u8 bgPen, char pChara);
void DisplayFontM2(u8* pVideo, u8 pPen, char pChara);
u8* GetVideoBufferFromAddress(int pScreenAddr);
u16 GetVKey(u16 pCpcKeyID);
void CreatePaletteCpc();
u8 M0byte2px(u8 pPix);