// ---------------------------------------------------------------------------

#ifndef UtilsMiscH
#define UtilsMiscH
// ---------------------------------------------------------------------------

#include <vcl.h>

int MsgBox(UnicodeString sMessage = "Groovy!!", UINT iType =
	MB_OK | MB_ICONINFORMATION, UnicodeString sCaption = NULL, HWND hHWND =
	NULL, WORD wLanguage = 0x00400);
int MsgBox(int iValue, UINT iType = MB_OK | MB_ICONINFORMATION,
	UnicodeString sCaption = NULL, HWND hHWND = NULL, WORD wLanguage = 0x00400);
int MsgBox(Extended fValue, UINT iType = MB_OK | MB_ICONINFORMATION,
	UnicodeString sCaption = NULL, HWND hHWND = NULL, WORD wLanguage = 0x00400);
int MsgBox(bool bValue, UINT iType = MB_OK | MB_ICONINFORMATION,
	UnicodeString sCaption = NULL, HWND hHWND = NULL, WORD wLanguage = 0x00400);
void MsgBoxErr(UnicodeString sMessage = "Error !?!", HWND hHWND = NULL);

void ProcMess();

void ShowWaitCursor();
void RestoreCursor();
void SetCurPosToCenter(TControl *Control);

bool GetFileVerInfo(String FileName, TVSFixedFileInfo &FileVersionInfo,
	String &CompanyName, String &FileDescription, String &FileVersion,
	String &InternalName, String &LegalCopyright, String &OriginalFilename,
	String &ProductName, String &ProductVersion);
String GetFileVer(String FileName, bool SmallFormat = true);

bool IsValueInWord(DWORD AWord, DWORD AValue);

String GetCPUSpeed();
String GetTotalPhys();

void CopyToClipBoard(String S);

void Delay(DWORD mSecs);

bool IsWinNT();

void ShowErrorBox(DWORD Error = 0, String AddStr = NULL, HWND hHWND = NULL);

double Round(double Number);

DWORD StartTimer();
String StopTimer(DWORD FirstTick, bool FormatMSec = true);
#endif
