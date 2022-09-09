// ---------------------------------------------------------------------------

#ifndef UtilsFileIniH
#define UtilsFileIniH

#include <Vcl.Controls.hpp>
#include <Vcl.Forms.hpp>
#include <System.IniFiles.hpp>

// ---------------------------------------------------------------------------
class TFileIni : public TIniFile {
public:
	__fastcall TFileIni(const System::UnicodeString FileName);

	static TFileIni* GetNewInstance(System::UnicodeString FileName = "");

	void __fastcall UpdateFile(void);
	void __fastcall WriteString(const System::UnicodeString Section,
		const System::UnicodeString Ident, const System::UnicodeString Value);

	TRect ReadRect(const String Section, const String Ident, TRect Default);
	void WriteRect(const String Section, const String Ident, TRect Value);

	TPoint ReadPoint(const String Section, const String Ident, TPoint Default);
	void WritePoint(const String Section, const String Ident, TPoint Value);

	void ReadFont(const String Section, const String Ident, TFont * Value);
	void WriteFont(const String Section, const String Ident, TFont * Value);

	void ReadBounds(TControl * Control, const String Section, const String Ident,
		TRect Default);
	void WriteBounds(const TControl * Control, const String Section,
		const String Ident);

	void ReadPosition(TControl * Control, const String Section,
		const String Ident);
	void WritePosition(const TControl * Control, const String Section,
		const String Ident);

	void ReadFormBounds(TForm * Form, String Section = "");
	void WriteFormBounds(const TForm * Form, String Section = "");

	void ReadFormPosition(TForm * Form, String Section = "");
	void WriteFormPosition(const TForm * Form, String Section = "");
};

// ---------------------------------------------------------------------------
#endif
