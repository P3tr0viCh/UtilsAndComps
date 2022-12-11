// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBCheckMySQL.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBCheckMySQL::TDBCheckMySQL(TConnectionInfo * Connection,
	IDBOperationEvent * DBOperationEvent)
	: TDBCheck(Connection, DBOperationEvent) {
	FQuery = "SELECT @@version;";
}

// ---------------------------------------------------------------------------
TField * TDBCheckMySQL::GetFieldVersion() {
	return DataSet->Fields->Fields[0];
}

// ---------------------------------------------------------------------------
