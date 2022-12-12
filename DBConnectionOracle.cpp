// ---------------------------------------------------------------------------

#pragma hdrstop

#include "DBConnectionOracle.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDBConnectionOracle::TDBConnectionOracle() : TDBConnectionServer() {
	Port = 1521;
	FService = "";
}

// ---------------------------------------------------------------------------
bool __fastcall TDBConnectionOracle::Equals(TObject * Obj) {
	if (this == Obj)
		return true;
	if (Obj == NULL || ClassType() != Obj->ClassType())
		return false;

	TDBConnectionOracle * Connection = (TDBConnectionOracle*) Obj;

	if (Driver != Connection->Driver || Host != Connection->Host ||
		Port != Connection->Port || Service != Connection->Service ||
		User != Connection->User || Password != Connection->Password)
		return false;

	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TDBConnectionOracle::Assign(TDBConnection * Source) {
	TDBConnectionOracle * Connection = (TDBConnectionOracle*) Source;

	Driver = Connection->Driver;
	Host = Connection->Host;
	Port = Connection->Port;
	Service = Connection->Service;
	User = Connection->User;
	Password = Connection->Password;
}

// ---------------------------------------------------------------------------
String __fastcall TDBConnectionOracle::ToString() {
	String S;

	S = "TDBConnectionOracle{";
	S += "Driver='" + Driver + "'";
	S += ", ";
	S += "Host='" + Host + "'";
	S += ", ";
	S += "Port='" + Port + "'";
	S += ", ";
	S += "Service='" + Service + "'";
	S += ", ";
	S += "User='" + User + "'";
	S += ", ";
	S += "Password='" + Password + "'";
	S += "}";

	return S;
}

// ---------------------------------------------------------------------------
String TDBConnectionOracle::GetConnectionString() {
	return Format("DRIVER={%s};DBQ=%s:%s/%s;UID=%s;PWD=%s;",
		ARRAYOFCONST((Driver, Host, Port, Service, User, Password)));
}

// ---------------------------------------------------------------------------
