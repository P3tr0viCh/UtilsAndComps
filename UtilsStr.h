// ---------------------------------------------------------------------------

#ifndef UtilsStrH
#define UtilsStrH

#include <System.Types.hpp>
#include <System.Classes.hpp>
#include <Vcl.Menus.hpp>

// ---------------------------------------------------------------------------
const Char COMMA = ',';
const Char SPACE = ' ';
const Char DOT = '.';
const Char TAB = '\t';

int PosPlace(String Substr, String S, int Place);
void SplitStr(String S, const String SplitString, const int SplitPlace,
	String &FirstHalf, String &SecondHalf);

TRect StrToRect(String S);
String RectToStr(TRect R);

TPoint StrToPoint(String S);
String PointToStr(TPoint P);

void StrToFont(String S, TFont * Font);
String FontToStr(TFont * Font);

String FormatBytes(Extended Bytes, TStrings * ByteNames = NULL);
String FormatHerzs(SIZE_T Herzs);

bool IsEmpty(const String S);
String ConcatStrings(const String S1, const String S2, const String Separator);
String RemoveLineBreaks(const String S);
String RemoveExtraSpaces(const String S);

String Format(const NativeUInt Ident, TVarRec const * Args,
	const int Args_High);
String Format(const String F, const String S);
String Format(const NativeUInt Ident, const String S);
String Format(const NativeUInt Ident, const int I);

String IToS(int Value);
String IToS_0(int Value, int MinLength = 2);

String WinShortCutToText(TShortCut ShortCut, bool HasWinKey);
TShortCut TextToWinShortCut(String Text, bool &HasWinKey);

// ---------------------------------------------------------------------------
#endif
