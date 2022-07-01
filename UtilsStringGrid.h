// ---------------------------------------------------------------------------

#ifndef UtilsStringGridH
#define UtilsStringGridH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>
#include <Vcl.Grids.hpp>

#include "UtilsFileIni.h"

// ---------------------------------------------------------------------------
class TStringGridBaseColumns : public TObject {
protected:
	static const SERVICE = 0;

	static const COUNT = 1;

	virtual int GetCount() {
		return COUNT;
	};

public:
	TIntegerSet ReadOnly;

	TIntegerSet ComboBox;

	TIntegerSet LeftAlign;

	virtual void SetStringGridHeader(TStringGrid * Grid) = 0;

	// -----------------------------------------------------------------------
	__property int Count = {read = GetCount};
};

// ---------------------------------------------------------------------------
class TStringGridService : public TObject {
	bool FChanged;

public:
	__property bool Changed = {read = FChanged, write = FChanged};
};

// ---------------------------------------------------------------------------
void StringGridInit(TStringGrid * Grid, TStringGridBaseColumns * Columns,
	int DefaultRowHeight);

// ---------------------------------------------------------------------------
bool StringGridIsEmpty(TStringGrid * Grid);
int StringGridAddRow(TStringGrid * Grid);
void StringGridClear(TStringGrid * Grid);
void StringGridSelectCell(TStringGrid * Grid, int ACol, int ARow);
void StringGridUpdateOrderNum(TStringGrid * Grid, int StartValue = 1);
void StringGridDeleteRow(TStringGrid * Grid, int ARow, int AColCount = -1);

// ---------------------------------------------------------------------------
TStringGridService * StringGridRowGetService(TStringGrid * Grid, int Index);

// ---------------------------------------------------------------------------
void StringGridRowSetChanged(TStringGrid * Grid, int Index, bool Changed);
bool StringGridRowIsChanged(TStringGrid * Grid, int Index);

// ---------------------------------------------------------------------------
void StringGridSetHeader(TStringGrid * Grid, int ACol, String ColName,
	int ColWidth);
void StringGridSetHeader(TStringGrid * Grid, int ACol, NativeUInt ColNameIdent,
	int ColWidth);
void StringGridSetCellInt(TStringGrid * Grid, int ACol, int ARow, int Value,
	bool ShowZero = false);

// ---------------------------------------------------------------------------
void StringGridDrawCell(TStringGrid * Grid, int ACol, int ARow, TRect Rect,
	TGridDrawState State, TIntegerSet ColsReadOnly, TIntegerSet ColsLeftAlign,
	TIntegerSet ColsCustomColor, TColor ReadOnlyColor, TColor CustomColor,
	bool DrawFocusedOnInactive, bool ReadOnlyRow, TColor ChangedColor,
	TColor SelectedRowColor = clMax);
void StringGridInvalidateCell(TStringGrid * Grid, int ACol, int ARow);
void StringGridInvalidateSelected(TStringGrid * Grid);

// ---------------------------------------------------------------------------
void StringGridMouseToCell(TStringGrid * Grid, int &ACol, int &ARow);
void StringGridSelectRowAfterFixedCellClick(TStringGrid * Grid, int ARow);

// ---------------------------------------------------------------------------
void StringGridColWidthsWriteToIni(TStringGrid * Grid, TFileIni * FileIni,
	const String Section, const String Ident);
void StringGridColWidthsReadFromIni(TStringGrid * Grid, TFileIni * FileIni,
	const String Section, const String Ident);

// ---------------------------------------------------------------------------
#endif
