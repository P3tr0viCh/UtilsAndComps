// ---------------------------------------------------------------------------

#ifndef DBOperationCheckOracleH
#define DBOperationCheckOracleH

#include "DBConnectionOracle.h"

#include "DBOperationCheck.h"

// ---------------------------------------------------------------------------
class TDBOperationCheckOracle : public TDBOperationCheck {
public:
	__fastcall TDBOperationCheckOracle(TDBConnectionOracle * Connection,
		IDBOperationEvent * DBOperationEvent);

	TField * GetFieldVersion();
};

// ---------------------------------------------------------------------------
#endif
