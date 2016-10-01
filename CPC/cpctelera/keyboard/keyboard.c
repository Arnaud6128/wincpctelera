#include <cpcteleraWin.h>

u8 cpct_keyboardStatusBuffer[10];

void cpct_scanKeyboard_f()
{
	ScanKeyboard();
}

void cpct_scanKeyboard_if()
{
	ScanKeyboard();
}

void cpct_scanKeyboard()
{
	ScanKeyboard();
}

u8 cpct_isKeyPressed(cpct_keyID key)
{
	MsgLoop();
	return GetAsyncKeyState(GetVKey(key)) != 0 ? TRUE : FALSE;
}

u8 cpct_isAnyKeyPressed()
{
	MsgLoop();
	BOOL isKeyPressed = (_curKey != FALSE);
	return isKeyPressed;
}

u8 cpct_isAnyKeyPressed_f()
{
	return cpct_isAnyKeyPressed();
}