#include "routingdisconnectclientop.h"
using namespace WONAPI;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void RoutingDisconnectClientOp::SendRequest()
{
	InitSendMsg(RoutingDisconnectClientRequest);
	SendMsg();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
