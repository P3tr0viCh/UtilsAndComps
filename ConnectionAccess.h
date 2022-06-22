// ---------------------------------------------------------------------------

#ifndef ConnectionAccessH
#define ConnectionAccessH

#include <System.Classes.hpp>

#include "ConnectionInfo.h"

// ---------------------------------------------------------------------------
class TConnectionAccess : public TConnectionInfo {
private:
	String FDatabaseFileName;

	// -----------------------------------------------------------------------
	String GetConnectionString();

public:
	__fastcall TConnectionAccess();

	bool __fastcall Equals(TObject * Obj);
	void __fastcall Assign(TConnectionInfo * Source);
	String __fastcall ToString();

	// -----------------------------------------------------------------------
	__property String DatabaseFileName = {
		read = FDatabaseFileName, write = FDatabaseFileName};
};

// ---------------------------------------------------------------------------
#endif
