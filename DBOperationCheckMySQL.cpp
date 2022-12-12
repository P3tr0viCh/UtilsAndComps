// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBOperationCheckMySQL.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBOperationCheckMySQL::TDBOperationCheckMySQL
	(TDBConnectionMySQL * Connection, IDBOperationEvent * DBOperationEvent)
	: TDBOperationCheck(Connection, DBOperationEvent) {
	FQuery = "SELECT @@version;";
}

// ---------------------------------------------------------------------------
TField * TDBOperationCheckMySQL::GetFieldVersion() {
	return DataSet->Fields->Fields[0];
}

// ---------------------------------------------------------------------------
