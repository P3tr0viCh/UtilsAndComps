// ---------------------------------------------------------------------------

#pragma hdrstop

#include "UtilsStringGrid.h"

#include "UtilsStr.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
void StringGridInit(TStringGrid * Grid, TStringGridBaseColumns * Columns,
	int DefaultRowHeight) {
	Grid->DefaultRowHeight = DefaultRowHeight;

	Grid->ColCount = Columns->Count;

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

	int Index = Grid->RowCount - 1;

	Grid->Objects[TStringGridBaseColumns::SERVICE][Index] =
		new TStringGridService();

	return Index;
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
void StringGridUpdateOrderNum(TStringGrid * Grid, int StartValue) {
	for (int ARow = 1, Count = Grid->RowCount; ARow < Count; ARow++) {
		Grid->Cells[0][ARow] = IntToStr(StartValue);
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
TStringGridService * StringGridRowGetService(TStringGrid * Grid, int Index) {
	return ((TStringGridService*)Grid->Objects[TStringGridBaseColumns::SERVICE]
		[Index]);
}

// ---------------------------------------------------------------------------
void StringGridRowSetChanged(TStringGrid * Grid, int Index, bool Changed) {
	TStringGridService * Service = StringGridRowGetService(Grid, Index);

	if (Service == NULL) {
		return;
	}

	if (Service->Changed == Changed) {
		return;
	}

	Service->Changed = Changed;

	StringGridInvalidateCell(Grid, TStringGridBaseColumns::SERVICE, Index);
}

// ---------------------------------------------------------------------------
bool StringGridRowIsChanged(TStringGrid * Grid, int Index) {
	TStringGridService * Service = StringGridRowGetService(Grid, Index);

	if (Service == NULL) {
		return false;
	}

	return Service->Changed;
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
void StringGridDrawCell(TStringGrid * Grid, int ACol, int ARow, TRect Rect,
	TGridDrawState State, TIntegerSet ColsReadOnly, TIntegerSet ColsLeftAlign,
	TIntegerSet ColsCustomColor, TColor ReadOnlyColor, TColor CustomColor,
	bool DrawFocusedOnInactive, bool ReadOnlyRow, TColor ChangedColor,
	bool DrawSelectedRow, TColor SelectedRowColor) {

	Grid->Canvas->Font = Grid->Font;

	if (State.Contains(gdFixed)) {
		Grid->Canvas->Brush->Color = Grid->FixedColor;
	}
	else {
		if (State.Contains(gdSelected)) {
			if (State.Contains(gdFocused)) {
				Grid->Canvas->Brush->Color = clMedGray;
			}
			else {
				Grid->Canvas->Brush->Color = DrawFocusedOnInactive ? clSilver :
					clWindow;
			}
		}
		else {
			if (ColsCustomColor.Contains(ACol)) {
				Grid->Canvas->Brush->Color = CustomColor;
			}
			else {
				if (ColsReadOnly.Contains(ACol) || ReadOnlyRow) {
					Grid->Canvas->Brush->Color = ReadOnlyColor;
				}
				else {
					Grid->Canvas->Brush->Color = Grid->Color;
				}
			}
		}
	}

	Grid->Canvas->FillRect(Rect);

	if (State.Contains(gdFixed)) {
		if (ARow > 0 && ACol == TStringGridBaseColumns::SERVICE &&
			StringGridRowIsChanged(Grid, ARow)) {
			TRect ChangedRect = Rect;

			ChangedRect.Top = ChangedRect.Top + 2;
			ChangedRect.Bottom = ChangedRect.Bottom - 2;
			ChangedRect.Right = ChangedRect.Right - 2;
			ChangedRect.Left = ChangedRect.Right - 4;

			Grid->Canvas->Brush->Color = ChangedColor;

			Grid->Canvas->FillRect(ChangedRect);

			Grid->Canvas->Brush->Color = Grid->FixedColor;
		}

		if (DrawSelectedRow && ARow == Grid->Row) {
			TRect ChangedRect = Rect;

			ChangedRect.Top = ChangedRect.Top + 2;
			ChangedRect.Bottom = ChangedRect.Bottom - 2;
			ChangedRect.Left = ChangedRect.Left + 2;
			ChangedRect.Right = ChangedRect.Left + 4;

			Grid->Canvas->Brush->Color = SelectedRowColor;

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

		if (!State.Contains(gdSelected)) {
			if (ColsCustomColor.Contains(ACol)) {
				// TODO
				// Grid->Canvas->Font->Color =
				// GetColorByBack(Grid->Canvas->Brush->Color);
			}
		}

		if (ColsLeftAlign.Contains(ACol)) {
			DrawText(Grid->Canvas->Handle, Grid->Cells[ACol][ARow].c_str(),
				Grid->Cells[ACol][ARow].Length(), (RECT*)&Rect,
				DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER);
		}
		else {
			DrawText(Grid->Canvas->Handle, Grid->Cells[ACol][ARow].c_str(),
				Grid->Cells[ACol][ARow].Length(), (RECT*)&Rect,
				DT_SINGLELINE | DT_END_ELLIPSIS | DT_CENTER | DT_VCENTER);
		}
	}
}

// ---------------------------------------------------------------------------
void StringGridInvalidateCell(TStringGrid * Grid, int ACol, int ARow) {
	TRect Rect = Grid->CellRect(ACol, ARow);
	InvalidateRect(Grid->Handle, &Rect, false);
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

	if (IsEmpty(S)) {
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
