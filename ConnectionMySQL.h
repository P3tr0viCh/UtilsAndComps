// ---------------------------------------------------------------------------

#ifndef ConnectionMySQLH
#define ConnectionMySQLH

#include <System.Classes.hpp>

#include "ConnectionServer.h"

// ---------------------------------------------------------------------------
class TConnectionMySQL : public TConnectionServer {
private:
	String FDatabase;

	// -----------------------------------------------------------------------
	String GetConnectionString();

public:
	__fastcall TConnectionMySQL();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TConnectionInfo * Source);
	String __fastcall ToString();

	// -----------------------------------------------------------------------
	__property String Database = {read = FDatabase, write = FDatabase};
};

// ---------------------------------------------------------------------------
#endif
