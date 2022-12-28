// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBConnectionFirebird.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBConnectionFirebird::TDBConnectionFirebird() : TDBConnection() {
	User = "SYSDBA";
	Password = "masterkey";
	FClient = "fbclient.dll";
}

// ---------------------------------------------------------------------------
bool __fastcall TDBConnectionFirebird::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TDBConnectionFirebird * Connection = (TDBConnectionFirebird*) Obj;

	if (Database != Connection->Database || User != Connection->User ||
		Password != Connection->Password || Client != Connection->Client)
		return false;

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TDBConnectionFirebird::Assign(TDBConnection * Source) {
	TDBConnectionFirebird * Connection = (TDBConnectionFirebird*) Source;

	Database = Connection->Database;
	User = Connection->User;
	Password = Connection->Password;
	Client = Connection->Client;
}

// ---------------------------------------------------------------------------
String __fastcall TDBConnectionFirebird::ToString() {
	String S;

	S = "TDBConnectionFirebird{";
	S += "Database=" + Database;
	S += ", ";
	S += "User=" + User;
	S += ", ";
	S += "Password=" + Password;
	S += ", ";
	S += "Client=" + Client;
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
String TDBConnectionFirebird::GetConnectionString() {
	return Format
		("DRIVER={Firebird/InterBase(r) driver};DBNAME=%s;UID=%s;PWD=%s;CLIENT=%s;",
		ARRAYOFCONST((Database, User, Password, Client)));
}

// ---------------------------------------------------------------------------
