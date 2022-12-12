// ---------------------------------------------------------------------------

#ifndef DBConnectionServerH
#define DBConnectionServerH

#include "DBConnection.h"

// ---------------------------------------------------------------------------
class TDBConnectionServer : public TDBConnection {
private:
	String FDriver;

	String FHost;
	String FPort;

public:
	__fastcall TDBConnectionServer();

	// -----------------------------------------------------------------------
	__property String Driver = {read = FDriver, write = FDriver};

	__property String Host = {read = FHost, write = FHost};
	__property String Port = {read = FPort, write = FPort};
};

// ---------------------------------------------------------------------------
#endif
