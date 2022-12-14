// ---------------------------------------------------------------------------

#ifndef UtilsSQLH
#define UtilsSQLH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>

const TDateTime DEFAULT_DATETIME = 0;

// ---------------------------------------------------------------------------
String SQLLoad(NativeUInt Ident);
String SQLMake(String S1, String S2);
String SQLMake(String S1, NativeUInt Ident);
String SQLSetText(TADOQuery * Query, NativeUInt Ident);

// ---------------------------------------------------------------------------
void SQLReplace(TADOQuery * Query, String OldPattern, String NewPattern);
void SQLReplace(TADOQuery * Query, NativeUInt OldPattern,
	NativeUInt NewPattern);

// ---------------------------------------------------------------------------
TParameter * SQLGetParam(TADOQuery * Query, String Name, TFieldType DataType);
String SQLParamsToStr(TADOQuery * Query);

// ---------------------------------------------------------------------------
String DateTimeToSQLStr(TDateTime ADateTime);

// ---------------------------------------------------------------------------
String SQLRemoveExtra(String S);

// ---------------------------------------------------------------------------
#endif
