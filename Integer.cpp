// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsStr.h>
#include <UtilsMisc.h>
#include <UtilsLog.h>

#include "Integer.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TInteger::TInteger() {
	Init();
}

// ---------------------------------------------------------------------------
__fastcall TInteger::TInteger(int Value) {
	Init();
	FValue = Value;
}

// ---------------------------------------------------------------------------
void TInteger::Init() {
	FValue = 0;
}

// ---------------------------------------------------------------------------
__fastcall TInteger::~TInteger() {
}

// ---------------------------------------------------------------------------
bool __fastcall TInteger::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TInteger * Integer = (TInteger*) Obj;

	if (Value != Integer->Value) {
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TInteger::Assign(TInteger * Source) {
	Value = Source->Value;
}

// ---------------------------------------------------------------------------
String __fastcall TInteger::ToString() {
	String S;

	S = "TInteger{";
	S += "Value=" + IntToStr(Value);
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
