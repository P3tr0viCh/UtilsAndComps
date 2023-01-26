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
	virtual void SetStringGridHeader(TStringGrid * Grid) = 0;

	// -----------------------------------------------------------------------
	__property int Count = {read = GetCount};
};

// ---------------------------------------------------------------------------
class TStringGridRowService : public TObject {
	TStringGrid * FGrid;

	int FRow;

	bool FChanged;
	bool FReadOnly;

	// -----------------------------------------------------------------------
	void SetChanged(bool Value);
	void SetReadOnly(bool Value);

public:
	__fastcall TStringGridRowService(TStringGrid * Grid, int Row);

	// -----------------------------------------------------------------------
	__property TStringGrid * Grid = {read = FGrid};

	__property int Row = {read = FRow};

	__property bool Changed = {read = FChanged, write = SetChanged};
	__property bool ReadOnly = {read = FReadOnly, write = SetReadOnly};
};

// ---------------------------------------------------------------------------
enum TEditorType {
	etText, etComboBox
};

// ---------------------------------------------------------------------------
class TStringGridColService : public TObject {
	TStringGrid * FGrid;

	bool FReadOnly;
	TAlignment FAlignment;
	TEditorType FEditorType;

public:
	__fastcall TStringGridColService(TStringGrid * Grid);

	// -----------------------------------------------------------------------
	__property TStringGrid * Grid = {read = FGrid, write = FGrid};

	__property bool ReadOnly = {read = FReadOnly, write = FReadOnly};
	__property TAlignment Alignment = {read = FAlignment, write = FAlignment};
	__property TEditorType EditorType = {read = FEditorType, write = FEditorType
	};
};

// ---------------------------------------------------------------------------
class TStringGridOptions : public TObject {
	TStringGrid * FGrid;

	TColor FColorChanged;
	TColor FColorReadOnly;
	TColor FColorSelected;

	bool FDrawFocusedOnInactive;

	// -----------------------------------------------------------------------
	void Init();

	bool GetEditing();
	void SetEditing(bool Value);
	bool GetColSizing();
	void SetColSizing(bool Value);

	void SetDefaultRowHeight(int Value);

public:
	__fastcall TStringGridOptions();
	__fastcall TStringGridOptions(TStringGrid * Grid);

	// -----------------------------------------------------------------------
	__property TStringGrid * Grid = {read = FGrid, write = FGrid};

	__property TColor ColorChanged = {
		read = FColorChanged, write = FColorChanged};
	__property TColor ColorReadOnly = {
		read = FColorReadOnly, write = FColorReadOnly};
	__property TColor ColorSelected = {
		read = FColorSelected, write = FColorSelected};

	__property bool Editing = {read = GetEditing, write = SetEditing};
	__property bool ColSizing = {read = GetColSizing, write = SetColSizing};

	__property bool DrawFocusedOnInactive = {
		read = FDrawFocusedOnInactive, write = FDrawFocusedOnInactive};
};

// ---------------------------------------------------------------------------
void StringGridInit(TStringGrid * Grid, TStringGridBaseColumns * Columns);

// ---------------------------------------------------------------------------
bool StringGridIsEmpty(TStringGrid * Grid);
int StringGridAddRow(TStringGrid * Grid);
void StringGridClear(TStringGrid * Grid);
void StringGridSelectCell(TStringGrid * Grid, int ACol, int ARow);
void StringGridUpdateOrderNum(TStringGrid * Grid, int ACol, int StartValue = 1);
void StringGridDeleteRow(TStringGrid * Grid, int ARow, int AColCount = -1);

// ---------------------------------------------------------------------------
TStringGridRowService * StringGridGetRowService(TStringGrid * Grid, int ARow);
TStringGridColService * StringGridGetColService(TStringGrid * Grid, int ACol);

// ---------------------------------------------------------------------------
void StringGridSetHeader(TStringGrid * Grid, int ACol, String ColName,
	int ColWidth);
void StringGridSetHeader(TStringGrid * Grid, int ACol, NativeUInt ColNameIdent,
	int ColWidth);
void StringGridSetCellInt(TStringGrid * Grid, int ACol, int ARow, int Value,
	bool ShowZero = false);

// ---------------------------------------------------------------------------
void StringGridSetCellChecked(TStringGrid * Grid, int ACol, int ARow,
	bool Value);
bool StringGridGetCellChecked(TStringGrid * Grid, int ACol, int ARow);

// ---------------------------------------------------------------------------
void StringGridDrawCell(TStringGrid * Grid, int ACol, int ARow, TRect Rect,
	TGridDrawState State, TStringGridOptions * Options);
void StringGridInvalidateCell(TStringGrid * Grid, int ACol, int ARow);
void StringGridInvalidateRow(TStringGrid * Grid, int ARow);
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
