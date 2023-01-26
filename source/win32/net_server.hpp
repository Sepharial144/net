#ifndef _NET_SERVER_HPP_
#define _NET_SERVER_HPP_

#include "win32_definitions.hpp"
#include "socket_context.hpp"
#include "net_client.hpp"

namespace net
{
	class server
	{
	public:
		explicit server();
		explicit server(const char* addr, const char* port);
		~server();

		void listening();
		int32_t waitConnection(client& client);

	private:
		WSADATA m_wsaData;
		SOCKET m_socket;
		SOCKET m_clientSocket;
		const char* m_address;
		const char* m_defaultPort;
		sockaddr_in m_serverSettings;
	};
} // ! namespace net

#endif // !_NET_SERVER_HPP_