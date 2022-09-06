// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsStr.h>

#include "UtilsSQL.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
String SQLMake(String S1, String S2) {
	return ConcatStrings(S1, S2, SPACE);
}

// ---------------------------------------------------------------------------
String SQLMake(String S1, NativeUInt Ident) {
	return ConcatStrings(S1, LoadStr(Ident), SPACE);
}

// ---------------------------------------------------------------------------
TParameter * SQLGetParam(TADOQuery * Query, String Name, TFieldType DataType) {
	TParameter * Parameter = Query->Parameters->ParamByName(Name);
	Parameter->DataType = DataType;
	return Parameter;
}

// ---------------------------------------------------------------------------
String DateTimeToSQLStr(TDateTime ADateTime) {
	return FormatDateTime("yyyy-MM-dd hh:nn:ss", ADateTime);
}

// ---------------------------------------------------------------------------
