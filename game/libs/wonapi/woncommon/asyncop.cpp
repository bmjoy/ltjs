#include "asyncop.h"
#include "asyncopcontainer.h"
#include "timerthread.h"
#include "wonapi.h"

using namespace WONAPI;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
AsyncOp::AsyncOp()
{
	mStartTick = 0;
	mTimeout = OP_TIMEOUT_INFINITE;
	mMode = OP_MODE_ASYNC;
	mStatus = WS_None;
	mAwaitingCompletion = false;
	mKilled = false;
	mRunAsyncImmediately = false;

	mTimerMap = NULL;
	mOpContainer = NULL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
AsyncOp::~AsyncOp()
{
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AsyncOp::AddToTimerMap(TimerMap &theMap, DWORD theTimeout)
{
	RemoveFromTimerMap(true);
	mTimerMap = &theMap;
	mTimerMapItr = theMap.insert(TimerMap::value_type(theTimeout,this));
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AsyncOp::RemoveFromTimerMap(bool erase)
{
	if(mTimerMap!=NULL)
	{
		if(erase)
			mTimerMap->erase(mTimerMapItr);

		mTimerMap = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AsyncOp::SetMode(AsyncRunMode theMode) 
{ 
	if(!Pending())
		mMode = theMode; 
}
	
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AsyncOp::SetTimeout(DWORD theTimeout) 
{ 
	if(!Pending())
		mTimeout = theTimeout; 
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AsyncOp::SetRunAsyncImmediately(bool runImmediately) // should op be queued before running async or not?
{
	mRunAsyncImmediately = runImmediately;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AsyncOp::Kill(bool finish)
{
	mKilled = true;

	if(finish)
		Finish(WS_Killed);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AsyncOp::QueueInOpContainer()
{
	if(mOpContainer!=NULL)
		mOpContainer->QueueOp(this);
	else
		WONAPICore::QueueInContainer(this);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AsyncOp::Finish(WONStatus theStatus)
{
	// Check if already finished
	AutoCrit aCrit(mStatusCrit);
//	if(mAwaitingCompletion) // already finished
//		return;
	if(!Pending()) // can only finish a pending operation
		return;
	
	mStatus = theStatus;
	mAwaitingCompletion = true;
	aCrit.Leave();

	switch(mMode)
	{
		case OP_MODE_ASYNC: QueueInOpContainer(); break;
		case OP_MODE_BLOCK: /*Complete();*/ break;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AsyncOp::ForceFinish(WONStatus theStatus)
{
	if(Pending())
	{
		Finish(theStatus);
		return;
	}

	AutoCrit aCrit(mStatusCrit);
	if(AwaitingCompletion()) // not valid if the op is already awaiting completion
		return;

	mStatus = WS_Pending;
	mKilled = false;

	aCrit.Leave();
	Finish(theStatus);	
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AsyncOp::Complete()
{
	// Check if already completed
	AutoCrit aCrit(mStatusCrit);
	if(!AwaitingCompletion())
		return;

	mAwaitingCompletion = false;

	if(mTimerMap!=NULL)
	{
		aCrit.Leave();
		WONAPICore::RemoveFromTimerThread(this);
	}
	
	aCrit.Leave();

	CleanupHook();	

	OpCompletionBasePtr aCompletion = mCompletion;
	if(aCompletion.get()!=NULL)
		aCompletion->Complete(this);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AsyncOp::StartRunAsync()
{
	RunHook();
	if(Pending())
	{
		if(mTimeout>0)
		{
			if(mTimeout!=OP_TIMEOUT_INFINITE)
			{
				WONAPICore::AddToTimerThread(this, mTimeout);
			}
		}
		else // timed out
			Finish(WS_TimedOut);
	}	
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
WONStatus AsyncOp::Run(AsyncRunMode theMode, DWORD theTimeout)
{
	// Check if already running
	AutoCrit aCrit(mStatusCrit);
	if(Pending())
		return WS_Pending;
	else if(AwaitingCompletion())
		return WS_AwaitingCompletion;

	mStatus = WS_Pending;
	mKilled = false;

	aCrit.Leave();

	mStartTick = GetTickCount();
	mTimeout = theTimeout;
	mMode = theMode;

	if(IsAsync())
	{
		if(mRunAsyncImmediately)
			StartRunAsync();
		else
			QueueInOpContainer();
	}
	else
	{
		RunHook();
		Complete();
	}

	return mStatus;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AsyncOp::RunAsTimer(DWORD theTimeout)
{
	AutoCrit aCrit(mStatusCrit);
	if(!Runnable())
		return;

	mStatus = WS_Pending;
	mKilled = false;

	aCrit.Leave();

	mStartTick = GetTickCount();
	mTimeout = theTimeout;
	mMode = OP_MODE_ASYNC;
	WONAPICore::AddToTimerThread(this, theTimeout);		
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void AsyncOp::RunAsync(DWORD theTimeout)
{
	Run(OP_MODE_ASYNC, theTimeout);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////	
bool AsyncOp::RunBlock(DWORD theTimeout)
{
	WONStatus aStatus = Run(OP_MODE_BLOCK, theTimeout);
	return aStatus==WS_Success;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
DWORD AsyncOp::TimeLeft()
{
	if(IsAsync()) // asynchronous
		return 0;
	
	int deltaTime = GetTickCount() - mStartTick;
	if(deltaTime >= mTimeout)
		return 0;
	else
		return mTimeout - deltaTime;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
DWORD AsyncOp::RealTimeLeft()
{
	int deltaTime = GetTickCount() - mStartTick;
	if(deltaTime >= mTimeout)
		return 0;
	else
		return mTimeout - deltaTime;
}

/*
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
TimeoutOp::TimeoutOp(AsyncOp *theOpToTimeout) : mOpToTimeout(theOpToTimeout)
{
	SetMode(OP_MODE_BLOCK);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void TimeoutOp::RunHook()
{ 
	mOpToTimeout->Finish(WS_TimedOut); 
	mOpToTimeout = NULL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void TimeoutOp::CleanupHook()
{
	if(mOpToTimeout.get()!=NULL)
	{
		mOpToTimeout->Kill();
		mOpToTimeout = NULL;
	}
}
*/
