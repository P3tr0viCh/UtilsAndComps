// ---------------------------------------------------------------------------

#pragma hdrstop

#include <System.SysUtils.hpp>

#include "UtilsStr.h"

#pragma package(smart_init)

// ---------------------------------------------------------------------------
int PosPlace(String Substr, String S, int Place) {
	int i, P;

	int Result = 0;

	if (S == NULL)
		return Result;

	if (Substr == NULL)
		return Result;

	for (int i = 0; i <= Place; i++) {
		P = Pos(Substr, S);
		if (P == 0)
			break;
		S.Delete(1, P);
		Result = Result + P;
	}

	return Result;
}

// ---------------------------------------------------------------------------
void SplitStr(String S, const String SplitString, const int SplitPlace,
	String &FirstHalf, String &SecondHalf) {
	int P;

	FirstHalf = S;
	SecondHalf = "";

	if (S.IsEmpty()) {
		return;
	}

	P = PosPlace(SplitString, S, SplitPlace);
	if (P != 0) {
		FirstHalf = S.SubString(1, P - 1);
		SecondHalf = S.SubString(P + SplitString.Length(), MaxInt);
	}
}

// ---------------------------------------------------------------------------
TRect StrToRect(String S) {
	String FirstHalf, SecondHalf;

	TRect Result = TRect(0, 0, 0, 0);

	if (S.IsEmpty()) {
		return Result;
	}

	try {
		SplitStr(S, COMMA, 0, FirstHalf, SecondHalf);
		Result.Left = StrToInt(FirstHalf);
		SplitStr(SecondHalf, COMMA, 0, FirstHalf, SecondHalf);
		Result.Top = StrToInt(FirstHalf);
		SplitStr(SecondHalf, COMMA, 0, FirstHalf, SecondHalf);
		Result.Right = StrToInt(FirstHalf);
		Result.Bottom = StrToInt(SecondHalf);
	}
	catch (Exception *e) {
		throw Exception("������ '" + S + "' �� �������� ������� TRect");
	}

	return Result;
}

// ---------------------------------------------------------------------------
String RectToStr(TRect R) {
	return IntToStr((int) R.Left) + COMMA + IntToStr((int) R.Top) + COMMA +
		IntToStr((int) R.Right) + COMMA + IntToStr((int) R.Bottom);
}

// ---------------------------------------------------------------------------
TPoint StrToPoint(String S) {
	String FirstHalf, SecondHalf;

	TPoint Result = TPoint(0, 0);

	if (S.IsEmpty()) {
		return Result;
	}

	try {
		SplitStr(S, COMMA, 0, FirstHalf, SecondHalf);
		Result.X = StrToInt(FirstHalf);
		Result.Y = StrToInt(SecondHalf);
	}
	catch (...) {
		throw Exception("������ '" + S + "' �� �������� ������� TPoint");
	}

	return Result;
}

// ---------------------------------------------------------------------------
String PointToStr(TPoint P) {
	return IToS(P.x) + COMMA + IToS(P.y);
}

// ---------------------------------------------------------------------------
void StrToFont(String S, TFont * Font) {
	// Byte intFontStyle;
	// TFontStyles FontStyle;
	String FirstHalf, SecondHalf;

	if (S.IsEmpty()) {
		return;
	}

	try {
		SplitStr(S, COMMA, 0, FirstHalf, SecondHalf);
		Font->Name = FirstHalf;
		SplitStr(SecondHalf, COMMA, 0, FirstHalf, SecondHalf);
		Font->Charset = StrToInt(FirstHalf);
		SplitStr(SecondHalf, COMMA, 0, FirstHalf, SecondHalf);
		Font->Color = TColor(StrToInt(FirstHalf));
		SplitStr(SecondHalf, COMMA, 0, FirstHalf, SecondHalf);
		Font->Size = StrToInt(FirstHalf);
		// intFontStyle = StrToInt(SecondHalf);
		////		Move(intFontStyle, FontStyle, 1);
		// Font->Style = FontStyle;
	}
	catch (...) {
		throw Exception("������ '" + S + "' �� �������� ������� TFont");
	}
}

// ---------------------------------------------------------------------------
String FontToStr(TFont * Font) {
	// TODO
	// Byte intFontStyle  = Font->Style.ToInt();
	// TFontStyles FontStyle = Font->Style;
	// Move(FontStyle, intFontStyle, 1);

	return Font->Name + COMMA + IToS(Font->Charset) + COMMA + IToS(Font->Color)
		+ COMMA + IToS(Font->Size);
	// + COMMA + IToS(intFontStyle);
}

// ---------------------------------------------------------------------------
String FormatBytes(Extended Bytes, TStrings *ByteNames) {
	const Kb = 1024, Mb = 1048576;
	const __int64 Gb = 1073741824LL;
	const __int64 Tb = 1099511627776LL;

	TStrings *Names = new TStringList();

	if (ByteNames != NULL) {
		Names->AddStrings(ByteNames);
	}

	while (Names->Count < 5) {
		Names->Add("");
	}

	if (Names->Strings[0].IsEmpty())
		Names->Strings[0] = "�";
	if (Names->Strings[1].IsEmpty())
		Names->Strings[1] = "��";
	if (Names->Strings[2].IsEmpty())
		Names->Strings[2] = "��";
	if (Names->Strings[3].IsEmpty())
		Names->Strings[3] = "��";
	if (Names->Strings[4].IsEmpty())
		Names->Strings[4] = "��";

	Extended EBytes[5] = {0, Kb, Mb, Gb, Tb};

	int i;
	String Result;

	Extended AbsBytes = fabs(Bytes);

	for (i = 4; i > 0; i--) {
		if (AbsBytes >= EBytes[i]) {
			break;
		}
	}

	if (i == 0) {
		Result = IntToStr((__int64) Bytes);
	}
	else {
		Result = FloatToStrF(Bytes / EBytes[i], ffFixed, 18, 1);
	}

	Result = Result + SPACE + Names->Strings[i];

	Names->Free();

	return Result;
}

// ---------------------------------------------------------------------------
String FormatHerzs(SIZE_T Herzs) {
	const kHz = 1000, MHz = 1000000;
	const __int64 GHz = 1000000000LL;
	const __int64 THz = 1000000000000LL;

	String HerzsArray[5] = {"��", "���", "���", "���", "���"};

	Byte i;
	Extended DivHerzs;
	String Result;

	if ((Herzs / THz) != 0)
		i = 4;
	else if ((Herzs / GHz) != 0)
		i = 3;
	else if ((Herzs / MHz) != 0)
		i = 2;
	else if ((Herzs / kHz) != 0)
		i = 1;
	else
		i = 0;

	switch (i) {
	case 1:
		DivHerzs = (Extended) Herzs / kHz;
		break;
	case 2:
		DivHerzs = (Extended) Herzs / MHz;
		break;
	case 3:
		DivHerzs = (Extended) Herzs / GHz;
		break;
	case 4:
		DivHerzs = (Extended) Herzs / THz;
		break;
	default:
		DivHerzs = 0;
	}

	if (i == 0)
		Result = IntToStr((__int64) Herzs);
	else
		Result = FloatToStrF(DivHerzs, ffFixed, 18, 2);

	Result = Result + SPACE + HerzsArray[i];

	return Result;
}

// ---------------------------------------------------------------------------
bool IsInt(const String S) {
	if (S.IsEmpty()) {
		return false;
	}

	for (int i = 1; i < S.Length(); i++) {
		if (S[i] < '0' || S[i] > '9') {
			return false;
		}
	}

	return true;
}

// ---------------------------------------------------------------------------
String ConcatStrings(const String S1, const String S2, const String Separator) {
	if (S1.IsEmpty() && S2.IsEmpty())
		return "";
	else if (S1.IsEmpty())
		return S2;
	else if (S2.IsEmpty())
		return S1;
	else
		return S1 + Separator + S2;
}

// ---------------------------------------------------------------------------
String ReplaceLineBreaks(const String S, const char C) {
	String Result = S;

	for (int i = Result.Length(); i > 1; i--) {
		if (Result[i] == '\r') {
			Result.Delete(i, 1);
		}
		else {
			if (Result[i] == '\n') {
				Result[i] = C;
			}
		}
	}

	return Result;
}

// ---------------------------------------------------------------------------
String RemoveExtraSpaces(const String S) {
	String Result = S;

	for (int i = Result.Length(); i >= 2; i--) {
		if (Result[i] == SPACE && Result[i - 1] == SPACE) {
			Result.Delete(i, 1);
		}
	}

	return Result;
}

// ---------------------------------------------------------------------------
String Format(const NativeUInt Ident, TVarRec const *Args, const int Args_High)
{
	return Format(LoadStr(Ident), Args, Args_High);
}

// ---------------------------------------------------------------------------
String Format(const String F, const String S) {
	return Format(F, ARRAYOFCONST((S)));
}

// ---------------------------------------------------------------------------
String Format(const NativeUInt Ident, const String S) {
	return Format(LoadStr(Ident), S);
}

// ---------------------------------------------------------------------------
String Format(const NativeUInt Ident, const int I) {
	return Format(Ident, ARRAYOFCONST((I)));
}

// ---------------------------------------------------------------------------
String IToS(int Value) {
	return IntToStr(Value);
}

// ---------------------------------------------------------------------------
String IToS_0(int Value, int MinLength) {
	bool Neg = Value < 0;

	if (Neg) {
		Value = -Value;
	}

	String Result = IToS(Value);

	while (Result.Length() < MinLength)
		Result = "0" + Result;

	if (Neg) {
		Result = "-" + Result;
	}

	return Result;
}

// ---------------------------------------------------------------------------
String WinShortCutToText(TShortCut ShortCut, bool HasWinKey) {
	String Result = ShortCutToText(ShortCut);
	if ((!Result.IsEmpty() && HasWinKey)) {
		Result = "Win+" + Result;
	}
	return Result;
}

// ---------------------------------------------------------------------------
TShortCut TextToWinShortCut(String Text, bool &HasWinKey) {
	HasWinKey = Text.Pos("Win+") == 1;
	if (HasWinKey) {
		Text.Delete(1, 4);
	}
	return TextToShortCut(Text);
}

// ---------------------------------------------------------------------------
void SAdd(TStrings * S, String Text) {
	S->Add(Text);
}

// ---------------------------------------------------------------------------
void SAdd(TStrings * S, NativeUInt Ident) {
	S->Add(LoadStr(Ident));
}

// ---------------------------------------------------------------------------
bool SContains(TStrings * S, String Text) {
	return S->IndexOf(Text) != -1;
}

// ---------------------------------------------------------------------------
String RandomStr(String SeedString, int Length) {
	if (SeedString.IsEmpty()) {
		SeedString = "�����Ũ��������������������������";
	}

	String Result;

	Randomize;

	for (int i = 0; i < Length; i++) {
		Result = Result + SeedString[Random(SeedString.Length()) + 1];
	}

	return Result;
}

// ---------------------------------------------------------------------------
String GUIDToUUID(String Value) {
	return Value.SubString(2, 36);
}

// ---------------------------------------------------------------------------
String UUIDToGUID(String Value) {
	return "{" + Value + "}";
}

// ---------------------------------------------------------------------------
String CreateGUID() {
	GUID Guid;

	HRESULT HResult = CreateGUID(Guid);

	if (HResult == S_OK) {
		return GUIDToString(Guid);
	}
	else {
		throw Exception("generate guid failed");
	}
}

// ---------------------------------------------------------------------------
String CreateUUID() {
	return GUIDToUUID(CreateGUID());
}

// ---------------------------------------------------------------------------
