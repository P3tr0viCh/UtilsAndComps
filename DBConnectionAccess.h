// ---------------------------------------------------------------------------

#ifndef DBConnectionAccessH
#define DBConnectionAccessH

#include <System.Classes.hpp>

#include "DBConnection.h"

// ---------------------------------------------------------------------------
class TDBConnectionAccess : public TDBConnection {
private:
	String GetConnectionString();

public:
	__fastcall TDBConnectionAccess();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TDBConnection * Source);
	String __fastcall ToString();
};

// ---------------------------------------------------------------------------
#endif
