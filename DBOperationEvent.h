// ---------------------------------------------------------------------------

#ifndef DBOperationEventH
#define DBOperationEventH

#include <System.Classes.hpp>

// ---------------------------------------------------------------------------
__interface IDBOperationEvent {
public:
	virtual void DBOperationEventStart(TObject * Sender) = 0;
	virtual void DBOperationEventEndOK(TObject * Sender) = 0;
	virtual void DBOperationEventEndFail(TObject * Sender) = 0;
};

// ---------------------------------------------------------------------------
#endif
