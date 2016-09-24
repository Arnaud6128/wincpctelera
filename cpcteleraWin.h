#include <windows.h>
#include <cpctelera.h>

#define WINDOW_STYLE	WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE
#define NB_PAL_COLOR	27
#define NB_COLORS		16
#define BORDER_COLOR	16
#define TITLE			"WinCPCTelera"	

typedef void(*TInterrupt)(void);

typedef struct tagSAmstrad
{
	u8 _mode;
	u8 _currentPage;
	u8 _internalTimer;
	TInterrupt _interruptFunction;

	HBITMAP _video[4];

	UCHAR _curPal[NB_COLORS + 1];
	UCHAR _memVideo[200][82];
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
extern u8 _curVideo;

void CPCTeleraWin();
void MsgLoop();

u8 GetCoordY(u8* memory);
void FillBorder();
void DrawSprite(void *sprite, int x, int y, int cx, int cy, BOOL pMasked);
void FillBox(HDC hdc, u8 pattern, int x, int y, int cx, int cy);
BOOL IsVideoMem(void* pAddress);
void SetCurVideo(int pVideoAddr);
void DrawFont(int x, int y, u8 fgPen, u8 bgPen, char chara);
HBITMAP GetVideoBitmap(int pScreenAddr);
HBITMAP GetCurVideoBuff();
u16 GetVKey(u16 pCpcKeyID);
void CreatePaletteCpc();
u8 M0byte2px(u8 pPix);