// ---------------------------------------------------------------------------

#ifndef DBConnectionParadoxH
#define DBConnectionParadoxH

#include <System.Classes.hpp>

#include "DBConnection.h"

// ---------------------------------------------------------------------------
class TDBConnectionParadox : public TDBConnection {
private:
	String GetConnectionString();

public:
	__fastcall TDBConnectionParadox();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TDBConnection * Source);
	String __fastcall ToString();
};

// ---------------------------------------------------------------------------
#endif
