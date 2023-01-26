// ---------------------------------------------------------------------------

#pragma hdrstop

#include "UtilsStringGrid.h"

#include "UtilsStr.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TStringGridRowService::TStringGridRowService(TStringGrid * Grid,
	int Row) {
	FGrid = Grid;

	FRow = Row;

	FChanged = false;
	FReadOnly = false;
}

// ---------------------------------------------------------------------------
void TStringGridRowService::SetChanged(bool Value) {
	if (FChanged == Value) {
		return;
	}

	FChanged = Value;

	StringGridInvalidateCell(Grid, TStringGridBaseColumns::SERVICE, Row);
}

// ---------------------------------------------------------------------------
void TStringGridRowService::SetReadOnly(bool Value) {
	if (FReadOnly == Value) {
		return;
	}

	FReadOnly = Value;

	StringGridInvalidateRow(Grid, Row);
}

// ---------------------------------------------------------------------------
__fastcall TStringGridColService::TStringGridColService(TStringGrid * Grid) {
	FGrid = Grid;

	FReadOnly = false;
	FAlignment = taCenter;
	FEditorType = etText;
}

// ---------------------------------------------------------------------------
void TStringGridOptions::Init() {
	FColorChanged = clMax;
	FColorReadOnly = clMax;
	FColorSelected = clMax;

	FDrawFocusedOnInactive = true;
}

// ---------------------------------------------------------------------------
__fastcall TStringGridOptions::TStringGridOptions() {
	Init();
}

// ---------------------------------------------------------------------------
__fastcall TStringGridOptions::TStringGridOptions(TStringGrid * Grid) {
	Init();

	FGrid = Grid;
}

// ---------------------------------------------------------------------------
bool TStringGridOptions::GetEditing() {
	return Grid->Options.Contains(goEditing);
}

// ---------------------------------------------------------------------------
void TStringGridOptions::SetEditing(bool Value) {
	if (Value) {
		Grid->Options = Grid->Options << goEditing;
	}
	else {
		Grid->Options = Grid->Options >> goEditing;
	}
}

// ---------------------------------------------------------------------------
bool TStringGridOptions::GetColSizing() {
	return Grid->Options.Contains(goColSizing);
}

// ---------------------------------------------------------------------------
void TStringGridOptions::SetColSizing(bool Value) {
	if (Value) {
		Grid->Options = Grid->Options << goColSizing;
	}
	else {
		Grid->Options = Grid->Options >> goColSizing;
	}
}

// ---------------------------------------------------------------------------
void StringGridInit(TStringGrid * Grid, TStringGridBaseColumns * Columns) {
	Grid->ColCount = Columns->Count;

	Grid->Canvas->Font->Assign(Grid->Font);
	Grid->DefaultRowHeight = Grid->Canvas->TextHeight("ComboBox") + 8;

	for (int ACol = TStringGridBaseColumns::SERVICE; ACol < Grid->ColCount;
	ACol++) {
		Grid->Objects[ACol][0] = new TStringGridColService(Grid);
	}

	StringGridSetHeader(Grid, TStringGridBaseColumns::SERVICE, 0, 16);

	Columns->SetStringGridHeader(Grid);
}

// ---------------------------------------------------------------------------
bool StringGridIsEmpty(TStringGrid * Grid) {
	return Grid->Objects[TStringGridBaseColumns::SERVICE][1] == NULL;
}

// ---------------------------------------------------------------------------
int StringGridAddRow(TStringGrid * Grid) {
	if (!StringGridIsEmpty(Grid)) {
		Grid->RowCount++;
	}

	int Row = Grid->RowCount - 1;

	Grid->Objects[TStringGridBaseColumns::SERVICE][Row] =
		new TStringGridRowService(Grid, Row);

	return Row;
}

// ---------------------------------------------------------------------------
void StringGridClear(TStringGrid * Grid) {
	for (int i = 1; i < Grid->RowCount; i++) {
		Grid->Rows[i]->Clear();
	}
	Grid->RowCount = 2;
}

// ---------------------------------------------------------------------------
void StringGridSelectCell(TStringGrid * Grid, int ACol, int ARow) {
	Grid->Col = ACol;
	Grid->Row = ARow;
}

// ---------------------------------------------------------------------------
void StringGridUpdateOrderNum(TStringGrid * Grid, int ACol, int StartValue) {
	for (int ARow = 1, Count = Grid->RowCount; ARow < Count; ARow++) {
		Grid->Cells[ACol][ARow] = IntToStr(StartValue);
		StartValue++;
	}
}

// ---------------------------------------------------------------------------
void StringGridDeleteRow(TStringGrid * Grid, int ARow, int AColCount) {
	int Count = Grid->RowCount;

	if (Count - Grid->FixedRows <= 1) {
		Grid->Rows[ARow]->Clear();
		return;
	}

	if (AColCount < 0) {
		for (ARow; ARow < Count; ARow++) {
			Grid->Rows[ARow] = Grid->Rows[ARow + 1];
		}
	}
	else {
		for (ARow; ARow < Count; ARow++) {
			for (int ACol = 0; ACol < AColCount; ACol++) {
				Grid->Cells[ACol][ARow] = Grid->Cells[ACol][ARow + 1];
			}
		}
	}

	Grid->Rows[Count]->Clear();

	Grid->RowCount--;
}

// ---------------------------------------------------------------------------
TStringGridRowService * StringGridGetRowService(TStringGrid * Grid, int ARow) {
	return ((TStringGridRowService*)Grid->Objects
		[TStringGridBaseColumns::SERVICE][ARow]);
}

// ---------------------------------------------------------------------------
TStringGridColService * StringGridGetColService(TStringGrid * Grid, int ACol) {
	return ((TStringGridColService*)Grid->Objects[ACol][0]);
}

// ---------------------------------------------------------------------------
void StringGridSetHeader(TStringGrid * Grid, int ACol, String ColName,
	int ColWidth) {
	Grid->Cells[ACol][0] = ColName;
	Grid->ColWidths[ACol] = ColWidth;
}

// ---------------------------------------------------------------------------
void StringGridSetHeader(TStringGrid * Grid, int ACol, NativeUInt ColNameIdent,
	int ColWidth) {
	StringGridSetHeader(Grid, ACol, LoadStr(ColNameIdent), ColWidth);
}

// ---------------------------------------------------------------------------
void StringGridSetCellInt(TStringGrid * Grid, int ACol, int ARow, int Value,
	bool ShowZero) {
	if (ShowZero) {
		Grid->Cells[ACol][ARow] = IntToStr(Value);
	}
	else {
		if (Value != 0) {
			Grid->Cells[ACol][ARow] = IntToStr(Value);
		}
		else {
			Grid->Cells[ACol][ARow] = "";
		}
	}
}

// ---------------------------------------------------------------------------
void StringGridSetCellChecked(TStringGrid * Grid, int ACol, int ARow,
	bool Value) {
	Grid->Cells[ACol][ARow] = Value ? "X" : "";
}

// ---------------------------------------------------------------------------
bool StringGridGetCellChecked(TStringGrid * Grid, int ACol, int ARow) {
	return !Grid->Cells[ACol][ARow].IsEmpty();
}

// ---------------------------------------------------------------------------
void StringGridDrawCell(TStringGrid * Grid, int ACol, int ARow, TRect Rect,
	TGridDrawState State, TStringGridOptions * Options) {

	Grid->Canvas->Font = Grid->Font;

	TStringGridRowService * RowService = StringGridGetRowService(Grid, ARow);
	TStringGridColService * ColService = StringGridGetColService(Grid, ACol);

	if (State.Contains(gdFixed)) {
		Grid->Canvas->Brush->Color = Grid->FixedColor;
	}
	else {
		if (State.Contains(gdSelected)) {
			if (State.Contains(gdFocused)) {
				Grid->Canvas->Brush->Color = clMedGray;
			}
			else {
				Grid->Canvas->Brush->Color = Options->DrawFocusedOnInactive ?
					clSilver : clWindow;
			}
		}
		else {
			if ((ColService != NULL && ColService->ReadOnly) ||
				(RowService != NULL && RowService->ReadOnly)) {
				Grid->Canvas->Brush->Color = Options->ColorReadOnly;
			}
			else {
				Grid->Canvas->Brush->Color = Grid->Color;
			}
		}
	}

	Grid->Canvas->FillRect(Rect);

	if (State.Contains(gdFixed)) {
		if (ARow > 0 && ACol == TStringGridBaseColumns::SERVICE && RowService !=
			NULL && RowService->Changed) {
			TRect ChangedRect = Rect;

			ChangedRect.Top = ChangedRect.Top + 2;
			ChangedRect.Bottom = ChangedRect.Bottom - 2;
			ChangedRect.Right = ChangedRect.Right - 2;
			ChangedRect.Left = ChangedRect.Right - 4;

			Grid->Canvas->Brush->Color = Options->ColorChanged;

			Grid->Canvas->FillRect(ChangedRect);

			Grid->Canvas->Brush->Color = Grid->FixedColor;
		}

		if (Options->ColorSelected != clMax && ARow == Grid->Row) {
			TRect ChangedRect = Rect;

			ChangedRect.Top = ChangedRect.Top + 2;
			ChangedRect.Bottom = ChangedRect.Bottom - 2;
			ChangedRect.Left = ChangedRect.Left + 2;
			ChangedRect.Right = ChangedRect.Left + 4;

			Grid->Canvas->Brush->Color = Options->ColorSelected;

			Grid->Canvas->FillRect(ChangedRect);

			Grid->Canvas->Brush->Color = Grid->FixedColor;
		}

		InflateRect(Rect, -2, 0);
		OffsetRect(Rect, -1, 0);

		DrawText(Grid->Canvas->Handle, Grid->Cells[ACol][ARow].c_str(),
			Grid->Cells[ACol][ARow].Length(), (RECT*)&Rect,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
	}
	else {
		InflateRect(Rect, -2, 0);

		UINT Alignment = DT_CENTER;

		if (ColService != NULL) {
			switch (ColService->Alignment) {
			case taLeftJustify:
				Alignment = DT_LEFT;
				break;
			case taRightJustify:
				Alignment = DT_RIGHT;
				break;
			}
		}

		DrawText(Grid->Canvas->Handle, Grid->Cells[ACol][ARow].c_str(),
			Grid->Cells[ACol][ARow].Length(), (RECT*)&Rect,
			DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | Alignment);
	}
}

// ---------------------------------------------------------------------------
void StringGridInvalidateCell(TStringGrid * Grid, int ACol, int ARow) {
	TRect Rect = Grid->CellRect(ACol, ARow);
	InvalidateRect(Grid->Handle, &Rect, false);
}

// ---------------------------------------------------------------------------
void StringGridInvalidateRow(TStringGrid * Grid, int ARow) {
	for (int ACol = 1; ACol < Grid->ColCount; ACol++) {
		StringGridInvalidateCell(Grid, ACol, ARow);
	}
}

// ---------------------------------------------------------------------------
void StringGridInvalidateSelected(TStringGrid * Grid) {
	for (int i = 1; i < Grid->RowCount; i++) {
		StringGridInvalidateCell(Grid, TStringGridBaseColumns::SERVICE, i);
	}
}

// ---------------------------------------------------------------------------
void StringGridMouseToCell(TStringGrid * Grid, int &ACol, int &ARow) {
	TPoint P = Grid->ScreenToClient(Mouse->CursorPos);

	Grid->MouseToCell(P.X, P.Y, ACol, ARow);
}

// ---------------------------------------------------------------------------
void StringGridSelectRowAfterFixedCellClick(TStringGrid * Grid, int ARow) {
	if (ARow < 1) {
		return;
	}

	if (StringGridIsEmpty(Grid)) {
		return;
	}

	Grid->Row = ARow;
}

// ---------------------------------------------------------------------------
void StringGridColWidthsWriteToIni(TStringGrid * Grid, TFileIni * FileIni,
	const String Section, const String Ident) {
	String S;

	for (int i = 1; i < Grid->ColCount; i++) {
		S = ConcatStrings(S, IntToStr(Grid->ColWidths[i]), COMMA);
	}

	FileIni->WriteString(Section, Ident, S);
}

// ---------------------------------------------------------------------------
void StringGridColWidthsReadFromIni(TStringGrid * Grid, TFileIni * FileIni,
	const String Section, const String Ident) {
	String Value;

	String S = FileIni->ReadString(Section, Ident, "");

	if (S.IsEmpty()) {
		return;
	}

	try {
		for (int i = 1; i < Grid->ColCount; i++) {
			SplitStr(S, COMMA, 0, Value, S);

			Grid->ColWidths[i] = StrToInt(Value);
		}
	}
	catch (...) {
	}
}
// ---------------------------------------------------------------------------
