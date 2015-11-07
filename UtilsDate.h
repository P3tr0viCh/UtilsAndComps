//---------------------------------------------------------------------------

#ifndef UtilsDateH
#define UtilsDateH
//---------------------------------------------------------------------------
#endif

#include <windows.h>
#include <System.Types.hpp>

SYSTEMTIME ExtractHMSFromMS(DWORD MilliSeconds);
String MyFormatTime(SYSTEMTIME SystemTime, bool WithMSec = false);
