#ifndef _NET_HPP_
#define _NET_HPP_

#include "socket_definition.hpp"
#include "NetServer.hpp"
#include "NetClient.hpp"
#include "NetConnection.hpp"

namespace net
{
	enum enumShutdown: int16_t
	{
		receive = SD_RECEIVE,
		send = SD_SEND,
		both = SD_BOTH
	};
} // !namespace net

#endif // !_NET_HPP