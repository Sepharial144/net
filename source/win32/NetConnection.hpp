#ifndef _NET_CONNECTION_HPP_
#define _NET_CONNECTION_HPP_

#include "socket_definition.hpp"

namespace net
{
	class connection
	{
	public:
		explicit connection() = delete;
		explicit connection(const char* addr, const char* port);
		~connection();
		void close();

		int32_t connect();
		int32_t recieve(char* data, size_t len);
		int32_t send(const char* data, size_t len);
		size_t sendFrame(const char* data, size_t len);

	private:
		WSADATA m_wsaData = { 0 };
		SOCKET m_socket = { INVALID_SOCKET };
		const char* m_address = nullptr;
		const char* m_defaultPort = nullptr;
		struct addrinfo* m_connectionSettings = nullptr;
		size_t m_defaultLengthMessage = 0ul;
	};
} // ! namespace net

#endif // !_NET_CONNECTION_HPP_