// ---------------------------------------------------------------------------

#pragma hdrstop

#include "ConnectionFirebird.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TConnectionFirebird::TConnectionFirebird() : TConnectionInfo() {
	FDatabase = "";
	User = "SYSDBA";
	Password = "MASTERKEY";
	FClient = "fblient.dll";
}

// ---------------------------------------------------------------------------
bool __fastcall TConnectionFirebird::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TConnectionFirebird * Connection = (TConnectionFirebird*) Obj;

	if (Database != Connection->Database || User != Connection->User ||
		Password != Connection->Password || Client != Connection->Client)
		return false;

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TConnectionFirebird::Assign(TConnectionInfo * Source) {
	TConnectionFirebird * Connection = (TConnectionFirebird*) Source;

	Database = Connection->Database;
	User = Connection->User;
	Password = Connection->Password;
	Client = Connection->Client;
}

// ---------------------------------------------------------------------------
String __fastcall TConnectionFirebird::ToString() {
	String S;

	S = "TConnectionFirebird{";
	S += "Database='" + Database + "'";
	S += ", ";
	S += "User='" + User + "'";
	S += ", ";
	S += "Password='" + Password + "'";
	S += ", ";
	S += "Client='" + Client + "'";
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
String TConnectionFirebird::GetConnectionString() {
	return Format
		("DRIVER={Firebird/InterBase(r) driver};DBNAME=%s;UID=%s;PWD=%s;CLIENT=%s",
		ARRAYOFCONST((Database, User, Password, Client)));
}

// ---------------------------------------------------------------------------
