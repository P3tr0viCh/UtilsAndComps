// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsStr.h>

#include "UtilsSQL.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
String SQLLoad(NativeUInt Ident) {
	String Result;

	TStrings * Strings = new TStringList;

	TResourceStream * Stream = new TResourceStream((int)HInstance,
		LoadStr(Ident), RT_RCDATA);

	try {
		Strings->LoadFromStream(Stream);

		Result = SQLRemoveExtra(Strings->Text);
	}
	__finally {
		Stream->Free();
		Strings->Free();
	}

	return Result;
}

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
String SQLRemoveExtra(const String S) {
	String Result = S;

	for (int i = Result.Length(); i > 1; i--) {
		if (Result[i] == '\t' || Result[i] == '\r') {
			Result.Delete(i, 1);
		}
		else {
			if (Result[i] == '\n') {
				Result[i] = ' ';
			}
		}
	}

	return Result;
}

// ---------------------------------------------------------------------------
