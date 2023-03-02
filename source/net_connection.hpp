#ifndef _NET_CONNECTION_HPP_
#define _NET_CONNECTION_HPP_

#include "win32_definitions.hpp"
#include "socket_definition.hpp"
#include "net_client.hpp"

namespace net
{
	class connection
	{
	public:
		explicit connection() = delete;
		explicit connection(const char* addr, const char* port);
		~connection();

		int32_t connect();

		int32_t send(const char* data, size_t len);
		int32_t read(char* data, size_t len);

	private:
		WSADATA m_wsaData = {0};
		SOCKET m_socket = { INVALID_SOCKET };
		const char* m_address = nullptr;
		const char* m_defaultPort = nullptr;
		sockaddr_in m_connectionSettings = {0};
	};
} // ! namespace net

#endif // !_NET_CONNECTION_HPP_