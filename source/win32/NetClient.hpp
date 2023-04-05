#ifndef _NET_CLIENT_HPP_
#define _NET_CLIENT_HPP_

#include "socket_definition.hpp"

namespace net
{
	struct ipAddress
	{
		u_short port;
		uint8_t address[INET6_ADDRSTRLEN];
		size_t addr_size;
		net::addrinfo::aifamily type;
	};

	class client
	{
	private:
		friend class server;

	public:
		explicit client(const size_t len_message);
		~client();
		void close();
		void shutdown(net::socket::shutdownMode param);
		int32_t recieve(char* data, size_t len);
		int32_t send(const char* data, size_t len);

	private:
		void interpretFamily();

	private:
		SOCKET m_socket = { INVALID_SOCKET };
		size_t m_lenMessage = {0ul};
		sockaddr_storage m_sockaddrStorage = {0};
		net::addrinfo::aifamily m_familyType;
		ipAddress m_address = {0};
	};
} // ! namespace net

#endif // !_NET_CLIENT_HPP_