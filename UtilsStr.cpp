// ---------------------------------------------------------------------------

#pragma hdrstop

#include <System.SysUtils.hpp>

#include "UtilsStr.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

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

void SplitStr(String S, const String SplitString, const int SplitPlace,
	String &FirstHalf, String &SecondHalf) {
	int P;

	FirstHalf == S;
	SecondHalf == "";

	if (S == NULL)
		return;

	P = PosPlace(SplitString, S, SplitPlace);
	if (P != 0) {
		FirstHalf  = S.SubString(1, P - 1);
		SecondHalf = S.SubString(P + SplitString.Length(), MaxInt);
	}
}

TRect StrToRect(String S) {
	String FirstHalf, SecondHalf;

	TRect Result;

	if (S == NULL)
		return Result;

	try {
		SplitStr(S, COMMA, 0, FirstHalf, SecondHalf);
		Result.Left = StrToInt(FirstHalf);
		SplitStr(SecondHalf, COMMA, 0, FirstHalf, SecondHalf);
		Result.Top = StrToInt(FirstHalf);
		SplitStr(SecondHalf, COMMA, 0, FirstHalf, SecondHalf);
		Result.Right  = StrToInt(FirstHalf);
		Result.Bottom = StrToInt(SecondHalf);
	}
	catch (Exception *e) {
		throw Exception("Ñòðîêà '" + S + "' íå ÿâëÿåòñÿ çàïèñüþ TRect");
	}
	return Result;
}

String RectToStr(TRect R) {
	return IntToStr((int) R.Left) + COMMA + IntToStr((int) R.Top) + COMMA +
		IntToStr((int) R.Right) + COMMA + IntToStr((int) R.Bottom);
}

String FormatBytes(SIZE_T Bytes) {
	const kb         = 1024, Mb = 1048576;
	const __int64 Gb = 1073741824LL;
	const __int64 Tb = 1099511627776LL;

	String BytesArray[5] = {"á", "Êá", "Ìá", "Ãá", "Òá"};

	Byte i;
	Extended DivBytes;
	String Result;

	if ((Bytes / Tb) != 0)
		i = 4;
	else if ((Bytes / Gb) != 0)
		i = 3;
	else if ((Bytes / Mb) != 0)
		i = 2;
	else if ((Bytes / kb) != 0)
		i = 1;
	else
		i = 0;

	switch (i) {
	case 1:
		DivBytes = (Extended) Bytes / kb;
		break;
	case 2:
		DivBytes = (Extended) Bytes / Mb;
		break;
	case 3:
		DivBytes = (Extended) Bytes / Gb;
		break;
	case 4:
		DivBytes = (Extended) Bytes / Tb;
		break;
	default:
		DivBytes = 0;
	}

	if (i == 0)
		Result = IntToStr((__int64) Bytes);
	else
		Result = FloatToStrF(DivBytes, ffFixed, 18, 1);

	Result = Result + SPACE + BytesArray[i];

	return Result;
}

String FormatHerzs(SIZE_T Herzs) {
	const kHz         = 1000, MHz = 1000000;
	const __int64 GHz = 1000000000LL;
	const __int64 THz = 1000000000000LL;

	String HerzsArray[5] = {"Ãö", "ÊÃö", "ÌÃö", "ÃÃö", "ÒÃö"};

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

bool IsEmpty(const String S) {
	return S == NULL | S.IsEmpty();
}

String ConcatStrings(const String S1, const String S2, const String Separator) {
	if (IsEmpty(S1) & IsEmpty(S2))
		return "";
	else if (IsEmpty(S1))
		return S2;
	else if (IsEmpty(S2))
		return S1;
	else
		return S1 + Separator + S2;
}

String Format(const NativeUInt Ident, TVarRec const *Args, const int Args_High)
{
	return Format(LoadStr(Ident), Args, Args_High);
}

String Format(const String F, const String S) {
	return Format(F, ARRAYOFCONST((S)));
}

String Format(const NativeUInt Ident, const String S) {
	return Format(LoadStr(Ident), S);
}

String IToS(int Value) {
	return IntToStr(Value);
}

String IToS_0(int Value, int MinLength) {
	String Result = IToS(Value);
	while (Result.Length() < MinLength) Result = "0" + Result;
	return Result;
}
