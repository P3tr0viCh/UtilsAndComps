// ---------------------------------------------------------------------------

#pragma hdrstop

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#include <vcl.h>
#include <System.SysUtils.hpp>
#include <System.StrUtils.hpp>

#include "UtilsLog.h"

#include "UtilsStr.h"
#include "UtilsMisc.h"
#include "UtilsFiles.h"

#include "UtilsLogStr.h"

// ---------------------------------------------------------------------------
const MaxLogSize = 1024 * 1024; // 1 Mb

// ---------------------------------------------------------------------------
void WriteToLog(String S) {
	HANDLE LogFile;
	String LogPath;
	String LogFileName;
	DWORD dwSize;
	AnsiString SS;
	TDateTime DateTime;
	LPDWORD NumberOfBytesWritten = new DWORD;

	try { // except
		DateTime = Now();

		LogPath = ExtractFilePath(Application->ExeName) + LoadStr(IDS_LOG_PATH);
		if (!DirectoryExists(LogPath)) {
			CreateDir(LogPath);
		}

		LogFileName = IncludeTrailingPathDelimiter(LogPath) +
			ChangeFileExt(ExtractFileName(Application->ExeName),
			LoadStr(IDS_LOG_EXT));

		LogFile = CreateFile(LogFileName.w_str(), GENERIC_READ, FILE_SHARE_READ,
			NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		dwSize = 0;
		if (LogFile != INVALID_HANDLE_VALUE) {
			dwSize = GetFileSize(LogFile, NULL);
			CloseHandle(LogFile);
		}

		if (dwSize > MaxLogSize) {
			String NewFileName = IncludeTrailingPathDelimiter(LogPath) +
				OnlyFileName(Application->ExeName) + SPACE + FormatDateTime
				(LoadStr(IDS_DATETIME_FORMAT_FILENAME), DateTime) +
				LoadStr(IDS_LOG_EXT);
			MoveFile(LogFileName.w_str(), NewFileName.w_str());
		}

		LogFile = CreateFile(LogFileName.w_str(), GENERIC_WRITE,
			FILE_SHARE_READ, NULL, OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_COMPRESSED, 0);

		if (LogFile != INVALID_HANDLE_VALUE) {
			SS = AnsiString(FormatDateTime(LoadStr(IDS_DATETIME_FORMAT_LOG),
				DateTime) + "; " + RemoveLineBreaks(S) + sLineBreak);

			if (SetFilePointer(LogFile, 0, NULL,
				FILE_END) != INVALID_SET_FILE_POINTER) {
				WriteFile(LogFile, SS.c_str(), SS.Length(),
					NumberOfBytesWritten, NULL);
			}

			CloseHandle(LogFile);
		}
	}
	catch (...) {
	}
}

// ---------------------------------------------------------------------------
void WriteToLog(NativeUInt Ident) {
	WriteToLog(LoadStr(Ident));
}

// ---------------------------------------------------------------------------
void WriteToLogForm(bool AShow, String AFormName) {
	// show (close): Имя формы

	AFormName = ": " + AFormName;

	if (AShow) {
		AFormName = "show" + AFormName;
	}
	else {
		AFormName = "close" + AFormName;
	}

	WriteToLog(AFormName);
}

// ---------------------------------------------------------------------------
void WriteToLogProgramStart() {
	TVSFixedFileInfo FileVersionInfo;

	String CompanyName, FileDescription, FileVersion, InternalName,
		LegalCopyright, OriginalFilename, ProductName, ProductVersion;

	String Name = "";
	String Version = "";

	if (GetFileVerInfo(Application->ExeName, FileVersionInfo, CompanyName,
		FileDescription, FileVersion, InternalName, LegalCopyright,
		OriginalFilename, ProductName, ProductVersion)) {
		Name = InternalName;

		Version = SmallFileVersion(FileVersion);

		if (IsValueInWord(FileVersionInfo.dwFileFlags, VS_FF_DEBUG)) {
			Version = Version + " (Debug build)";
		}
	}

	WriteToLog(Format(IDS_LOG_PROGRAM_START, ARRAYOFCONST((Name, Version))));
}

// ---------------------------------------------------------------------------
void WriteToLogProgramStop() {
	WriteToLog(IDS_LOG_PROGRAM_STOP);
}

// ---------------------------------------------------------------------------
