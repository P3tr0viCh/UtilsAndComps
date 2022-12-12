// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBOperation.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBOperation::TDBOperation(TDBConnection * DBConnection,
	IDBOperationEvent * DBOperationEvent) {
	FConnection = new TADOConnection(NULL);

	FDBConnection = DBConnection;

	FDBOperationEvent = DBOperationEvent;
}

// ---------------------------------------------------------------------------
__fastcall TDBOperation::~TDBOperation() {
	FConnection->Free();
}

// ---------------------------------------------------------------------------
bool TDBOperation::Execute() {
	bool Result = false;

	FDBOperationEvent->DBOperationEventStart(this);

	FConnection->ConnectionString = FDBConnection->ConnectionString;

	try {
		Operation();

		Result = true;
	}
	catch (Exception * E) {
		FErrorMessage = E->Message;
	}

	if (Result) {
		FDBOperationEvent->DBOperationEventEndOK(this);
	}
	else {
		FDBOperationEvent->DBOperationEventEndFail(this);
	}

	return Result;
}

// ---------------------------------------------------------------------------
