// ---------------------------------------------------------------------------

#ifndef IntegerPairH
#define IntegerPairH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>

#include <Integer.h>
#include <ObjList.h>

// ---------------------------------------------------------------------------
class TIntegerPair : public TObject {
private:
	TInteger * FInt1;
	TInteger * FInt2;

	// -----------------------------------------------------------------------
	void Init();

	void SetInt1(TInteger * Value);
	void SetInt2(TInteger * Value);

public:
	__fastcall TIntegerPair();
	__fastcall TIntegerPair(int Value1, int Value2);
	__fastcall TIntegerPair(TIntegerPair * Value);
	__fastcall ~TIntegerPair();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TIntegerPair * Source);
	String __fastcall ToString();

	// -----------------------------------------------------------------------
	__property TInteger * Int1 = {read = FInt1, write = SetInt1};
	__property TInteger * Int2 = {read = FInt2, write = SetInt2};
};

// ---------------------------------------------------------------------------
typedef TObjList<TIntegerPair>TIntegerPairList;

// ---------------------------------------------------------------------------
#endif
