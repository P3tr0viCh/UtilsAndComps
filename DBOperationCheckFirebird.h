// ---------------------------------------------------------------------------

#ifndef DBOperationCheckFirebirdH
#define DBOperationCheckFirebirdH

#include "DBConnectionFirebird.h"

#include "DBOperationCheck.h"

// ---------------------------------------------------------------------------
class TDBOperationCheckFirebird : public TDBOperationCheck {
public:
	__fastcall TDBOperationCheckFirebird(TDBConnectionFirebird * Connection,
		IDBOperationEvent * DBOperationEvent);

	TField * GetFieldVersion();
};

// ---------------------------------------------------------------------------
#endif
