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
	if (FileName.IsEmpty()) {
		FileName = ChangeFileExt(Application->ExeName, ".ini");
	}
	else {
		if (ExtractFileExt(FileName).IsEmpty()) {
			FileName = FileName + ".ini";
		}
	}

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
	String RectStr = ReadString(Section, Ident, NULL);

	if (RectStr.IsEmpty()) {
		return Default;
	}
	else {
		try {
			return StrToRect(RectStr);
		}
		catch (...) {
			return Default;
		}
	}
}

void TFileIni::WriteRect(const String Section, const String Ident, TRect Value)
{
	WriteString(Section, Ident, RectToStr(Value));
}

TPoint TFileIni::ReadPoint(const String Section, const String Ident,
	TPoint Default) {
	String PointStr = ReadString(Section, Ident, NULL);

	if (PointStr.IsEmpty()) {
		return Default;
	}
	else {
		try {
			return StrToPoint(PointStr);
		}
		catch (...) {
			return Default;
		}
	}
}

void TFileIni::WritePoint(const String Section, const String Ident,
	TPoint Value) {
	WriteString(Section, Ident, PointToStr(Value));
}

void TFileIni::ReadFont(const String Section, const String Ident, TFont * Value)
{
	try {
		StrToFont(ReadString(Section, Ident, NULL), Value);
	}
	catch (...) {
	}
}

void TFileIni::WriteFont(const String Section, const String Ident,
	TFont * Value) {
	WriteString(Section, Ident, FontToStr(Value));
}

void TFileIni::ReadBounds(TControl * Control, const String Section,
	const String Ident, TRect Default) {
	TRect Rect = ReadRect(Section, Ident, Default);
	Control->SetBounds(Rect.Left, Rect.Top, Rect.Right, Rect.Bottom);
}

void TFileIni::WriteBounds(const TControl * Control, const String Section,
	const String Ident) {
	WriteRect(Section, Ident, Rect(Control->Left, Control->Top, Control->Width,
		Control->Height));
}

void TFileIni::ReadPosition(TControl * Control, const String Section,
	const String Ident) {
	TPoint Point = ReadPoint(Section, Ident,
		TPoint(Control->Left, Control->Top));
	Control->SetBounds(Point.X, Point.Y, Control->Width, Control->Height);
}

void TFileIni::WritePosition(const TControl * Control, const String Section,
	const String Ident) {
	WritePoint(Section, Ident, TPoint(Control->Left, Control->Top));
}

void TFileIni::ReadFormBounds(TForm * Form, String Section) {
	if (Section.IsEmpty()) {
		Section = Form->Name;
	}

	ReadBounds(Form, Section, "Position",
		Rect((Screen->Width - Form->Width) / 2,
		(Screen->Height - Form->Height) / 2, Form->Width, Form->Height));

	Form->MakeFullyVisible();

	if (ReadBool(Section, "Maximized", false)) {
		Form->WindowState = wsMaximized;
	}
}

void TFileIni::WriteFormBounds(const TForm * Form, String Section) {
	if (Section.IsEmpty()) {
		Section = Form->Name;
	}

	if (Form->WindowState == wsMaximized) {
		WriteBool(Section, "Maximized", true);
	}
	else {
		WriteBool(Section, "Maximized", false);
		WriteBounds(Form, Section, "Position");
	}
}

void TFileIni::ReadFormPosition(TForm * Form, String Section) {
	if (Section.IsEmpty()) {
		Section = Form->Name;
	}

	Form->SetBounds((Screen->Width - Form->Width) / 2,
		(Screen->Height - Form->Height) / 2, Form->Width, Form->Height);

	ReadPosition(Form, Section, "Position");

	Form->MakeFullyVisible();
}

void TFileIni::WriteFormPosition(const TForm * Form, String Section) {
	if (Section.IsEmpty()) {
		Section = Form->Name;
	}

	WritePosition(Form, Section, "Position");
}
