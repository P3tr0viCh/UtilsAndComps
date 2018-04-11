//---------------------------------------------------------------------------

#pragma hdrstop

#include "UtilsDate.h"

#pragma package(smart_init)

#include "UtilsStr.h"

//---------------------------------------------------------------------------
SYSTEMTIME ExtractHMSFromMS(DWORD MilliSeconds) {
	const MSInSec  = 1000;
	const MSInMin  = 60 * MSInSec;
	const MSInHour = 60 * MSInMin;
	const MSInDay  = 24 * MSInHour;

	DWORD MS = MilliSeconds;
	SYSTEMTIME Result;

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

String MyFormatTime(SYSTEMTIME SystemTime, bool WithMSec) {
	String Result = "";

	if (SystemTime.wDay != 0) Result = IToS_0(SystemTime.wDay) + "d ";
	Result = Result + IToS_0(SystemTime.wHour) + ":" +
		IToS_0(SystemTime.wMinute) + ":" + IToS_0(SystemTime.wSecond);
	if (WithMSec) Result = Result + "." + IToS_0(SystemTime.wMilliseconds, 3);

	return Result;
}
