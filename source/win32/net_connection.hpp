#ifndef _NET_CONNECTION_HPP_
#define _NET_CONNECTION_HPP_

#include "win32_definitions.hpp"
#include "socket_definition.hpp"
#include "net_client.hpp"

namespace net
{
	namespace socket 
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
			WSADATA m_wsaData;
			SOCKET m_socket;
			const char* m_address;
			const char* m_defaultPort;
			sockaddr_in m_connectionSettings;
		};
	} // ! namespace socket
} // ! namespace net

#endif // !_NET_CONNECTION_HPP_