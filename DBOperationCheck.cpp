// ---------------------------------------------------------------------------

#pragma hdrstop

#include "UtilsLog.h"

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
			if (SQLToLog) {
				WriteToLog(Query);
			}

			DataSet->Recordset = Connection->Execute(Query);

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
