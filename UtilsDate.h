// ---------------------------------------------------------------------------

#ifndef UtilsDateH
#define UtilsDateH

#include <System.Types.hpp>
#include <System.Classes.hpp>
#include <System.DateUtils.hpp>

// ---------------------------------------------------------------------------
void DecodeMilliseconds(ULONGLONG Value, DWORD &Day, BYTE &Hour, BYTE &Minute,
	BYTE &Second, WORD &Milliseconds);
String DHMSMSToString(DWORD Day, BYTE Hour, BYTE Minute, BYTE Second,
	WORD Milliseconds, bool ShowMilliseconds = true);
String MillisecondsToDateTimeString(ULONGLONG Value,
	bool ShowMilliseconds = true);

String FormatDate(String Format, TSystemTime SystemTime);
String FormatTime(String Format, TSystemTime SystemTime);

String GetTimeZone(TDateTime Value);
// ---------------------------------------------------------------------------
#endif
