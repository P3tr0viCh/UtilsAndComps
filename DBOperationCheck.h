// ---------------------------------------------------------------------------

#ifndef DBOperationCheckH
#define DBOperationCheckH

#include "DBOperation.h"

// ---------------------------------------------------------------------------
class TDBOperationCheck : public TDBOperation {
private:
	String FDBVersion;

protected:
	TADODataSet * DataSet;

	String FQuery;

	// -----------------------------------------------------------------------
	virtual TField * GetFieldVersion() = 0;

public:
	__fastcall TDBOperationCheck(TDBConnection * DBConnection,
		IDBOperationEvent * DBOperationEvent);

	void Operation();

	// -----------------------------------------------------------------------
	__property String DBVersion = {read = FDBVersion};
};

// ---------------------------------------------------------------------------
#endif
