// ---------------------------------------------------------------------------

#pragma hdrstop

#pragma package(smart_init)

#include <stdio.h>

#include <System.hpp>
#include <Vcl.Clipbrd.hpp>
#include <System.DateUtils.hpp>

#include "UtilsMisc.h"
#include "UtilsStr.h"
#include "UtilsDate.h"
#include "UtilsCPUSpeed.h"

// ---------------------------------------------------------------------------
int MsgBox(String sMessage, UINT iType, String sCaption, HWND hHWND,
	WORD wLanguage) {
	if (sCaption.IsEmpty()) {
		sCaption = Application->Title;
	}

	if (hHWND == NULL) {
		if (Screen->ActiveForm) {
			if (Screen->ActiveForm == Application->MainForm) {
				hHWND = Screen->ActiveForm->Handle;

				for (int i = 0; i < Screen->FormCount; i++) {
					if (Screen->Forms[i]->FormStyle == fsStayOnTop) {
						if (Screen->Forms[i]->Visible) {
							hHWND = Application->Handle;
							break;
						}
					}
				}
			}
			else {
				if (Screen->ActiveForm->FormState.Contains(fsModal)) {
					hHWND = Screen->ActiveForm->Handle;
				}
				else {
					hHWND = Application->Handle;
				}
			}
		}
		else {
			hHWND = Application->Handle;
		}
	}

	return MessageBoxEx(hHWND, sMessage.w_str(), sCaption.w_str(), iType,
		wLanguage);
}

// ---------------------------------------------------------------------------
int MsgBox(NativeUInt Ident, UINT iType, String sCaption, HWND hHWND,
	WORD wLanguage) {
	return MsgBox(LoadStr(Ident), iType, sCaption, hHWND, wLanguage);
}

// ---------------------------------------------------------------------------
void MsgBoxErr(String sMessage, HWND hHWND) {
	MsgBox(sMessage, MB_OK | MB_ICONERROR, "������", hHWND);
}

// ---------------------------------------------------------------------------
void MsgBoxErr(NativeUInt Ident, HWND hHWND) {
	MsgBoxErr(LoadStr(Ident), hHWND);
}

// ---------------------------------------------------------------------------
bool MsgBoxYesNo(String sMessage, bool DefaultNo, HWND hHWND) {
	DWORD uType = MB_YESNO | MB_ICONQUESTION;
	if (DefaultNo) {
		uType = uType | MB_DEFBUTTON2;
	}

	return MsgBox(sMessage, uType, "", hHWND) == ID_YES;
}

// ---------------------------------------------------------------------------
bool MsgBoxYesNo(NativeUInt Ident, bool DefaultNo, HWND hHWND) {
	return MsgBoxYesNo(LoadStr(Ident), DefaultNo, hHWND);
}

// ---------------------------------------------------------------------------
void ShowErrorBox(DWORD Error, String AddStr, HWND hHWND) {
	if (Error == 0) {
		Error = GetLastError();
	}

	if (AddStr.IsEmpty()) {
		AddStr = SysErrorMessage(Error);
	}
	else {
		if (Pos("%s", AddStr) == 0) {
			AddStr = AddStr + SysErrorMessage(Error);
		}
		else {
			AddStr = Format(AddStr, ARRAYOFCONST((SysErrorMessage(Error))));
		}
	}

	MsgBoxErr(AddStr, hHWND);
}

// ---------------------------------------------------------------------------
void ProcMess() {
	Application->ProcessMessages();
}

// ---------------------------------------------------------------------------
bool CheckExit() {
	return Application->Terminated;
}

// ---------------------------------------------------------------------------
void ShowWaitCursor() {
	Screen->Cursor = crHourGlass;
	ProcMess();
}

// ---------------------------------------------------------------------------
void RestoreCursor() {
	Screen->Cursor = crDefault;
}

// ---------------------------------------------------------------------------
void SetCurPosToCenter(TControl *Control) {
	TPoint EndPoint;

	if (Control != NULL) {
		EndPoint = Control->ClientToScreen(Point(Control->Width / 2,
			Control->Height / 2));
	}
	else {
		EndPoint = Point(Screen->Width / 2, Screen->Height / 2);
	}

	SetCursorPos(EndPoint.x, EndPoint.y);
}

// ---------------------------------------------------------------------------
void TrayIconLoadFromResourceName(TTrayIcon * TrayIcon, String ResName) {
	TIcon * TempIcon = new TIcon();
	TempIcon->LoadFromResourceName((int)HInstance, ResName);
	TrayIcon->Icon = TempIcon;
	TempIcon->Free();
}

// ---------------------------------------------------------------------------
String InternalVerQueryValue(LPTSTR lpData, String SubBlock) {
	LPTSTR lpBuffer;
	DWORD dwBytes;

	SubBlock = "\\StringFileInfo\\040904E4\\" + SubBlock;
	VerQueryValue(lpData, SubBlock.w_str(), (LPVOID*) &lpBuffer,
		(PUINT) &dwBytes);
	return lpBuffer;
}

// ---------------------------------------------------------------------------
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

	dwLen = GetFileVersionInfoSize(FileName.w_str(), &dwHandle);
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

			CompanyName = InternalVerQueryValue(lpData, "CompanyName");
			FileDescription = InternalVerQueryValue(lpData, "FileDescription");
			FileVersion = InternalVerQueryValue(lpData, "FileVersion");
			InternalName = InternalVerQueryValue(lpData, "InternalName");
			LegalCopyright = InternalVerQueryValue(lpData, "LegalCopyright");
			OriginalFilename = InternalVerQueryValue(lpData,
				"OriginalFilename");
			ProductName = InternalVerQueryValue(lpData, "ProductName");
			ProductVersion = InternalVerQueryValue(lpData, "ProductVersion");
		}

		free(lpData);
	}

	return Result;
}

// ---------------------------------------------------------------------------
String GetFileVerDate() {
	return FormatDateTime("yyyy.mm.dd",
		UnixToDateTime(((PIMAGE_NT_HEADERS)((DWORD)((PIMAGE_DOS_HEADER)
		HInstance) + (((PIMAGE_DOS_HEADER) HInstance)->e_lfanew)))
		->FileHeader.TimeDateStamp));
}

// ---------------------------------------------------------------------------
String SmallFileVersion(String FileVersion) {
	String S1, S2, S3;

	SplitStr(FileVersion, DOT, 1, S1, S2);

	SplitStr(S2, DOT, 0, S2, S3);

	if (S3 == "0") {
		S3 = "";
		if (S2 == "0") {
			S2 = "";
		}
	}

	return ConcatStrings(S1, ConcatStrings(S2, S3, DOT), DOT);
}

// ---------------------------------------------------------------------------
String GetFileVer(String FileName, bool SmallFormat) {
	TVSFixedFileInfo FileVersionInfo;

	String CompanyName, FileDescription, FileVersion, InternalName,
		LegalCopyright, OriginalFilename, ProductName, ProductVersion;

	String Result = "";

	if (GetFileVerInfo(FileName, FileVersionInfo, CompanyName, FileDescription,
		FileVersion, InternalName, LegalCopyright, OriginalFilename,
		ProductName, ProductVersion)) {
		Result = FileVersion;
		if (SmallFormat) {
			Result = SmallFileVersion(Result);
		}
		if (IsValueInWord(FileVersionInfo.dwFileFlags, VS_FF_DEBUG)) {
			Result = Result + " (debug build)";
		}
	}

	return Result;
}

// ---------------------------------------------------------------------------
String GetFileVer() {
	return GetFileVer(Application->ExeName);
}

// ---------------------------------------------------------------------------
bool IsValueInWord(DWORD AWord, DWORD AValue) {
	return (AWord & AValue);
}

// ---------------------------------------------------------------------------
String GetCPUSpeed() {
	return FormatHerzs(floor((double)_GetCPUSpeed()));
}

// ---------------------------------------------------------------------------
String GetTotalPhys() {
	TMemoryStatusEx MS;

	MS.dwLength = sizeof(TMemoryStatusEx);
	GlobalMemoryStatusEx(&MS);

	return FormatBytes(MS.ullTotalPhys);
}

// ---------------------------------------------------------------------------
void ClipboardSetText(String S) {
	Clipboard()->AsText = S;
}

// ---------------------------------------------------------------------------
String ClipboardGetText() {
	return Clipboard()->AsText;
}

// ---------------------------------------------------------------------------
void Delay(DWORD mSecs) {
	DWORD FirstTick = GetTickCount();
	do {
		ProcMess();
	}
	while ((GetTickCount() - FirstTick) < mSecs);
}

// ---------------------------------------------------------------------------
double Round(double Number) {
	return Number < 0.0 ? ceil(Number - 0.5) : floor(Number + 0.5);
}

// ---------------------------------------------------------------------------
ULONGLONG StartTimer() {
	return GetTickCount64();
}

// ---------------------------------------------------------------------------
String StopTimer(ULONGLONG FirstTick, bool FormatMSec) {
	ULONGLONG ElapsedTime = GetTickCount64() - FirstTick;
	return FormatMSec ? MillisecondsToDateTimeString(ElapsedTime) :
		FormatFloat("0,# msec.", ElapsedTime);
}

// ---------------------------------------------------------------------------
bool IsWinNT() {
	return Win32Platform == VER_PLATFORM_WIN32_NT;
}

// ---------------------------------------------------------------------------
bool IsWinVistaOrGreat() {
	return (Win32MajorVersion >= 6) && (Win32MinorVersion >= 0);
}

// ---------------------------------------------------------------------------
float Percent(float Number, float Max) {
	if (Number == 0.0 || Max == 0.0) {
		return 0;
	}
	return (Number / Max) * 100.0;
}

// ---------------------------------------------------------------------------
int Percent(int Number, int Max) {
	return (int)Percent((float)Number, (float)Max);
}

// ---------------------------------------------------------------------------
bool Odd(int I) {
	return I & 1;
}

// ---------------------------------------------------------------------------
HWND FindWindowByClass(String ClassName) {
	return FindWindow(ClassName.w_str(), NULL);
}

// ---------------------------------------------------------------------------
String GetClassName(HWND hWnd) {
	wchar_t buf[256];
	String Result;
	if (GetClassName(hWnd, buf, sizeof(buf))) {
		Result = String(buf);
	}
	return Result;
}

// ---------------------------------------------------------------------------
String GetControlText(HWND hWnd) {
	wchar_t buf[256];
	SendMessage(hWnd, WM_GETTEXT, sizeof(buf), (LPARAM)(LPCTSTR)buf);
	return String(buf);
}

// ---------------------------------------------------------------------------
HWND GetWindowChildByIndex(HWND hWnd, int Index) {
	int ChildIndex = 0;

	HWND Result = 0;

	HWND hChild = GetWindow(hWnd, GW_CHILD);

	while (hChild) {
		if (ChildIndex == Index) {
			return hChild;
		}

		hChild = GetWindow(hChild, GW_HWNDNEXT);

		ChildIndex++;
	}

	return Result;
}

// ---------------------------------------------------------------------------
void DebugEnumWindowControls(HWND hWnd, TStringList * Result, int Level) {
	int Index = 0;

	HWND hChild = GetWindow(hWnd, GW_CHILD);

	String LevelStr;
	String ClassName;
	String ControlText;

	Level++;

	for (int i = 0; i < Level; i++) {
		LevelStr = LevelStr + "-";
	}

	while (hChild) {
		ClassName = GetClassName(hChild);

		ControlText = GetControlText(hChild);

		Result->Add(LevelStr + "|" + IntToStr(Index) + ":" + ClassName + ":" +
			ControlText + ":" + BoolToStr(IsWindowEnabled(hChild), true));

		DebugEnumWindowControls(hChild, Result, Level);

		hChild = GetWindow(hChild, GW_HWNDNEXT);

		Index++;
	}
}

// ---------------------------------------------------------------------------
int GetRusControlNumberInternal(String Value, int x) {
	int Sum = 0;

	for (int i = 1; i <= Value.Length(); i++) {
		Sum += StrToInt(Value[i]) * x;

		x++;
	}

	return Sum % 11;
}

// ---------------------------------------------------------------------------
int GetRusControlNumber(int Value, int ValueDigits) {
	if (Value <= 0) {
		return 0;
	}

	String S = IToS_0(Value, ValueDigits);

	int Result = GetRusControlNumberInternal(S, 1);

	if (Result == 10) {
		Result = GetRusControlNumberInternal(S, 3);
	}

	if (Result == 10) {
		Result = 0;
	}

	return Result;
}

// ---------------------------------------------------------------------------
