// ---------------------------------------------------------------------------

#ifndef UtilsDateH
#define UtilsDateH

#include <System.Types.hpp>
#include <System.Classes.hpp>

// ---------------------------------------------------------------------------
TSystemTime ExtractHMSFromMS(DWORD MilliSeconds);
String MyFormatTime(TSystemTime SystemTime, bool WithMSec = false);

String FormatDate(String Format, TSystemTime SystemTime);
String FormatTime(String Format, TSystemTime SystemTime);
// ---------------------------------------------------------------------------
#endif
