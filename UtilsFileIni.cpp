// ---------------------------------------------------------------------------

#pragma hdrstop

#include <vcl.h>
#include <System.IniFiles.hpp>

#include "UtilsFileIni.h"
#include "UtilsStr.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

__fastcall TFileIni::TFileIni(const System::UnicodeString FileName)
	: TIniFile(FileName) {

}

TFileIni* TFileIni::GetNewInstance(System::UnicodeString FileName) {
	if (FileName == NULL)
		FileName = ChangeFileExt(Application->ExeName, ".ini");
	else {
		if (ExtractFileExt(FileName) == NULL)
			FileName = FileName + ".ini";
		// if (ExtractFilePath(FileName) == NULL) FileName = FileInAppDir(FileName);
	};
	return new TFileIni(FileName);
}

void __fastcall TFileIni::UpdateFile(void) {
	try {
		TIniFile::UpdateFile();
	}
	catch (...) {
	}
}

void __fastcall TFileIni::WriteString(const System::UnicodeString Section,
	const System::UnicodeString Ident, const System::UnicodeString Value) {
	try {
		TIniFile::WriteString(Section, Ident, Value);
	}
	catch (...) {
	}
}

TRect TFileIni::ReadRect(const String Section, const String Ident,
	TRect Default) {
	String RectStr;
	TRect Result;

	RectStr = ReadString(Section, Ident, NULL);

	if (RectStr == NULL)
		Result = Default;
	else {
		try {
			Result = StrToRect(RectStr);
		}
		catch (...) {
			Result = Default;
		}
	}
	return Result;
}

void TFileIni::WriteRect(const String Section, const String Ident, TRect Value)
{
	WriteString(Section, Ident, RectToStr(Value));
}

void TFileIni::ReadBounds(TControl* Control, const String Section,
	const String Ident, TRect Default) {
	TRect Rect = ReadRect(Section, Ident, Default);
	Control->SetBounds(Rect.Left, Rect.Top, Rect.Right, Rect.Bottom);
}

void TFileIni::WriteBounds(const TControl* Control, const String Section,
	const String Ident) {
	WriteRect(Section, Ident, Rect(Control->Left, Control->Top, Control->Width,
		Control->Height));
}

void TFileIni::ReadFormBounds(TForm* Form, String Section) {
	if (Section == NULL)
		Section = Form->Name;

	ReadBounds(Form, Section, "Position",
		Rect((Screen->Width - Form->Width) / 2,
		(Screen->Height - Form->Height) / 2, Form->Width, Form->Height));

	Form->MakeFullyVisible();

	if (ReadBool(Section, "Maximized", false))
		Form->WindowState = wsMaximized;
}

void TFileIni::WriteFormBounds(const TForm* Form, String Section) {
	if (Section == NULL)
		Section = Form->Name;
	if (Form->WindowState == wsMaximized)
		WriteBool(Section, "Maximized", true);
	else {
		WriteBool(Section, "Maximized", false);
		WriteBounds(Form, Section, "Position");
	}
}
