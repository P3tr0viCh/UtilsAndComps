// ---------------------------------------------------------------------------

#pragma hdrstop

#include <UtilsStr.h>
#include <UtilsMisc.h>
#include <UtilsLog.h>

#include "IntegerPair.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TIntegerPair::TIntegerPair() {
	Init();
}

// ---------------------------------------------------------------------------
__fastcall TIntegerPair::TIntegerPair(int Value1, int Value2) {
	Init();

	FInt1->Value = Value1;
	FInt2->Value = Value2;
}

// ---------------------------------------------------------------------------
void TIntegerPair::Init() {
	FInt1 = new TInteger();
	FInt2 = new TInteger();
}

// ---------------------------------------------------------------------------
void TIntegerPair::SetInt1(TInteger * Value) {
	FInt1->Assign(Value);
}

// ---------------------------------------------------------------------------
void TIntegerPair::SetInt2(TInteger * Value) {
	FInt2->Assign(Value);
}

// ---------------------------------------------------------------------------
__fastcall TIntegerPair::~TIntegerPair() {
	FInt2->Free();
	FInt1->Free();
}

// ---------------------------------------------------------------------------
bool __fastcall TIntegerPair::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TIntegerPair * IntegerPair = (TIntegerPair*) Obj;

	if (!Int1->Equals(IntegerPair->Int1) || !Int2->Equals(IntegerPair->Int2)) {
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TIntegerPair::Assign(TIntegerPair * Source) {
	Int1 = Source->Int1;
	Int2 = Source->Int2;
}

// ---------------------------------------------------------------------------
String __fastcall TIntegerPair::ToString() {
	String S;

	S = "TIntegerPair{";
	S += "Int1=" + Int1->ToString();
	S += ",";
	S += "Int2=" + Int2->ToString();
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
