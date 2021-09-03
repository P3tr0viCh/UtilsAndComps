// ---------------------------------------------------------------------------

#ifndef UtilsMiscH
#define UtilsMiscH

#include <vcl.h>

// ---------------------------------------------------------------------------
int MsgBox(String sMessage = "TODO", UINT iType = MB_OK | MB_ICONINFORMATION,
	String sCaption = NULL, HWND hHWND = NULL, WORD wLanguage = 0x00400);
int MsgBox(NativeUInt Ident, UINT iType = MB_OK | MB_ICONINFORMATION,
	UnicodeString sCaption = NULL, HWND hHWND = NULL, WORD wLanguage = 0x00400);
void MsgBoxErr(String sMessage = "ERROR", HWND hHWND = NULL);
void MsgBoxErr(NativeUInt Ident, HWND hHWND = NULL);
bool MsgBoxYesNo(String sMessage = "To be or not to be?", bool DefaultNo = true,
	HWND hHWND = NULL);
bool MsgBoxYesNo(NativeUInt Ident, bool DefaultNo = true,
	HWND hHWND = NULL);

// ---------------------------------------------------------------------------
void ProcMess();
bool CheckExit();

// ---------------------------------------------------------------------------
void ShowWaitCursor();
void RestoreCursor();
void SetCurPosToCenter(TControl * Control);
void TrayIconLoadFromResourceName(TTrayIcon * TrayIcon, String ResName);

// ---------------------------------------------------------------------------
bool GetFileVerInfo(String FileName, TVSFixedFileInfo &FileVersionInfo,
	String &CompanyName, String &FileDescription, String &FileVersion,
	String &InternalName, String &LegalCopyright, String &OriginalFilename,
	String &ProductName, String &ProductVersion);
String GetFileVer(String FileName, bool SmallFormat = true);
String GetFileVer();
String SmallFileVersion(String FileVersion);
String GetFileVerDate();

// ---------------------------------------------------------------------------
bool IsValueInWord(DWORD AWord, DWORD AValue);

// ---------------------------------------------------------------------------
String GetCPUSpeed();
String GetTotalPhys();

// ---------------------------------------------------------------------------
void CopyToClipBoard(String S);

// ---------------------------------------------------------------------------
void Delay(DWORD mSecs);

// ---------------------------------------------------------------------------
void ShowErrorBox(DWORD Error = 0, String AddStr = NULL, HWND hHWND = NULL);

// ---------------------------------------------------------------------------
double Round(double Number);

// ---------------------------------------------------------------------------
ULONGLONG StartTimer();
String StopTimer(ULONGLONG FirstTick, bool FormatMSec = true);

// ---------------------------------------------------------------------------
bool IsWinNT();
bool IsWinVistaOrGreat();

// ---------------------------------------------------------------------------
float Percent(float Number, float Max);
int Percent(int Number, int Max);

// ---------------------------------------------------------------------------
bool Odd(int I);

// ---------------------------------------------------------------------------
#endif
