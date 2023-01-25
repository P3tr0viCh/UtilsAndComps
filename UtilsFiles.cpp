// ---------------------------------------------------------------------------

#pragma hdrstop

#pragma package(smart_init)

#include <System.Classes.hpp>

#include "UtilsMisc.h"

#include "UtilsFiles.h"

// ---------------------------------------------------------------------------
bool OKExec(const String FileName, const String Params) {
	HINSTANCE hInstance = ShellExecute(Application->Handle, NULL,
		FileName.w_str(), Params.w_str(), NULL, SW_SHOWNORMAL);

	if ((INT_PTR)hInstance > 32) {
		return true;
	}

	ShowErrorBox(GetLastError());

	return false;
}

// ---------------------------------------------------------------------------
void ShellExec(const String FileName, const String Params) {
	if (FileName.IsEmpty()) {
		throw EFileNotFoundException("filename empty");
	}

	HINSTANCE hInstance = ShellExecute(Application->Handle, NULL,
		FileName.w_str(), Params.w_str(), NULL, SW_SHOWNORMAL);

	if ((INT_PTR) hInstance > 32) {
		return;
	}

	if ((INT_PTR) hInstance == ERROR_FILE_NOT_FOUND) {
		throw EFileNotFoundException("file not found");
	}

	throw Exception("shell exec error:" + IntToStr((int)GetLastError()));
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
