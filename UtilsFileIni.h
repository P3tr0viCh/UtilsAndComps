//---------------------------------------------------------------------------

#ifndef UtilsFileIniH
#define UtilsFileIniH
//---------------------------------------------------------------------------
#endif

#include <System.IniFiles.hpp>

class TFileIni : public TIniFile
{
public:
	__fastcall TFileIni(const System::UnicodeString FileName);

	void __fastcall UpdateFile(void);
	void __fastcall WriteString(const System::UnicodeString Section,
		const System::UnicodeString Ident, const System::UnicodeString Value);

	TRect ReadRect(const String Section, const String Ident, TRect Default);
	void WriteRect(const String Section, const String Ident, TRect Value);

	void ReadBounds(TControl* Control, const String Section,
		const String Ident, TRect Default);
	void WriteBounds(const TControl* Control, const String Section,
		const String Ident);

	void ReadFormBounds(TForm* Form, String Section = NULL);
	void WriteFormBounds(const TForm* Form, String Section = NULL);

};

TFileIni* CreateINIFile(System::UnicodeString FileName = NULL);

