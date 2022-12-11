// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBOperation.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBOperation::TDBOperation(TConnectionInfo * ConnectionInfo,
	IDBOperationEvent * DBOperationEvent) {
	FConnection = new TADOConnection(NULL);

	FConnectionInfo = ConnectionInfo;

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

	FConnection->ConnectionString = FConnectionInfo->ConnectionString;

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
