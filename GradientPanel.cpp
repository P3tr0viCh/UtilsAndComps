// ---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "GradientPanel.h"

#include "UtilsMisc.h"

#pragma package(smart_init)
// ---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TGradientPanel *) {
	new TGradientPanel(NULL);
}

// ---------------------------------------------------------------------------
__fastcall TGradientPanel::TGradientPanel(TComponent* Owner)
	: TCustomPanel(Owner) {
	StartUpdate();
	Caption = Name;
	SetBounds(Left, Top, 200, 100);
	FColorStart = clActiveCaption;
	FColorEnd   = clInactiveCaption;
	// FTextPosition = tpTopLeft;
	FFillDirection = fdLeftToRight;
	Font->Name     = "Tahoma";
	Font->Size     = 18;
	Font->Color    = clActiveCaption;
	EndUpdate();
}

// ---------------------------------------------------------------------------
namespace Gradientpanel {
	void __fastcall PACKAGE Register() {
		TComponentClass classes[1] = {__classid(TGradientPanel)};
		RegisterComponents(L"P3tr0viCh", classes, 0);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TGradientPanel::StartUpdate() {
	FNotPaint = true;
}

void __fastcall TGradientPanel::EndUpdate() {
	FNotPaint = false;
}

void __fastcall TGradientPanel::SetColorEnd(TColor Color) {
	FColorEnd = Color;
	Invalidate();
}

void __fastcall TGradientPanel::SetFillDirection(TFillDirection FillDirection) {
	FFillDirection = FillDirection;
	Invalidate();
}

void __fastcall TGradientPanel::SetColorStart(TColor Color) {
	FColorStart = Color;
	Invalidate();
}

void GradientFill(TCanvas *ACanvas, TColor StartColor, TColor EndColor,
	int ALeft, int ATop, int AWidth, int AHeight, TFillDirection FillDirection)
{
	int C1, C2, R, G, B;
	double DR, DG, DB, DH;

	ACanvas->Brush->Style = bsSolid;

	if (FillDirection == fdLeftToRight | FillDirection == fdTopToBottom) {
		C1 = ColorToRGB(StartColor);
		C2 = ColorToRGB(EndColor);
	}
	else {
		C1 = ColorToRGB(EndColor);
		C2 = ColorToRGB(StartColor);
	}

	R = GetRValue(C1);
	G = GetGValue(C1);
	B = GetBValue(C1);

	DR = (double)(GetRValue(C2) - R) / 256;
	DG = (double)(GetGValue(C2) - G) / 256;
	DB = (double)(GetBValue(C2) - B) / 256;

	if (FillDirection == fdTopToBottom | FillDirection == fdBottomToTop) {

		DH = (double)(AHeight - ATop) / 256;

		for (int X = 0; X < 256; X++) {
			ACanvas->Brush->Color = (TColor) RGB(R + Round(DR * X), G + Round(DG * X),
				B + Round(DB * X));
			ACanvas->FillRect(Rect(ALeft, ATop + Round(X * DH), AWidth,
				ATop + Round((X + 1) * DH)));
		}
	}
	else {

		DH = (double)(AWidth - ALeft) / 256;

		for (int X = 0; X < 256; X++) {
			ACanvas->Brush->Color = (TColor) RGB(R + Round(DR * X), G + Round(DG * X),
				B + Round(DB * X));
			ACanvas->FillRect(Rect(ALeft + Round(X * DH), ATop,
				ALeft + Round((X + 1) * DH), AHeight));
		}
	}
}

void __fastcall TGradientPanel::Paint(void) {
	if (FNotPaint)
		return;
	if (FColorStart == FColorEnd) {
		Canvas->Brush->Color = FColorStart;
		Canvas->FillRect(Rect(0, 0, Width, Height));
	}
	else
		GradientFill(Canvas, FColorStart, FColorEnd, 0, 0, Width, Height,
		FFillDirection);
}
