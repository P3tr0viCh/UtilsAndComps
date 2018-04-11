// ---------------------------------------------------------------------------

#pragma hdrstop

#pragma package(smart_init)

#include <System.hpp>
#include <stdio.h>
#include <Vcl.Clipbrd.hpp>
#include <System.DateUtils.hpp>

#include "UtilsMisc.h"
#include "UtilsStr.h"
#include "UtilsDate.h"
#include "UtilsCPUSpeed.h"

// ---------------------------------------------------------------------------
int MsgBox(String sMessage, UINT iType, String sCaption, HWND hHWND,
	WORD wLanguage) {
	if (IsEmpty(sCaption)) {
		sCaption = Application->Title;
	}

	if (hHWND == NULL) {
		if (Screen->ActiveForm) {
			if ((Screen->ActiveForm == Application->MainForm) ||
				(Screen->ActiveForm->FormState.Contains(fsModal))) {
				hHWND = Screen->ActiveForm->Handle;
			}
			else {
				hHWND = Application->Handle;
			}
		}
		else {
			hHWND = Application->Handle;
		}
	}

	return MessageBoxEx(hHWND, sMessage.w_str(), sCaption.w_str(), iType,
		wLanguage);
}

int MsgBox(int iValue, UINT iType, String sCaption, HWND hHWND, WORD wLanguage)
{
	return MsgBox(IntToStr(iValue), iType, sCaption, hHWND, wLanguage);
}

int MsgBox(Extended fValue, UINT iType, String sCaption, HWND hHWND,
	WORD wLanguage) {
	return MsgBox(FormatFloat(",0.00", fValue), iType, sCaption, hHWND,
		wLanguage);
}

int MsgBox(bool bValue, UINT iType, String sCaption, HWND hHWND, WORD wLanguage)
{
	return MsgBox(BoolToStr(bValue, true), iType, sCaption, hHWND, wLanguage);
}

void MsgBoxErr(String sMessage, HWND hHWND) {
	MsgBox(sMessage, MB_OK | MB_ICONERROR, "Îøèáêà", hHWND);
}

bool MsgBoxYesNo(String sMessage, bool DefaultNo, HWND hHWND) {
	DWORD uType = MB_YESNO | MB_ICONQUESTION;

	if (IsEmpty(sMessage)) {
		sMessage = "To be or not to be?";
	}

	if (DefaultNo) {
		uType = uType | MB_DEFBUTTON2;
	}

	return MsgBox(sMessage, uType, NULL, hHWND) == ID_YES;
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
	else {
		EndPoint = Point(Screen->Width / 2, Screen->Height / 2);
	}

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

String GetFileVerDate() {
	return FormatDateTime("yyyy.mm.dd",
		UnixToDateTime(((PIMAGE_NT_HEADERS)((DWORD)((PIMAGE_DOS_HEADER)
		HInstance) + (((PIMAGE_DOS_HEADER) HInstance)->e_lfanew)))
		->FileHeader.TimeDateStamp));
}

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

	return S1 + S2 + S3;
}

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
			Result = Result + " (Debug build)";
		}
	}

	return Result;
}

bool IsValueInWord(DWORD AWord, DWORD AValue) {
	return (AWord & AValue);
}

String GetCPUSpeed() {
	return FormatHerzs(floor(_GetCPUSpeed()));
}

String GetTotalPhys() {
	TMemoryStatusEx MS;

	MS.dwLength = sizeof(TMemoryStatusEx);
	GlobalMemoryStatusEx(&MS);

	return FormatBytes(MS.ullTotalPhys);
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
	if (Error == 0) {
		Error = GetLastError();
	}

	if (AddStr == NULL) {
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

double Round(double Number) {
	return Number < 0.0 ? ceil(Number - 0.5) : floor(Number + 0.5);
}

DWORD StartTimer() {
	return GetTickCount();
}

String StopTimer(DWORD FirstTick, bool FormatMSec) {
	DWORD ElapsedTime = GetTickCount() - FirstTick;
	return FormatMSec ? MyFormatTime(ExtractHMSFromMS(ElapsedTime), true) :
		FormatFloat("0,# msec.", ElapsedTime);
}

bool IsWinVistaOrGreat() {
	return (Win32MajorVersion >= 6) && (Win32MinorVersion >= 0);
}

float Percent(float Number, float Max) {
	if (Number == 0.0 || Max == 0.0) {
		return 0;
	}
	return (Number / Max) * 100.0;
}

int Percent(int Number, int Max) {
	return (int)Percent((float)Number, (float)Max);
}

bool Odd(int I) {
	return I & 1;
}
