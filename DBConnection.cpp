// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBConnection.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBConnection::TDBConnection() {
	FUser = "root";
	FPassword = "";
	FDatabase = "";
}

// ---------------------------------------------------------------------------