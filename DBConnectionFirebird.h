// ---------------------------------------------------------------------------

#ifndef DBConnectionFirebirdH
#define DBConnectionFirebirdH

#include <System.Classes.hpp>

#include "DBConnection.h"

// ---------------------------------------------------------------------------
class TDBConnectionFirebird : public TDBConnection {
private:
	String FClient;

	// -----------------------------------------------------------------------
	String GetConnectionString();

public:
	__fastcall TDBConnectionFirebird();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TDBConnection * Source);
	String __fastcall ToString();

	// -----------------------------------------------------------------------
	__property String Client = {read = FClient, write = FClient};
};

// ---------------------------------------------------------------------------
#endif
