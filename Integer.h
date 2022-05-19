//---------------------------------------------------------------------------

#ifndef IntegerH
#define IntegerH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

#include <ObjList.h>

// ---------------------------------------------------------------------------
class TInteger : public TObject {
private:
	int FValue;

	// -----------------------------------------------------------------------
	void Init();

public:
	__fastcall TInteger();
	__fastcall TInteger(int Value);
	__fastcall ~TInteger();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TInteger * Source);
	String __fastcall ToString();

	// -----------------------------------------------------------------------
	__property int Value = {read = FValue, write = FValue};
};

// ---------------------------------------------------------------------------
typedef TObjList<TInteger>TIntegerList;

//---------------------------------------------------------------------------
#endif
