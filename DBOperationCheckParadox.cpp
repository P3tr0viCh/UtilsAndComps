// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBOperationCheckParadox.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBOperationCheckParadox::TDBOperationCheckParadox
	(TDBConnectionParadox * Connection, IDBOperationEvent * DBOperationEvent,
	String TableName) : TDBOperationCheck(Connection, DBOperationEvent) {
	Query = "SELECT COUNT(*) FROM " + TableName + ";";
}

// ---------------------------------------------------------------------------
TField * TDBOperationCheckParadox::GetFieldVersion() {
	return DataSet->Fields->Fields[0];
}

// ---------------------------------------------------------------------------
