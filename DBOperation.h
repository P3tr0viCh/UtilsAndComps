// ---------------------------------------------------------------------------

#ifndef DBOperationH
#define DBOperationH

#include <System.Classes.hpp>
#include <Data.Win.ADODB.hpp>

#include "DBConnection.h"

#include "DBOperationEvent.h"

// ----------------------------------------------------------------------------
class TDBOperation : public TObject {
private:
	TADOConnection * FConnection;

	TDBConnection * FDBConnection;

	IDBOperationEvent * FDBOperationEvent;

	String FErrorMessage;

	// -----------------------------------------------------------------------
	__fastcall TDBOperation();

public:
	__fastcall TDBOperation(TDBConnection * DBConnection,
		IDBOperationEvent * DBOperationEvent);
	__fastcall ~TDBOperation();

	virtual void Operation() = 0;

	bool Execute();

	// -----------------------------------------------------------------------
	__property TADOConnection * Connection = {read = FConnection};

	__property TDBConnection * DBConnection = {read = FDBConnection};

	__property String ErrorMessage = {read = FErrorMessage};
};

// ---------------------------------------------------------------------------
#endif
