// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBCheck.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBCheck::TDBCheck(TConnectionInfo * ConnectionInfo,
	IDBOperationEvent * DBOperationEvent)
	: TDBOperation(ConnectionInfo, DBOperationEvent) {
}

// ---------------------------------------------------------------------------
void TDBCheck::Operation() {
	Connection->Open();

	try {
		DataSet = new TADODataSet(NULL);
		try {
			DataSet->Recordset = Connection->Execute(FQuery);

			FDBVersion = GetFieldVersion()->AsString;
		}
		__finally {
			DataSet->Free();
		}
	}
	__finally {
		Connection->Close();
	}
}

// ---------------------------------------------------------------------------
