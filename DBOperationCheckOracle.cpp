// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBOperationCheckOracle.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBOperationCheckOracle::TDBOperationCheckOracle
	(TDBConnectionOracle * Connection, IDBOperationEvent * DBOperationEvent)
	: TDBOperationCheck(Connection, DBOperationEvent) {
	FQuery = "SELECT * FROM v$version;";
}

// ---------------------------------------------------------------------------
TField * TDBOperationCheckOracle::GetFieldVersion() {
	TField * F = DataSet->Fields->FindField("BANNER_FULL");
	if (F == NULL) {
		F = DataSet->Fields->Fields[0];
	}

	return F;
}

// ---------------------------------------------------------------------------
