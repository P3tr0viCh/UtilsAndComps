// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBConnectionParadox.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBConnectionParadox::TDBConnectionParadox() : TDBConnection() {
}

// ---------------------------------------------------------------------------
bool __fastcall TDBConnectionParadox::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TDBConnectionParadox * Connection = (TDBConnectionParadox*) Obj;

	if (Database != Connection->Database || User != Connection->User ||
		Password != Connection->Password)
		return false;

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TDBConnectionParadox::Assign(TDBConnection * Source) {
	TDBConnectionParadox * Connection = (TDBConnectionParadox*) Source;

	Database = Connection->Database;
	User = Connection->User;
	Password = Connection->Password;
}

// ---------------------------------------------------------------------------
String __fastcall TDBConnectionParadox::ToString() {
	String S;

	S = "TDBConnectionParadox{";
	S += "Database=" + Database;
	S += ", ";
	S += "User=" + User;
	S += ", ";
	S += "Password=" + Password;
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
String TDBConnectionParadox::GetConnectionString() {
	return Format
		("DRIVER={Microsoft Paradox Driver (*.db )};DBQ=%0:s;Persist Security Info=True;PWD=%1:s;Mode=Read;Extended Properties='FIL=Paradox 7.x;DefaultDir=%0:s;';Initial Catalog=%0:s;",
		ARRAYOFCONST((Database, Password)));
}

// ---------------------------------------------------------------------------
