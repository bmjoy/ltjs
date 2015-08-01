#ifndef __WON_SOCKETTHREAD_H__
#define __WON_SOCKETTHREAD_H__
#include "wonshared.h"

#include "socketop.h"
#include "woncommon/thread.h"
#include "woncommon/criticalsection.h"

namespace WONAPI
{


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class SocketThread : public Thread
{
protected:
	CriticalSection mDataCrit;
	bool mSingleThreaded;

	static bool DoOp(SocketOp *theOp);

protected:
	virtual void ThreadFunc();

public:
	SocketThread();
	virtual ~SocketThread();

	void SetSingleThreaded(bool isSingleThreaded) { mSingleThreaded = isSingleThreaded; }

	virtual void AddSocketOp(SocketOp *);
	virtual void RemoveSocketOp(SocketOp *);
	virtual void Pump(DWORD theWaitTime);
	virtual void PurgeOps();

	// Does the socket thread actually need a thread?  For instance, if it simply
	// manages other threads then the SocketThread doesn't need to start at all.
	virtual bool NeedThread() { return true; } 
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

}; // namespace WONAPI

#endif
