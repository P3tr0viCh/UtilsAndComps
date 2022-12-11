// ---------------------------------------------------------------------------

#ifndef TDBCheckOracleH
#define TDBCheckOracleH

#include <ConnectionOracle.h>

#include "DBCheck.h"

// ---------------------------------------------------------------------------
class TDBCheckOracle : public TDBCheck {
public:
	__fastcall TDBCheckOracle(TConnectionOracle * Connection);

	TField * GetFieldVersion();

	String ConnectionToLogString();
};

// ---------------------------------------------------------------------------
#endif
