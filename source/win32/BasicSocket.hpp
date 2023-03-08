#ifndef _NET_BASIC_SOCKET_HPP_
#define _NET_BASIC_SOCKET_HPP_

#include "socket_definition.hpp"

namespace net
{
	class BasicSocket
	{
	public:
		explicit BasicSocket();
		explicit BasicSocket(int32_t net_param, int32_t sock_type, int32_t connection_type);
		virtual ~BasicSocket();
		void close();

		SOCKET getSocket() const;
		int32_t send(const char* data, size_t len);
		int32_t recieve(char* data, size_t len);
		size_t sendFrame(const char* data, size_t len);

	private:
		void initWSA();

	private:
		WSADATA m_wsaData = {0};
		SOCKET m_socket = { INVALID_SOCKET };
		size_t m_lenMessage = { 0UL };
	};
} // ! namespace net

#endif // !_NET_BASIC_SOCKET_HPP_