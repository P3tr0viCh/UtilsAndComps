// ---------------------------------------------------------------------------

#ifndef DBCheckMySQLH
#define DBCheckMySQLH

//#include "ConnectionMySQL.h"

#include "DBCheck.h"

// ---------------------------------------------------------------------------
class TDBCheckMySQL : public TDBCheck {
public:
	__fastcall TDBCheckMySQL(TConnectionInfo * Connection,
		IDBOperationEvent * DBOperationEvent);

	TField * GetFieldVersion();
};

// ---------------------------------------------------------------------------
#endif
