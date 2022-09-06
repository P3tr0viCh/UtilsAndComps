// ---------------------------------------------------------------------------

#ifndef UtilsSQLH
#define UtilsSQLH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>

// ---------------------------------------------------------------------------
String SQLMake(String S1, String S2);
String SQLMake(String S1, NativeUInt Ident);
TParameter * SQLGetParam(TADOQuery * Query, String Name, TFieldType DataType);

// ---------------------------------------------------------------------------
String DateTimeToSQLStr(TDateTime ADateTime);

// ---------------------------------------------------------------------------
#endif
