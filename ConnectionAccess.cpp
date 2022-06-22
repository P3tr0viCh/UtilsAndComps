// ---------------------------------------------------------------------------

#pragma hdrstop

#include "ConnectionAccess.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TConnectionAccess::TConnectionAccess() : TConnectionInfo() {
	FDatabaseFileName = "";
	User = "Admin";
}

// ---------------------------------------------------------------------------
bool __fastcall TConnectionAccess::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TConnectionAccess * Connection = (TConnectionAccess*) Obj;

	if (DatabaseFileName != Connection->DatabaseFileName ||
		User != Connection->User || Password != Connection->Password)
		return false;

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TConnectionAccess::Assign(TConnectionInfo * Source) {
	TConnectionAccess * Connection = (TConnectionAccess*) Source;

	DatabaseFileName = Connection->DatabaseFileName;
	User = Connection->User;
	Password = Connection->Password;
}

// ---------------------------------------------------------------------------
String __fastcall TConnectionAccess::ToString() {
	String S;

	S = "TConnectionAccess{";
	S += "DatabaseFileName='" + DatabaseFileName + "'";
	S += ", ";
	S += "User='" + User + "'";
	S += ", ";
	S += "Password='" + Password + "'";
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
String TConnectionAccess::GetConnectionString() {
	return Format
		("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Persist Security Info=False;User ID=%s;Jet OLEDB:Database Password=%s;",
		ARRAYOFCONST((DatabaseFileName, User, Password)));
}

// ---------------------------------------------------------------------------
