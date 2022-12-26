// ---------------------------------------------------------------------------

#ifndef DBOperationCheckParadoxH
#define DBOperationCheckParadoxH

#include "DBConnectionParadox.h"

#include "DBOperationCheck.h"

// ---------------------------------------------------------------------------
class TDBOperationCheckParadox : public TDBOperationCheck {
public:
	__fastcall TDBOperationCheckParadox(TDBConnectionParadox * Connection,
		IDBOperationEvent * DBOperationEvent, String TableName);

	TField * GetFieldVersion();
};

// ---------------------------------------------------------------------------
#endif
