// ---------------------------------------------------------------------------

#ifndef CodeNamePairH
#define CodeNamePairH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

#include <ObjList.h>

// ---------------------------------------------------------------------------
class TCodeNamePair : public TObject {
private:
	int FCode;
	String FName;

public:
	__fastcall TCodeNamePair();
	__fastcall TCodeNamePair(int Code, String Name);
	__fastcall TCodeNamePair(TCodeNamePair * Value);

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TCodeNamePair * Source);
	String __fastcall ToString();

	// -----------------------------------------------------------------------
	__property int Code = {read = FCode, write = FCode};
	__property String Name = {read = FName, write = FName};
};

// ---------------------------------------------------------------------------
typedef TObjList<TCodeNamePair>TCodeNamePairList;

// ---------------------------------------------------------------------------
#endif
