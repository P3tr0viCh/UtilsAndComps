// ---------------------------------------------------------------------------

#pragma hdrstop

#pragma package(smart_init)

#include <windows.h>
#include <psapi.h>
#include <System.Classes.hpp>

#pragma comment(lib,"C:/Program Files/Borland/CBuilder6/Lib/Psdk/psapi.lib")

#include "UtilsMisc.h"
#include "UtilsFiles.h"

#include "UtilsFilesStr.h"

// ---------------------------------------------------------------------------
void CreateWin9xProcessList(TStringList *List) {
	throw(Exception("TODO: CreateWin9xProcessList"));

	/* hSnapShot: THandle;
	 ProcInfo: TProcessEntry32;

	 hSnapShot := CreateToolHelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	 if (hSnapShot <> THandle(-1)) then
	 begin
	 ProcInfo.dwSize := SizeOf(ProcInfo);
	 if (Process32First(hSnapshot, ProcInfo)) then
	 begin
	 List.Add(ProcInfo.szExeFile);
	 while (Process32Next(hSnapShot, ProcInfo)) do
	 List.Add(ProcInfo.szExeFile);
	 end;
	 CloseHandle(hSnapShot);
	 end; */
}

// ---------------------------------------------------------------------------
void CreateWinNTProcessList(TStringList *List) {
	DWORD aProcesses[1024];
	DWORD cbNeeded;
	DWORD cProcesses;

	EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded);

	cProcesses = cbNeeded / sizeof(DWORD);

	for (DWORD i = 0; i < cProcesses; i++) {
		if (aProcesses[i] != 0) {
			HANDLE hProcess =
				OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false,
				aProcesses[i]);
			if (hProcess != NULL) {
				HMODULE hMod;

				if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
					&cbNeeded)) {
					// for (DWORD y = 0; y < (cbNeeded / sizeof(HMODULE)); y++)
					// {
					TCHAR szProcessName[MAX_PATH];

					if (GetModuleFileNameEx(hProcess, hMod, szProcessName,
						sizeof(szProcessName) / sizeof(TCHAR)))

						List->Add(szProcessName);
					// }
				}

				CloseHandle(hProcess);
			}
		}
	}
	// List->SaveToFile("Processes.txt");
}

// ---------------------------------------------------------------------------
void GetProcessList(TStringList *List) {
	if (IsWinNT())
		CreateWinNTProcessList(List);
	else
		CreateWin9xProcessList(List);
}

// ---------------------------------------------------------------------------
bool EXEIsRunning(String FileName, bool FullPath) {
	String S;
	TStringList *ProcessList = new TStringList();

	bool Result = false;

	GetProcessList(ProcessList);

	for (int i = 0; i < ProcessList->Count; i++) {
		if (FullPath)
			S = ProcessList[0][i];
		else
			S = ExtractFileName(ProcessList[0][i]);

		Result = AnsiCompareText(S, FileName) == 0;

		if (Result)
			break;
	}

	ProcessList->Free();

	return Result;
}

// ---------------------------------------------------------------------------
bool OKExec(const int Inst, String FileName) {
	bool Result = Inst > 32;

	if (!Result) {
		if (Inst == ERROR_ACCESS_DENIED)
			return Result;

		SetForegroundWindow(Application->Handle);

		if ((Inst == ERROR_FILE_NOT_FOUND) && (FileName != NULL))
			MsgBoxErr(Format(LoadStr(IDS_ERROR_FILE_NOT_FOUND),
			ARRAYOFCONST((FileName))));
		else
			ShowErrorBox(GetLastError());
	}

	return Result;
}

// ---------------------------------------------------------------------------
bool ShellExec(const String FileName) {
	return OKExec((int) ShellExecEx(FileName, ""), FileName);
}

// ---------------------------------------------------------------------------
HINSTANCE ShellExecEx(const String FileName, const String Params) {
	return ShellExecute(Application->Handle, NULL, FileName.w_str(),
		Params.w_str(), NULL, SW_SHOWNORMAL);
}

// ---------------------------------------------------------------------------
String FileInAppDir(String FileName) {
	return IncludeTrailingPathDelimiter(ExtractFilePath(Application->ExeName)) +
		FileName;
}

// ---------------------------------------------------------------------------
String FileWithoutExt(String FileName) {
	if (Pos('.', FileName) == 0)
		return FileName;

	int PosDot = FileName.Length();

	while ((FileName[PosDot] != '.') & (PosDot > 1)) {
		PosDot--;
	}

	return FileName.SubString(1, PosDot - 1);
}

// ---------------------------------------------------------------------------
String OnlyFileName(String FileName) {
	return FileWithoutExt(ExtractFileName(FileName));
}

// ---------------------------------------------------------------------------
String SlashSep(String Path, String S) {
	return IncludeTrailingPathDelimiter(Path) + S;
}

// ---------------------------------------------------------------------------
String GetSpecialFolderPath(int Csidl) {
	TCHAR pszPath[MAX_PATH];

	if (SHGetSpecialFolderPath(NULL, pszPath, Csidl, false)) {
		return pszPath;
	}
	else {
		throw EPathNotFoundException("special folder fail: " + SysErrorMessage
			(GetLastError()));
	}
}

// ---------------------------------------------------------------------------
String GetTempFolderPath() {
	TCHAR pszPath[MAX_PATH];

	if (GetTempPath(MAX_PATH, pszPath) == 0) {
		throw EPathNotFoundException("temp folder fail: " + SysErrorMessage
			(GetLastError()));
	}

	return pszPath;
}

// ---------------------------------------------------------------------------
String GetTempFileNamePath(String Prefix) {
	TCHAR pszPath[MAX_PATH];

	if (GetTempFileName(GetTempFolderPath().c_str(), Prefix.c_str(), 0,
		pszPath) == 0) {
		throw EFileNotFoundException("temp file fail: " + SysErrorMessage
			(GetLastError()));
	}

	return pszPath;
}

// ---------------------------------------------------------------------------
