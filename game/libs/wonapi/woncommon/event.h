#ifndef __WON_EVENT_H__
#define __WON_EVENT_H__
#include "wonshared.h"

#ifndef __WON_SINGLETHREADED__

#if defined(WIN32)
#include "event_windows.h"
#elif defined(_LINUX)
#include "event_linux.h"
#elif defined(macintosh) && (macintosh == 1)
#include "event_mac.h"
#endif

#else // __WON_SINGLETHREADED__

#include "platform.h"
namespace WONAPI
{

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class Event
{
private:
	bool mIsSet;
	bool mIsManualReset;

public:
	explicit Event(bool manualReset = false, bool initiallySet = false) : mIsSet(initiallySet), mIsManualReset(manualReset) { }
	virtual ~Event() { }

	void Set() { mIsSet = true; }
	void Reset() { mIsSet = false; }

	bool WaitFor(DWORD theMilliseconds) 
	{ 
		if (mIsSet)
		{
			if (!mIsManualReset)
				mIsSet = false;

			return true;
		}

		Sleep(theMilliseconds);
		return false;
	}
};

}; // namespace WONAPI



#endif // __WON_SINGLETHREADED__


#endif
