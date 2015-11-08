//---------------------------------------------------------------------------

#ifndef UtilsStrH
#define UtilsStrH
//---------------------------------------------------------------------------
#endif

#include <System.Types.hpp>

const Char COMMA = ',';
const Char SPACE = ' ';
const Char DOT = '.';

int PosPlace(String Substr, String S, int Place);
void SplitStr(String S, const String SplitString, const int SplitPlace,
	String &FirstHalf, String &SecondHalf);

TRect StrToRect(String S);
String RectToStr(TRect R);

String FormatBytes(SIZE_T Bytes);
String FormatHerzs(SIZE_T Herzs);

bool IsEmpty(const String S);
String ConcatStrings(const String S1, const String S2, const String Separator);

String Format(const NativeUInt Ident, TVarRec const *Args, const int Args_High);
String Format(const String F, const String S);
String Format(const NativeUInt Ident, const String S);

String IToS(int Value);
String IToS_0(int Value, int MinLength = 2);
