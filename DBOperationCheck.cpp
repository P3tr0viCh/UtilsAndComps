// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBOperationCheck.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBOperationCheck::TDBOperationCheck(TDBConnection * DBConnection,
	IDBOperationEvent * DBOperationEvent)
	: TDBOperation(DBConnection, DBOperationEvent) {
}

// ---------------------------------------------------------------------------
void TDBOperationCheck::Operation() {
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
