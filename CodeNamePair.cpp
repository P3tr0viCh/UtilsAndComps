// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsStr.h>
#include <UtilsSQL.h>
#include <UtilsMisc.h>
#include <UtilsLog.h>

#include "CodeNamePair.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TCodeNamePair::TCodeNamePair() {
	FCode = DEFAULT_CODE;
	FName = "";
}

// ---------------------------------------------------------------------------
__fastcall TCodeNamePair::TCodeNamePair(int Code, String Name) {
	FCode = Code;
	FName = Name;
}

// ---------------------------------------------------------------------------
__fastcall TCodeNamePair::TCodeNamePair(TCodeNamePair * Value) {
	Assign(Value);
}

// ---------------------------------------------------------------------------
bool __fastcall TCodeNamePair::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TCodeNamePair * CodeNamePair = (TCodeNamePair*) Obj;

	if (Code != CodeNamePair->Code || !AnsiSameStr(Name, CodeNamePair->Name)) {
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TCodeNamePair::Assign(TCodeNamePair * Source) {
	Code = Source->Code;
	Name = Source->Name;
}

// ---------------------------------------------------------------------------
String __fastcall TCodeNamePair::ToString() {
	String S;

	S = "TCodeNamePair{";
	S += "Code='" + IntToStr(Code) + "'";
	S += ",";
	S += "Name='" + Name + "'";
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
