// ---------------------------------------------------------------------------

#ifndef DBConnectionH
#define DBConnectionH

#include <System.Classes.hpp>

// ---------------------------------------------------------------------------
class TDBConnection : public TObject {
private:
	String FUser;
	String FPassword;

	virtual String GetConnectionString() = 0;

protected:
	String FDatabase;

public:
	__fastcall TDBConnection();

	virtual bool __fastcall Equals(TObject * Obj) = 0;
	virtual void __fastcall Assign(TDBConnection * Source) = 0;
	virtual String __fastcall ToString() = 0;

	// -----------------------------------------------------------------------
	__property String User = {read = FUser, write = FUser};
	__property String Password = {read = FPassword, write = FPassword};

	__property String Database = {read = FDatabase, write = FDatabase};

	__property String ConnectionString = {read = GetConnectionString};
};

// ---------------------------------------------------------------------------
#endif
