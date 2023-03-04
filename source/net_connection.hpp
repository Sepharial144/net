#ifndef _NET_CONNECTION_HPP_
#define _NET_CONNECTION_HPP_

#include "BasicSocket.hpp"

namespace net
{
	class connection: public BasicSocket
	{
	public:
		explicit connection() = delete;
		explicit connection(const char* addr, int32_t port);
		~connection();

		int32_t connect();

	private:
		const char* m_address = nullptr;
		int32_t m_defaultPort = {0};
		sockaddr_in m_connectionSettings = {0};
	};
} // ! namespace net

#endif // !_NET_CONNECTION_HPP_