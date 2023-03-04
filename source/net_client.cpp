#include "net_server.hpp"
#include "net_client.hpp"
#include "algo.hpp"

namespace net
{
	client::client(size_t len_message)
		: BasicSocket{ AF_INET, SOCK_STREAM, IPPROTO_TCP }
		, m_lenMessage{ len_message }
	{
	}

	client::~client()
	{
		close();
	}

	SOCKET client::getSocket()
	{
		return BasicSocket::getSocket();
	}

} // ! namespace net