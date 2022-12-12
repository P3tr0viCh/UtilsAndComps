// ---------------------------------------------------------------------------

#ifndef DBConnectionOracleH
#define DBConnectionOracleH

#include <System.Classes.hpp>

#include "DBConnectionServer.h"

// ---------------------------------------------------------------------------
class TDBConnectionOracle : public TDBConnectionServer {
private:
	String FService;

	String GetConnectionString();

	__property String Database = {};

public:
	__fastcall TDBConnectionOracle();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TDBConnection * Source);
	String __fastcall ToString();

	// -----------------------------------------------------------------------
	__property String Service = {read = FDatabase, write = FDatabase};
};

// ---------------------------------------------------------------------------
#endif
