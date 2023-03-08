#ifndef _NET_BASIC_SERVER_HPP_
#define _NET_BASIC_SERVER_HPP_

#include "socket_definition.hpp"

namespace net
{
	class BasicServer
	{
	public:
		explicit BasicServer();
		explicit BasicServer(int32_t net_param, int32_t sock_type, int32_t connection_type, const char* port, const size_t default_len);
		virtual ~BasicServer();
		void close();

		SOCKET getSocket() const;
		int32_t send(const char* data, size_t len);
		int32_t recieve(char* data, size_t len);
		size_t sendFrame(const char* data, size_t len);

	private:
		WSADATA m_wsaData = { 0 };
		SOCKET m_socket = { INVALID_SOCKET };
		size_t m_defaultLenMessage = { 0 };
	};
} // ! namespace net

#endif // !_NET_BASIC_SERVER_HPP_