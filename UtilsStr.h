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

// ---------------------------------------------------------------------------
int PosPlace(String Substr, String S, int Place);

// ---------------------------------------------------------------------------
void SplitStr(String S, const String SplitString, const int SplitPlace,
	String &FirstHalf, String &SecondHalf);

// ---------------------------------------------------------------------------
TRect StrToRect(String S);
String RectToStr(TRect R);

// ---------------------------------------------------------------------------
TPoint StrToPoint(String S);
String PointToStr(TPoint P);

// ---------------------------------------------------------------------------
void StrToFont(String S, TFont * Font);
String FontToStr(TFont * Font);

// ---------------------------------------------------------------------------
String FormatBytes(Extended Bytes, TStrings * ByteNames = NULL);
String FormatHerzs(SIZE_T Herzs);

// ---------------------------------------------------------------------------
bool IsInt(const String S);

// ---------------------------------------------------------------------------
String ConcatStrings(const String S1, const String S2, const String Separator);

// ---------------------------------------------------------------------------
String ReplaceLineBreaks(const String S, const char C = SPACE);
String RemoveExtraSpaces(const String S);

// ---------------------------------------------------------------------------
String Format(const NativeUInt Ident, TVarRec const * Args,
	const int Args_High);
String Format(const String F, const String S);
String Format(const NativeUInt Ident, const String S);
String Format(const NativeUInt Ident, const int I);

// ---------------------------------------------------------------------------
String IToS(int Value);
String IToS_0(int Value, int MinLength = 2);

// ---------------------------------------------------------------------------
String WinShortCutToText(TShortCut ShortCut, bool HasWinKey);
TShortCut TextToWinShortCut(String Text, bool &HasWinKey);

// ---------------------------------------------------------------------------
void SAdd(TStrings * S, String Text);
void SAdd(TStrings * S, NativeUInt Ident);
bool SContains(TStrings * S, String Text);

// ---------------------------------------------------------------------------
String RandomStr(String SeedString, int Length);

// ---------------------------------------------------------------------------
String GUIDToUUID(String Value);
String UUIDToGUID(String Value);
String CreateGUID();
String CreateUUID();

// ---------------------------------------------------------------------------
#endif
