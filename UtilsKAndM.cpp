// ---------------------------------------------------------------------------

#pragma hdrstop

#include "UtilsKAndM.h"

#pragma package(smart_init)

// ---------------------------------------------------------------------------
bool IsKey(int Key) {
	return GetAsyncKeyState(Key) < 0;
}

bool IsShift() {
	return GetAsyncKeyState(VK_SHIFT) < 0;
}

bool IsCtrl() {
	return GetAsyncKeyState(VK_CONTROL) < 0;
}

bool IsAlt() {
	return GetAsyncKeyState(VK_MENU) < 0;
}

TPoint GetCurPos() {
	TPoint Result;
	// GetCursorPos(Result);
	return Result;
}
