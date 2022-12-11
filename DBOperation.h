// ---------------------------------------------------------------------------

#ifndef DBOperationH
#define DBOperationH

#include <System.Classes.hpp>
#include <Data.Win.ADODB.hpp>

#include "ConnectionInfo.h"

#include "DBOperationEvent.h"

// ---------------------------------------------------------------------------
class TDBOperation : public TObject {
private:
	TADOConnection * FConnection;

	TConnectionInfo * FConnectionInfo;

	IDBOperationEvent * FDBOperationEvent;

	String FErrorMessage;

	// -----------------------------------------------------------------------
	__fastcall TDBOperation();

public:
	__fastcall TDBOperation(TConnectionInfo * ConnectionInfo,
		IDBOperationEvent * DBOperationEvent);
	__fastcall ~TDBOperation();

	virtual void Operation() = 0;

	bool Execute();

	// -----------------------------------------------------------------------
	__property TADOConnection * Connection = {read = FConnection};

	__property TConnectionInfo * ConnectionInfo = {read = FConnectionInfo};

	__property String ErrorMessage = {read = FErrorMessage};
};

// ---------------------------------------------------------------------------
#endif
