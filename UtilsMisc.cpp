// ---------------------------------------------------------------------------

#pragma hdrstop

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#include <System.hpp>
#include <stdio.h>
#include <Vcl.Clipbrd.hpp>

#include "UtilsMisc.h"
#include "UtilsStr.h"

int MsgBox(UnicodeString sMessage, UINT iType, UnicodeString sCaption,
	HWND hHWND, WORD wLanguage) {
	if (sCaption == NULL)
		sCaption = Application->Title;
	if (hHWND == NULL) {
		if (Screen->ActiveForm) {
			if ((Screen->ActiveForm == Application->MainForm) |
				(Screen->ActiveForm->FormState.Contains(fsModal)))
				hHWND = Screen->ActiveForm->Handle;
			else
				hHWND = Application->Handle;
		}
		else
			hHWND = Application->Handle;
	}

	return MessageBoxEx(hHWND, sMessage.w_str(), sCaption.w_str(), iType,
		wLanguage);
}

int MsgBox(int iValue, UINT iType, UnicodeString sCaption, HWND hHWND,
	WORD wLanguage) {
	return MsgBox(IntToStr(iValue), iType, sCaption, hHWND, wLanguage);
}

int MsgBox(Extended fValue, UINT iType, UnicodeString sCaption, HWND hHWND,
	WORD wLanguage) {
	return MsgBox(FormatFloat(",0.00", fValue), iType, sCaption, hHWND,
		wLanguage);
}

int MsgBox(bool bValue, UINT iType, UnicodeString sCaption, HWND hHWND,
	WORD wLanguage) {
	return MsgBox(BoolToStr(bValue, true), iType, sCaption, hHWND, wLanguage);
}

void MsgBoxErr(UnicodeString sMessage, HWND hHWND) {
	MsgBox(sMessage, MB_OK | MB_ICONERROR, "Îøèáêà", hHWND);
}

void ProcMess() {
	Application->ProcessMessages();
}

void ShowWaitCursor() {
	Screen->Cursor = crHourGlass;
	ProcMess();
}

void RestoreCursor() {
	Screen->Cursor = crDefault;
}

void SetCurPosToCenter(TControl *Control) {
	TPoint EndPoint;

	if (Control != NULL) {
		EndPoint = Control->ClientToScreen(Point(Control->Width / 2,
			Control->Height / 2));
	}
	else
		EndPoint = Point(Screen->Width / 2, Screen->Height / 2);

	SetCursorPos(EndPoint.x, EndPoint.y);
}

String InternalVerQueryValue(LPTSTR lpData, String SubBlock) {
	LPTSTR lpBuffer;
	DWORD dwBytes;

	SubBlock = "\\StringFileInfo\\040904E4\\" + SubBlock;
	VerQueryValue(lpData, SubBlock.w_str(), (LPVOID*) &lpBuffer,
		(PUINT) &dwBytes);
	return lpBuffer;
}

bool GetFileVerInfo(String FileName, TVSFixedFileInfo &FileVersionInfo,
	String &CompanyName, String &FileDescription, String &FileVersion,
	String &InternalName, String &LegalCopyright, String &OriginalFilename,
	String &ProductName, String &ProductVersion) {
	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	}*lpTranslate;

	DWORD dwLen;
	DWORD dwHandle;
	LPTSTR lpData;

	VS_FIXEDFILEINFO *pFileInfo;
	DWORD dwBytes;

	bool Result;

	dwLen  = GetFileVersionInfoSize(FileName.w_str(), &dwHandle);
	Result = dwLen != 0;
	if (Result) {
		lpData = (LPTSTR) malloc(dwLen);

		Result = GetFileVersionInfo(FileName.w_str(), dwHandle, dwLen, lpData);
		if (Result) {
			// Result = VerQueryValue(lpData, TEXT("\VarFileInfo\Translation"),
			// (LPVOID*) &lpTranslate, (PUINT) &cbTranslate);
			/*
			 if Result then
			 InfoPath := '\StringFileInfo\'+
			 IntToHex(Translation^.wLanguage,4) + IntToHex(Translation^.wCodePage,4) + '\'
			 else
			 InfoPath := '\StringFileInfo\040904E4\'; */

			VerQueryValue(lpData, TEXT("\\"), (LPVOID *) &pFileInfo,
				(PUINT) &dwBytes);

			FileVersionInfo = *pFileInfo;

			CompanyName     = InternalVerQueryValue(lpData, "CompanyName");
			FileDescription = InternalVerQueryValue(lpData, "FileDescription");
			FileVersion     = InternalVerQueryValue(lpData, "FileVersion");
			InternalName    = InternalVerQueryValue(lpData, "InternalName");
			LegalCopyright  = InternalVerQueryValue(lpData, "LegalCopyright");
			OriginalFilename = InternalVerQueryValue(lpData,
				"OriginalFilename");
			ProductName    = InternalVerQueryValue(lpData, "ProductName");
			ProductVersion = InternalVerQueryValue(lpData, "ProductVersion");
		}

		free(lpData);
	}

	return Result;
}

bool IsValueInWord(DWORD AWord, DWORD AValue) {
	return (AWord & AValue);
}

String GetCPUSpeed() {
	const DelayTime = 500;

	DWORD TimerHi, TimerLo;

	int PriorityClass = GetPriorityClass(GetCurrentProcess());
	int Priority      = GetThreadPriority(GetCurrentThread());
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

	Sleep(10);
	asm {
		dw 310Fh
		mov TimerLo, eax
		mov TimerHi, edx
	}

	Sleep(DelayTime);

	asm {
		dw 310Fh
		sub eax, TimerLo
		sbb edx, TimerHi
		mov TimerLo, eax
		mov TimerHi, edx
	}

	SetThreadPriority(GetCurrentThread(), Priority);
	SetPriorityClass(GetCurrentProcess(), PriorityClass);

	return FormatHerzs(floor(1000.0 * TimerLo / (DelayTime)));
}

String GetTotalPhys() {
	TMemoryStatus MS;

	MS.dwLength = sizeof(TMemoryStatus);
	GlobalMemoryStatus(&MS);
	return FormatBytes(MS.dwTotalPhys);
}

void CopyToClipBoard(String S) {
	Clipboard()->AsText = S;
}

void Delay(DWORD mSecs) {
	DWORD FirstTick = GetTickCount();
	do {
		ProcMess();
	}
	while ((GetTickCount() - FirstTick) < mSecs);
}

bool IsWinNT() {
	return Win32Platform == VER_PLATFORM_WIN32_NT;
}

void ShowErrorBox(DWORD Error, String AddStr, HWND hHWND) {
	if (Error == 0)
		Error = GetLastError();

	if (AddStr == NULL)
		AddStr = SysErrorMessage(Error);
	else if (Pos("%s", AddStr) == 0)
		AddStr = AddStr + SysErrorMessage(Error);
	else
		AddStr = Format(AddStr, ARRAYOFCONST((SysErrorMessage(Error))));

	MsgBoxErr(AddStr, hHWND);
}

double Round(double Number) {
	return Number < 0.0 ? ceil(Number - 0.5) : floor(Number + 0.5);
}
