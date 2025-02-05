// ---------------------------------------------------------------------------

#pragma hdrstop

#include <math.h>

#include "UtilsDate.h"

#pragma package(smart_init)

#include "UtilsStr.h"

// ---------------------------------------------------------------------------
void DecodeMilliseconds(ULONGLONG Value, DWORD &Day, BYTE &Hour, BYTE &Minute,
	BYTE &Second, WORD &Milliseconds) {
	const ULONGLONG MSInSec = 1000;
	const ULONGLONG MSInMin = 60 * MSInSec;
	const ULONGLONG MSInHour = 60 * MSInMin;
	const ULONGLONG MSInDay = 24 * MSInHour;

	Day = Value / MSInDay;
	Value = Value - (Day * MSInDay);
	Hour = Value / MSInHour;
	Value = Value - (Hour * MSInHour);
	Minute = Value / MSInMin;
	Value = Value - (Minute * MSInMin);
	Second = Value / MSInSec;
	Milliseconds = Value - (Second * MSInSec);
}

// ---------------------------------------------------------------------------
String DHMSMSToString(DWORD Day, BYTE Hour, BYTE Minute, BYTE Second,
	WORD Milliseconds, bool ShowMilliseconds) {
	String Result = "";

	if (Day != 0) {
		Result = IToS_0(Day) + "d ";
	}
	Result = Result + IToS_0(Hour) + ":" + IToS_0(Minute) + ":" +
		IToS_0(Second);
	if (ShowMilliseconds) {
		Result = Result + "." + IToS_0(Milliseconds, 3);
	}

	return Result;
}

// ---------------------------------------------------------------------------
String MillisecondsToDateTimeString(ULONGLONG Value, bool ShowMilliseconds) {
	DWORD Day;
	BYTE Hour, Minute, Second;
	WORD Milliseconds;

	DecodeMilliseconds(Value, Day, Hour, Minute, Second, Milliseconds);
	return DHMSMSToString(Day, Hour, Minute, Second, Milliseconds,
		ShowMilliseconds);
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
String GetTimeZone(TDateTime Value) {
	TTimeSpan TimeSpan = TTimeZone::Local->GetUtcOffset(Value);

	String Result = IToS_0(abs(TimeSpan.Hours), 2) + ":" + IToS_0(abs(TimeSpan.Minutes), 2);

	if (TimeSpan.Hours >= 0) {
		Result = "+" + Result;
	} else {
		Result = "-" + Result;
	}

	return Result;
}

// ---------------------------------------------------------------------------
