// ---------------------------------------------------------------------------

#pragma hdrstop

#include "UtilsStringGrid.h"

#include "UtilsStr.h"
#include "UtilsLog.h"

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
__fastcall TStringGridCellService::TStringGridCellService(TStringGrid * Grid) {
	FGrid = Grid;

	FColor = Grid->Color;
}

// ---------------------------------------------------------------------------
void TStringGridOptions::Init() {
	FColorReadOnly = TColor(0x00E8E8E8);
	FColorMarkChanged = TColor(0x000085FB);
	FColorMarkSelected = clHotLight;
	FColorRowSelected = TColor(0x00D0D0D0);
	FColorCellFocused = TColor(0x00A0A0A0);
	FColorCellSelected = TColor(0x00C0C0C0);

	FDrawFocusedOnInactive = true;
	FDrawRowSelected = false;
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
	for (int ARow = 1; ARow < Grid->RowCount; ARow++) {
		for (int ACol = 0; ACol < Grid->ColCount; ACol++) {
			Grid->Objects[ACol][ARow]->Free();
		}

		Grid->Rows[ARow]->Clear();
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
TStringGridCellService * StringGridGetCellService(TStringGrid * Grid, int ACol,
	int ARow) {
	return ((TStringGridCellService*)Grid->Objects[ACol][ARow]);
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
	TStringGridCellService * CellService =
		StringGridGetCellService(Grid, ACol, ARow);

	TColor CellColor = Grid->Color;

	if (CellService != NULL) {
		CellColor = CellService->Color;
	}

	if (State.Contains(gdFixed)) {
		Grid->Canvas->Brush->Color = Grid->FixedColor;
	}
	else {
		if (StringGridIsEmpty(Grid)) {
			Grid->Canvas->Brush->Color = Grid->Color;
		}
		else {
			if (State.Contains(gdSelected)) {
				if (Grid->Focused()) {
					if (State.Contains(gdFocused)) {
						Grid->Canvas->Brush->Color = clMedGray;
					}
					else {
						Grid->Canvas->Brush->Color = clSilver;
					}
				}
				else {
					if (Options->DrawFocusedOnInactive) {
						if (State.Contains(gdFocused)) {
							Grid->Canvas->Brush->Color = clMedGray;
						}
						else {
							Grid->Canvas->Brush->Color = clSilver;
						}
					}
					else {
						if ((ColService != NULL && ColService->ReadOnly) ||
							(RowService != NULL && RowService->ReadOnly)) {
							Grid->Canvas->Brush->Color = Options->ColorReadOnly;
						}
						else {
							Grid->Canvas->Brush->Color = CellColor;
						}
					}
				}
			}
			else {
				if (Options->DrawRowSelected && ARow == Grid->Row) {
					if (Grid->Focused()) {
						Grid->Canvas->Brush->Color = Options->ColorRowSelected;
					}
					else {
						if (Options->DrawFocusedOnInactive) {
							Grid->Canvas->Brush->Color =
								Options->ColorRowSelected;
						}
						else {
							if ((ColService != NULL && ColService->ReadOnly) ||
								(RowService != NULL && RowService->ReadOnly)) {
								Grid->Canvas->Brush->Color =
									Options->ColorReadOnly;
							}
							else {
								Grid->Canvas->Brush->Color = CellColor;
							}
						}
					}
				}
				else {
					if ((ColService != NULL && ColService->ReadOnly) ||
						(RowService != NULL && RowService->ReadOnly)) {
						Grid->Canvas->Brush->Color = Options->ColorReadOnly;
					}
					else {
						Grid->Canvas->Brush->Color = CellColor;
					}
				}
			}
		}
	}

	Grid->Canvas->FillRect(Rect);

	if (State.Contains(gdFixed)) {
		if (!StringGridIsEmpty(Grid)) {
			if (ARow > 0 && ACol == TStringGridBaseColumns::SERVICE &&
				RowService != NULL && RowService->Changed) {
				TRect ChangedRect = Rect;

				ChangedRect.Top = ChangedRect.Top + 2;
				ChangedRect.Bottom = ChangedRect.Bottom - 2;
				ChangedRect.Right = ChangedRect.Right - 2;
				ChangedRect.Left = ChangedRect.Right - 4;

				Grid->Canvas->Brush->Color = Options->ColorMarkChanged;

				Grid->Canvas->FillRect(ChangedRect);

				Grid->Canvas->Brush->Color = Grid->FixedColor;
			}

			if (ARow == Grid->Row) {
				TRect ChangedRect = Rect;

				ChangedRect.Top = ChangedRect.Top + 2;
				ChangedRect.Bottom = ChangedRect.Bottom - 2;
				ChangedRect.Left = ChangedRect.Left + 2;
				ChangedRect.Right = ChangedRect.Left + 4;

				Grid->Canvas->Brush->Color = Options->ColorMarkSelected;

				Grid->Canvas->FillRect(ChangedRect);

				Grid->Canvas->Brush->Color = Grid->FixedColor;
			}
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
	for (int ACol = 0; ACol < Grid->ColCount; ACol++) {
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
void StringGridSelectAll(TStringGrid * Grid) {
	TGridRect Selection;

	Selection.Left = Grid->ColCount - 1;
	Selection.Right = 1;

	Selection.Top = Grid->RowCount - 1;
	Selection.Bottom = 1;

	Grid->Selection = Selection;

	Grid->Repaint();
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
