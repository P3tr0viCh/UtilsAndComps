// ---------------------------------------------------------------------------

#ifndef DBOperationCheckMySQLH
#define DBOperationCheckMySQLH

#include "DBConnectionMySQL.h"

#include "DBOperationCheck.h"

// ---------------------------------------------------------------------------
class TDBOperationCheckMySQL : public TDBOperationCheck {
public:
	__fastcall TDBOperationCheckMySQL(TDBConnectionMySQL * Connection,
		IDBOperationEvent * DBOperationEvent);

	TField * GetFieldVersion();
};

// ---------------------------------------------------------------------------
#endif
