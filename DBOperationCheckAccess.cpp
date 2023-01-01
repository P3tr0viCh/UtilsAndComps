// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBOperationCheckAccess.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBOperationCheckAccess::TDBOperationCheckAccess
	(TDBConnectionAccess * Connection, IDBOperationEvent * DBOperationEvent,
	String TableName) : TDBOperationCheck(Connection, DBOperationEvent) {
	Query = "SELECT COUNT(*) FROM " + TableName + ";";
}

// ---------------------------------------------------------------------------
TField * TDBOperationCheckAccess::GetFieldVersion() {
	return DataSet->Fields->Fields[0];
}

// ---------------------------------------------------------------------------
