// ---------------------------------------------------------------------------

#ifndef DBOperationCheckAccessH
#define DBOperationCheckAccessH

#include "DBConnectionAccess.h"

#include "DBOperationCheck.h"

// ---------------------------------------------------------------------------
class TDBOperationCheckAccess : public TDBOperationCheck {
public:
	__fastcall TDBOperationCheckAccess(TDBConnectionAccess * Connection,
		IDBOperationEvent * DBOperationEvent, String TableName);

	TField * GetFieldVersion();
};

// ---------------------------------------------------------------------------
#endif
