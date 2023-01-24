// ---------------------------------------------------------------------------

#pragma hdrstop

#include "AboutFrm.h"
#include "AboutFrmStr.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#include <vcl.h>
#include <System.DateUtils.hpp>

#include "UtilsMisc.h"
#include "UtilsKAndM.h"
#include "UtilsStr.h"
#include "UtilsFiles.h"

#include "GradientPanel.h"

// ---------------------------------------------------------------------------
class TAboutObject : public TObject {
public:
	TLabel * lblMemoryValue, *lblProcValue;
	String MailAddress, MailSubject;

	TAboutObject() {
		MailAddress = NULL;
		MailSubject = NULL;
	}

	void __fastcall TimerTimer(TObject *Sender);

	void __fastcall MouseEnter(TObject *Sender);
	void __fastcall MouseLeave(TObject *Sender);

	void __fastcall Click(TObject *Sender);

	void __fastcall SetLabels(TLabel * lblMemoryValue, TLabel * lblProcValue) {
		this->lblMemoryValue = lblMemoryValue;
		this->lblProcValue = lblProcValue;
	}
};

void ShowAbout(int iFontSize, Byte bLineBreak, Byte bDatePos, Byte bVersionPos,
	String sDate, TPicture * pPicture, String sAddComp, String sVersion,
	String sAppName, String sCopyright, String sText, HICON hIcon) {
	int I;
	String sCaption;

	TVSFixedFileInfo FileVersionInfo;

	String CompanyName, FileDescription, FileVersion, InternalName,
		LegalCopyright, OriginalFilename, ProductName, ProductVersion;

	ShowWaitCursor();

	TAboutObject * AboutObject = new TAboutObject();

	if (IsShift() && IsCtrl() && IsAlt()) {
		sAppName = "Дураев Константин Петрович";
		bLineBreak = 0;
		sCopyright = "Рандом говорит нам:";
		sCaption = "Автор";
		sDate = "";
		sVersion = "29.03.1981";
		iFontSize = 14;
		sAddComp = NULL;
		pPicture = NULL;
		sText = RandomStr("", 40);
		for (int i = 0; i < 9; i++) {
			sText = sText + sLineBreak + RandomStr("", 40);
		}
	}
	else {
		GetFileVerInfo(Application->ExeName, FileVersionInfo, CompanyName,
			FileDescription, FileVersion, InternalName, LegalCopyright,
			OriginalFilename, ProductName, ProductVersion);

		sCaption = LoadStr(IDS_ABOUT_CAPTION);

		if (sDate == NULL) {
			sDate = FormatDateTime("yyyy.mm.dd",
				UnixToDateTime(((PIMAGE_NT_HEADERS)
				((DWORD)((PIMAGE_DOS_HEADER) HInstance) +
				(((PIMAGE_DOS_HEADER) HInstance)->e_lfanew)))
				->FileHeader.TimeDateStamp));
		}
		if (sVersion == NULL) {
			sVersion = FileVersion;
			if (IsValueInWord(FileVersionInfo.dwFileFlags, VS_FF_DEBUG)) {
				sVersion = sVersion + LoadStr(IDS_ABOUT_DEBUG);
			}
			else {
				if (IsValueInWord(FileVersionInfo.dwFileFlags,
				VS_FF_PRERELEASE)) {
					sVersion = sVersion + LoadStr(IDS_ABOUT_RC);
				}
			}
		}

		if (sAppName == NULL) {
			sAppName = ProductName;
		}

		if (sCopyright == NULL) {
			sCopyright = LegalCopyright + "|@P3tr0viCh@yandex.ru";
		}

		if (sText == NULL) {
			sText = LoadStr(IDS_ABOUT_EULA_1_1) + sLineBreak +
				LoadStr(IDS_ABOUT_EULA_2) + sLineBreak +
				LoadStr(IDS_ABOUT_EULA_3) + sLineBreak +
				LoadStr(IDS_ABOUT_EULA_4);
		}
		else {
			if (SameStr(sText, "LIC_#2")) {
				sText = LoadStr(IDS_ABOUT_EULA_1_2) + sLineBreak +
					LoadStr(IDS_ABOUT_EULA_2) + sLineBreak +
					LoadStr(IDS_ABOUT_EULA_3) + sLineBreak +
					LoadStr(IDS_ABOUT_EULA_4);
			}
		}
	}

	if (bLineBreak != MAXBYTE) {
		I = PosPlace(SPACE, sAppName, bLineBreak);
		sAppName = sAppName.SubString(1, I - 1) + sLineBreak +
			sAppName.SubString(I + 1, MAXINT);
	}

	if (hIcon == NULL) {
		hIcon = LoadIcon(HInstance, L"MAINICON");
	}

	TForm * Form = new TForm(Application); // Owner;
	Form->ShowHint = true;
	Form->Font->Name = "Segoe UI";
	Form->Font->Size = 10;
	Form->BorderStyle = bsDialog;
	Form->Caption = sCaption;
	Form->ClientHeight = 168;
	Form->ClientWidth = 420;

	TPanel * pnlIcon = new TPanel(Form);
	pnlIcon->Parent = Form;
	pnlIcon->SetBounds(8, 8, 64, 64);
	pnlIcon->Caption = "";
	pnlIcon->BevelOuter = bvNone;
	pnlIcon->BorderStyle = bsSingle;
	pnlIcon->ParentBackground = false;
	pnlIcon->Color = (TColor) Random(0xFFFFFF);

	TImage * imgIcon = new TImage(Form); // Icon
	imgIcon->Parent = pnlIcon;
	imgIcon->Align = alClient;
	imgIcon->Transparent = true;
	imgIcon->Center = true;
	imgIcon->Picture->Icon->Handle = hIcon;

	TLabel * lblCopyright = new TLabel(Form); // Copyright
	lblCopyright->Tag = 1;
	lblCopyright->Parent = Form;
	lblCopyright->Font->Style = TFontStyles() << fsBold;
	lblCopyright->SetBounds(8, 80, 0, 0);

	int PosHint = Pos('|', sCopyright);
	if (PosHint == 0) {
		lblCopyright->Caption = sCopyright;
	}
	else {
		lblCopyright->Caption = sCopyright.SubString(1, PosHint - 1);
		lblCopyright->Hint = sCopyright.SubString(PosHint + 1, MAXINT);
		lblCopyright->Cursor = crHandPoint;
		lblCopyright->OnMouseEnter = AboutObject->MouseEnter;
		lblCopyright->OnMouseLeave = AboutObject->MouseLeave;
		if (lblCopyright->Hint[1] == '@') {
			lblCopyright->Hint = lblCopyright->Hint.SubString(2, MAXINT);

			AboutObject->MailAddress = lblCopyright->Hint;
			AboutObject->MailSubject = OriginalFilename + " " + FileVersion;
		}
		lblCopyright->OnClick = AboutObject->Click;
	}

	TLabel * lblText = new TLabel(Form); // Text
	lblText->Parent = Form;
	lblText->WordWrap = true;
	lblText->SetBounds(8, 100, Form->ClientWidth - 16, 0);
	lblText->Caption = sText;
	// I =
	lblText->Top + lblText->Height;
	Form->ClientHeight = Form->ClientHeight + lblText->Height;

	if (sAddComp != NULL) {
		// TODO
		/* with TBevel.Create(Form) do
		 {
		 Parent = Form;
		 SetBounds(7, I + 5, Form.ClientWidth - 15, 5);
		 Shape = bsTopLine;
		 I = Top + Height;
		 };

		 with TLabel.Create(Form) do // Add Components
		 {
		 Parent = Form;
		 SetBounds(8, I, 0, 0);
		 Caption = 'Программное обеспечение использует следующие компоненты:';
		 I = Top + Height;
		 };

		 with TLabel.Create(Form) do // Components
		 {
		 Tag = 2;
		 Parent = Form;
		 Font.Style = [fsBold];
		 SetBounds(8, I, 0, 0);
		 Caption = sAddComp;
		 Form.ClientHeight = Form.ClientHeight + Height + 12;
		 if FileExists(FileInAppDir(rsLicenses)) then
		 {
		 Cursor = crHandPoint;
		 OnClick = ClickObject.Click;
		 OnMouseEnter = ClickObject.MouseEnter;
		 OnMouseLeave = ClickObject.MouseLeave;
		 };
		 } */ }
	else { // sAddComp != NULL
		Form->ClientHeight = Form->ClientHeight - 12;
	}

	TGradientPanel * pnlName = new TGradientPanel(Form);
	pnlName->Parent = Form;
	pnlName->StartUpdate();
	pnlName->SetBounds(80, 8, Form->ClientWidth - 88, 64);
	pnlName->BorderStyle = bsSingle;

	pnlName->BevelOuter = bvNone;
	pnlName->ParentBackground = false;
	if (pPicture == NULL) {
		pnlName->ColorStart = pnlIcon->Color;
	}
	else {
		pnlName->ColorStart = clBlack;
	}
	pnlName->ColorEnd = clBlack;
	pnlName->EndUpdate();

	if (pPicture == NULL) {
		TLabel * lblAppName = new TLabel(Form); // Application Name (1)
		lblAppName->Parent = pnlName;
		lblAppName->Alignment = taCenter;
		lblAppName->AutoSize = false;
		lblAppName->Caption = sAppName;
		lblAppName->ParentColor = false;
		lblAppName->Color = clBlack;
		lblAppName->ParentFont = false;
		lblAppName->Font->Charset = DEFAULT_CHARSET;
		lblAppName->Font->Color = clBlack;
		lblAppName->Font->Height = -51;
		lblAppName->Font->Name = "Segoe UI";
		lblAppName->Font->Style = TFontStyles() << fsBold << fsItalic;
		lblAppName->Font->Size = iFontSize;
		lblAppName->Transparent = true;
		lblAppName->Layout = tlCenter;
		lblAppName->WordWrap = true;
		lblAppName->SetBounds(4, 4, pnlName->ClientWidth - 4,
			pnlName->ClientHeight - 4);

		TLabel * lblAppName2 = new TLabel(Form); // Application Name (2)
		lblAppName2->Parent = pnlName;
		lblAppName2->Alignment = taCenter;
		lblAppName2->AutoSize = false;
		lblAppName2->Caption = sAppName;
		lblAppName2->ParentColor = false;
		lblAppName2->Color = clBlack;
		lblAppName2->ParentFont = false;
		lblAppName2->Font->Charset = DEFAULT_CHARSET;
		lblAppName2->Font->Color = clWhite;
		lblAppName2->Font->Height = -51;
		lblAppName2->Font->Name = "Segoe UI";
		lblAppName2->Font->Style = TFontStyles() << fsBold << fsItalic;
		lblAppName2->Font->Size = iFontSize;
		lblAppName2->Transparent = true;
		lblAppName2->Layout = tlCenter;
		lblAppName2->WordWrap = true;
		lblAppName2->SetBounds(0, 0, pnlName->ClientWidth,
			pnlName->ClientHeight);
	} // pPicture == NULL
	else {
		TImage * imgPicture = new TImage(Form);
		imgPicture->Parent = pnlName;
		imgPicture->Align = alClient;
		imgPicture->Picture->Assign(pPicture);
	}

	TLabel * lblDate = new TLabel(Form); // Date
	lblDate->Parent = pnlName;
	lblDate->Caption = sDate;
	lblDate->Font->Assign(Form->Font);
	lblDate->Font->Style = TFontStyles() << fsBold;
	lblDate->Font->Color = clWhite;
	lblDate->Font->Size = 8;
	lblDate->Alignment = taRightJustify;

	switch (bDatePos) {
	case 0:
		lblDate->SetBounds(2, 2, lblDate->Width, lblDate->Height);
		break;
	case 1:
		lblDate->SetBounds(pnlName->ClientWidth - lblDate->Width - 2, 2,
			lblDate->Width, lblDate->Height);
		break;
	case 2:
		lblDate->SetBounds(2, pnlName->ClientHeight - lblDate->Height - 2,
			lblDate->Width, lblDate->Height);
		break;
	default:
		lblDate->SetBounds(pnlName->ClientWidth - lblDate->Width - 2,
			pnlName->ClientHeight - lblDate->Height - 2, lblDate->Width,
			lblDate->Height);
	}
	lblDate->Transparent = true;

	TLabel * lblVerion = new TLabel(Form); // Version
	lblVerion->Parent = pnlName;
	lblVerion->Caption = sVersion;
	lblVerion->Font->Assign(Form->Font);
	lblVerion->Font->Style = TFontStyles() << fsBold;
	lblVerion->Font->Color = clWhite;
	lblVerion->Font->Size = 8;

	switch (bVersionPos) {
	case 0:
		lblVerion->SetBounds(2, 2, lblVerion->Width, lblVerion->Height);
		break;
	case 2:
		lblVerion->SetBounds(2, pnlName->ClientHeight - lblVerion->Height - 2,
			lblVerion->Width, lblVerion->Height);
		break;
	case 3:
		lblVerion->SetBounds(pnlName->ClientWidth - lblVerion->Width - 2,
			pnlName->ClientHeight - lblVerion->Height - 2, lblVerion->Width,
			lblVerion->Height);
		break;
	default:
		lblVerion->SetBounds(pnlName->ClientWidth - lblVerion->Width - 2, 2,
			lblVerion->Width, lblVerion->Height);
	}
	lblVerion->Transparent = true;

	TBevel * bvlBottom = new TBevel(Form);
	bvlBottom->Parent = Form;
	bvlBottom->SetBounds(8, Form->ClientHeight - 48, Form->ClientWidth - 16, 4);
	bvlBottom->Shape = bsTopLine;

	TLabel * lblMemory = new TLabel(Form); // Физическая ...
	lblMemory->Parent = Form;
	lblMemory->SetBounds(8, Form->ClientHeight - 40, 0, 0);
	lblMemory->Caption = LoadStr(IDS_ABOUT_MEMORY);

	TLabel * lblMemoryValue = new TLabel(Form); // TotalPhys
	lblMemoryValue->Parent = Form;
	lblMemoryValue->Font->Style = TFontStyles() << fsBold;
	lblMemoryValue->SetBounds(lblMemory->Width + 16,
		Form->ClientHeight - 40, 0, 0);

	TLabel * lblProc = new TLabel(Form); // Процессор:
	lblProc->Parent = Form;
	lblProc->SetBounds(8, Form->ClientHeight - 22, 0, 0);
	lblProc->Caption = LoadStr(IDS_ABOUT_PROCESSOR);

	TLabel * lblProcValue = new TLabel(Form); // CPUSpeed
	lblProcValue->Parent = Form;
	lblProcValue->Font->Style = TFontStyles() << fsBold;
	lblProcValue->SetBounds(lblMemory->Width + 16,
		Form->ClientHeight - 22, 0, 0);

	TButton * btnClose = new TButton(Form);
	btnClose->Parent = Form;
	btnClose->Caption = "OK";
	btnClose->ModalResult = mrCancel;
	btnClose->Cancel = true;
	btnClose->SetBounds(Form->ClientWidth - 88, Form->ClientHeight - 40,
		80, 32);
	btnClose->TabOrder = 0;

	AboutObject->SetLabels(lblMemoryValue, lblProcValue);

	TTimer * Timer = new TTimer(Form);
	Timer->OnTimer = AboutObject->TimerTimer;
	Timer->Interval = 1;
	Timer->Enabled = true;

	RestoreCursor();

	Form->Left = (Screen->Width - Form->Width) / 2;
	Form->Top = (Screen->Height - Form->Height) / 2;

	Form->ShowModal();

	Form->Free();

	AboutObject->Free();
}

void __fastcall TAboutObject::TimerTimer(TObject * Sender) {
	((TTimer*) Sender)->Enabled = false;
	lblMemoryValue->Caption = GetTotalPhys();
	lblProcValue->Caption = GetCPUSpeed();
}

void __fastcall TAboutObject::MouseEnter(TObject * Sender) {
	((TLabel*) Sender)->Font->Color = clHotLight;
	((TLabel*) Sender)->Font->Style =
		((TLabel*) Sender)->Font->Style << fsUnderline;
}

void __fastcall TAboutObject::MouseLeave(TObject * Sender) {
	((TLabel*) Sender)->Font->Color = clWindowText;
	((TLabel*) Sender)->Font->Style =
		((TLabel*) Sender)->Font->Style >> fsUnderline;
}

void __fastcall TAboutObject::Click(TObject * Sender) {
	ShowWaitCursor();

	switch (((TControl*) Sender)->Tag) {
	case 1:
		if (MailAddress.IsEmpty()) {
			MsgBox(((TControl*) Sender)->Hint);
		}
		else {
			OKExec("mailto:" + MailAddress + "?subject=" + MailSubject, "");
		}
		break;

	case 2:
		OKExec(FileInAppDir(LoadStr(IDS_ABOUT_LICENSES)));
	}

	RestoreCursor();
}
