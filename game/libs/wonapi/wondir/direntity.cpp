#include "direntity.h"
#include "woncommon/readbuffer.h"
#include "woncommon/writebuffer.h"

using namespace WONAPI;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
DirEntity::DirEntity()
{
	mType = 0;
	mLifespan = 0;
	mCreated = 0;
	mTouched = 0;
	mCRC = 0;
	mCreateIP = 0;
	mTouchIP = 0;
	mCreateId = 0;
	mTouchId = 0;
	mReqUnique = false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
IPAddr DirEntity::GetNetAddrAsIP() const
{
	IPAddr anAddr;
	if(mNetAddr->length()>=6)
		anAddr.SetSixByte(mNetAddr->data());

	return anAddr;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
