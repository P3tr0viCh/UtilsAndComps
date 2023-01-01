// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBOperationCheckFirebird.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBOperationCheckFirebird::TDBOperationCheckFirebird
	(TDBConnectionFirebird * Connection, IDBOperationEvent * DBOperationEvent)
	: TDBOperationCheck(Connection, DBOperationEvent) {
	Query = "SELECT rdb$get_context('SYSTEM', 'ENGINE_VERSION') FROM rdb$database;";
}

// ---------------------------------------------------------------------------
TField * TDBOperationCheckFirebird::GetFieldVersion() {
	return DataSet->Fields->Fields[0];
}

// ---------------------------------------------------------------------------
