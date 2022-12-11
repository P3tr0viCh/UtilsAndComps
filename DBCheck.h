// ---------------------------------------------------------------------------

#ifndef DBCheckH
#define DBCheckH

#include "DBOperation.h"

// ---------------------------------------------------------------------------
class TDBCheck : public TDBOperation {
private:
	String FDBVersion;

protected:
	TADODataSet * DataSet;

	String FQuery;

	// -----------------------------------------------------------------------
	virtual TField * GetFieldVersion() = 0;

public:
	__fastcall TDBCheck(TConnectionInfo * ConnectionInfo,
		IDBOperationEvent * DBOperationEvent);

	void Operation();

	// -----------------------------------------------------------------------
	__property String DBVersion = {read = FDBVersion};
};

// ---------------------------------------------------------------------------
#endif
