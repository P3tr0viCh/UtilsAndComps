// ---------------------------------------------------------------------------

#ifndef DBConnectionMySQLH
#define DBConnectionMySQLH

#include <System.Classes.hpp>

#include "DBConnectionServer.h"

// ---------------------------------------------------------------------------
class TDBConnectionMySQL : public TDBConnectionServer {
private:
	String GetConnectionString();

public:
	__fastcall TDBConnectionMySQL();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TDBConnection * Source);
	String __fastcall ToString();
};

// ---------------------------------------------------------------------------
#endif
