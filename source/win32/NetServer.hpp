#ifndef _NET_SERVER_HPP_
#define _NET_SERVER_HPP_

#include "NetClient.hpp"
#include "socket_definition.hpp"

namespace net
{
	class server final
	{
	public:
		explicit server(net::addrinfo::SockSetting& setting, 
						const char* addr, 
						const char* port, 
						const size_t default_len);
		~server();

		void close();

		int32_t waitConnection(client& client);

	private:
		net::addrinfo::SockSetting m_serverSetting;
		WSADATA m_wsaData = { 0 };
		SOCKET m_socket = { INVALID_SOCKET };
		const char* m_address = nullptr;
		const char* m_defaultPort = nullptr;
	};
} // ! namespace net

#endif // !_NET_SERVER_HPP_