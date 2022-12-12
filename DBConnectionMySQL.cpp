// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBConnectionMySQL.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBConnectionMySQL::TDBConnectionMySQL() : TDBConnectionServer() {
	Port = 3306;
}

// ---------------------------------------------------------------------------
bool __fastcall TDBConnectionMySQL::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TDBConnectionMySQL * Connection = (TDBConnectionMySQL*) Obj;

	if (Driver != Connection->Driver || Host != Connection->Host ||
		Port != Connection->Port || Database != Connection->Database ||
		User != Connection->User || Password != Connection->Password)
		return false;

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TDBConnectionMySQL::Assign(TDBConnection * Source) {
	TDBConnectionMySQL * Connection = (TDBConnectionMySQL*) Source;

	Driver = Connection->Driver;
	Host = Connection->Host;
	Port = Connection->Port;
	Database = Connection->Database;
	User = Connection->User;
	Password = Connection->Password;
}

// ---------------------------------------------------------------------------
String __fastcall TDBConnectionMySQL::ToString() {
	String S;

	S = "TDBConnectionMySQL{";
	S += "Driver='" + Driver + "'";
	S += ", ";
	S += "Host='" + Host + "'";
	S += ", ";
	S += "Port='" + Port + "'";
	S += ", ";
	S += "Database='" + Database + "'";
	S += ", ";
	S += "User='" + User + "'";
	S += ", ";
	S += "Password='" + Password + "'";
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
String TDBConnectionMySQL::GetConnectionString() {
	return Format
		("DRIVER={%s};SERVER=%s;PORT=%s;DATABASE=%s;USER=%s;PASSWORD=%s;OPTION=3;",
		ARRAYOFCONST((Driver, Host, Port, Database, User, Password)));
}

// ---------------------------------------------------------------------------
