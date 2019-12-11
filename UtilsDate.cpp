// ---------------------------------------------------------------------------

#pragma hdrstop

#include "UtilsDate.h"

#pragma package(smart_init)

#include "UtilsStr.h"

// ---------------------------------------------------------------------------
TSystemTime ExtractHMSFromMS(DWORD MilliSeconds) {
	const MSInSec = 1000;
	const MSInMin = 60 * MSInSec;
	const MSInHour = 60 * MSInMin;
	const MSInDay = 24 * MSInHour;

	DWORD MS = MilliSeconds;
	TSystemTime Result;

	Result.wDay = MS / MSInDay;
	MS = MS - (Result.wDay * MSInDay);
	Result.wHour = MS / MSInHour;
	MS = MS - (Result.wHour * MSInHour);
	Result.wMinute = MS / MSInMin;
	MS = MS - (Result.wMinute * MSInMin);
	Result.wSecond = MS / MSInSec;
	Result.wMilliseconds = MS - (Result.wSecond * MSInSec);

	return Result;
}

// ---------------------------------------------------------------------------
String MyFormatTime(TSystemTime SystemTime, bool WithMSec) {
	String Result = "";

	if (SystemTime.wDay != 0) {
		Result = IToS_0(SystemTime.wDay) + "d ";
	}
	Result = Result + IToS_0(SystemTime.wHour) + ":" +
		IToS_0(SystemTime.wMinute) + ":" + IToS_0(SystemTime.wSecond);
	if (WithMSec) {
		Result = Result + "." + IToS_0(SystemTime.wMilliseconds, 3);
	}

	return Result;
}

// ---------------------------------------------------------------------------
String FormatDate(String Format, TSystemTime SystemTime) {
	LPCTSTR lpFormat;
	DWORD Flags;
	WCHAR Buffer[255];

	if (Format == NULL || Format.IsEmpty()) {
		lpFormat = NULL;
		Flags = DATE_LONGDATE;
	}
	else {
		lpFormat = Format.w_str();
		Flags = 0;
	}

	GetDateFormat(LOCALE_USER_DEFAULT, Flags, &SystemTime, lpFormat, Buffer,
		sizeof(Buffer));

	return Buffer;
}

// ---------------------------------------------------------------------------
String FormatTime(String Format, TSystemTime SystemTime) {
	LPCTSTR lpFormat;
	DWORD Flags;
	WCHAR Buffer[255];

	Flags = 0;
	if (Format == NULL || Format.IsEmpty()) {
		lpFormat = NULL;
	}
	else {
		lpFormat = Format.w_str();
	}

	GetTimeFormat(LOCALE_USER_DEFAULT, Flags, &SystemTime, lpFormat, Buffer,
		sizeof(Buffer));

	return Buffer;
}
// ---------------------------------------------------------------------------
