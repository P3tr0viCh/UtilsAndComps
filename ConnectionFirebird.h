// ---------------------------------------------------------------------------

#ifndef ConnectionFirebirdH
#define ConnectionFirebirdH

#include <System.Classes.hpp>

#include "ConnectionInfo.h"

// ---------------------------------------------------------------------------
class TConnectionFirebird : public TConnectionInfo {
private:
	String FDatabase;
	String FClient;

	// -----------------------------------------------------------------------
	String GetConnectionString();

public:
	__fastcall TConnectionFirebird();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TConnectionInfo * Source);
	String __fastcall ToString();

	// -----------------------------------------------------------------------
	__property String Database = {read = FDatabase, write = FDatabase};
	__property String Client = {read = FClient, write = FClient};
};

// ---------------------------------------------------------------------------
#endif
