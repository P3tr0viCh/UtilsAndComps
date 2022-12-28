// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBConnectionAccess.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBConnectionAccess::TDBConnectionAccess() : TDBConnection() {
	User = "Admin";
}

// ---------------------------------------------------------------------------
bool __fastcall TDBConnectionAccess::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TDBConnectionAccess * Connection = (TDBConnectionAccess*) Obj;

	if (Database != Connection->Database || User != Connection->User ||
		Password != Connection->Password)
		return false;

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TDBConnectionAccess::Assign(TDBConnection * Source) {
	TDBConnectionAccess * Connection = (TDBConnectionAccess*) Source;

	Database = Connection->Database;
	User = Connection->User;
	Password = Connection->Password;
}

// ---------------------------------------------------------------------------
String __fastcall TDBConnectionAccess::ToString() {
	String S;

	S = "TDBConnectionAccess{";
	S += "Database=" + Database;
	S += ", ";
	S += "User=" + User;
	S += ", ";
	S += "Password=" + Password;
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
String TDBConnectionAccess::GetConnectionString() {
	return Format
		("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s;UID=%s;PWD=%s;",
		ARRAYOFCONST((Database, User, Password)));
}

// ---------------------------------------------------------------------------
