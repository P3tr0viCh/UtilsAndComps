// ---------------------------------------------------------------------------

#pragma hdrstop

#include "ConnectionServer.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TConnectionServer::TConnectionServer() : TConnectionInfo() {
	FDriver = "";
	FHost = "localhost";
	FPort = "";
}

// ---------------------------------------------------------------------------