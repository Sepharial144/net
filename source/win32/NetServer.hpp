#ifndef _NET_SERVER_HPP_
#define _NET_SERVER_HPP_

#include "NetClient.hpp"
#include "socket_definition.hpp"

#include <exception>
#include <variant>
#include <type_traits>


namespace net
{
	class server final
	{
	public:
		explicit server(const net::addrinfo::SockSetting& settings, 
						const int32_t port, 
						const size_t default_len);

		explicit server(const net::addrinfo::SockSetting& settings, 
						const char* addr, 
						const char* port, 
						const size_t default_len);
		~server();

		void close();
		int32_t waitConnection(client& client);

	private:
		void initWSA();

		void initListeningSocket(PADDRINFOA* pAddrInfo, int32_t family, int32_t socket_type, int32_t protocol, sockaddr* ai_address, int32_t ai_addrlen);
		void listening(const int32_t count_connections);

	private:
		net::addrinfo::SockSetting m_serverSetting;
		WSADATA m_wsaData = { 0 };
		SOCKET m_socket = { INVALID_SOCKET };
		const char* m_address = nullptr;
		std::variant<int32_t, const char*> m_defaultPort = nullptr;
	};

} // ! namespace net

#endif // !_NET_SERVER_HPP_