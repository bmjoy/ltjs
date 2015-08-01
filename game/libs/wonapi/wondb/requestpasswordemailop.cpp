
#include "requestpasswordemailop.h"

using namespace WONAPI;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
RequestPasswordEmailOp::RequestPasswordEmailOp(ServerContext* theContext)
	: DBProxyOp(theContext),
	  mMsgType(8)
{
	Init();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
RequestPasswordEmailOp::RequestPasswordEmailOp(const IPAddr& theAddr)
	: DBProxyOp(theAddr),
	  mMsgType(8)
{
	Init();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
WONStatus RequestPasswordEmailOp::CheckResponse()
{
	// Call the base class implementation
	WONStatus result = DBProxyOp::CheckResponse();
	if (result != WS_Success)
		return result;
	
	if (mSubMessageReplyType != mMsgType+1)
		return InvalidReplyHeader();
	
	// Do extended unpack
	return WS_Success;

}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void RequestPasswordEmailOp::RunHook()
{
	SetMessageType(DBProxyAccount);
	SetSubMessageType(mMsgType);

	// Pack the message data
	WriteBuffer requestData;
	requestData.AppendWString(mUserName);

	// Pack the custom data (if any)
	mProfileData->PackRequestData(requestData, true);

	SetProxyRequestData(requestData.ToByteBuffer());

	DBProxyOp::RunHook();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void RequestPasswordEmailOp::Init()
{
	mLengthFieldSize = 4;
	mProfileData = new ProfileData;
}
