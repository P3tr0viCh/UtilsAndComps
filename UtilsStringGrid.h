// ---------------------------------------------------------------------------

#ifndef UtilsStringGridH
#define UtilsStringGridH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>
#include <Vcl.Grids.hpp>

#include "UtilsFileIni.h"

// ---------------------------------------------------------------------------
bool StringGridIsEmpty(TStringGrid * Grid);
void StringGridClear(TStringGrid * Grid);
void StringGridSelectCell(TStringGrid * Grid, int ACol, int ARow);
void StringGridUpdateOrderNum(TStringGrid * Grid, int StartValue = 1);
void StringGridDeleteRow(TStringGrid * Grid, int ARow, int AColCount = -1);

void StringGridSetHeader(TStringGrid * Grid, int ACol, String ColName,
	int ColWidth);
void StringGridSetHeader(TStringGrid * Grid, int ACol, NativeUInt ColNameIdent,
	int ColWidth);

void StringGridDrawCell(TStringGrid * Grid, int ACol, int ARow, TRect Rect,
	TGridDrawState State, TIntegerSet ColsReadOnly, TIntegerSet ColsLeftAlign,
	TIntegerSet ColsCustomColor, TColor ReadOnlyColor, TColor CustomColor,
	bool DrawFocusedOnInactive, bool ReadOnlyRow, bool DrawChanged,
	TColor ChangedColor, bool DrawSelectedRow, TColor SelectedRowColor);
void StringGridInvalidateCell(TStringGrid * Grid, int ACol, int ARow);

void StringGridMouseToCell(TStringGrid * Grid, int &ACol, int &ARow);
void StringGridSelectRowAfterFixedCellClick(TStringGrid * Grid, int ARow);

void StringGridColWidthsWriteToIni(TStringGrid * Grid, TFileIni * FileIni,
	const String Section, const String Ident);
void StringGridColWidthsReadFromIni(TStringGrid * Grid, TFileIni * FileIni,
	const String Section, const String Ident);

// ---------------------------------------------------------------------------
#endif
