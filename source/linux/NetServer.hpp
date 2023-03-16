#ifndef _NET_SERVER_HPP_
#define _NET_SERVER_HPP_

#include "linux_definitions.hpp"
#include "socket_definition.hpp"
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
		int m_socket;
		int m_clientSocket;
		const char* m_address;
		const char* m_defaultPort;
		struct sockaddr_in m_serverSettings;
	};
} // ! namespace net

#endif // !_NET_SERVER_HPP_