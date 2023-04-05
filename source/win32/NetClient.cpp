#include "NetServer.hpp"
#include "NetClient.hpp"
#include "algo.hpp"
#include "exceptions/SocketException.hpp"

namespace net
{
	client::client(const size_t len_message)
		: m_socket{ INVALID_SOCKET }
		, m_lenMessage{ len_message }
		, m_sockaddrStorage{ 0 }
		, m_address{ 0 }
	{
	}

	client::~client()
	{
		close();
	}

	void client::close()
	{
		std::cout << "client socket: close socket ..." << &std::endl;
		shutdown(net::socket::both);

		if (m_socket != INVALID_SOCKET)
		{
			std::cout << "client socket: socket is not invalid, try close ..." << &std::endl;
			if (int32_t ret = ::closesocket(m_socket) == SOCKET_ERROR)
			{
				throw net::exception("Netlib: client close socket failed", ret);
			}
			::WSACleanup();
			m_socket = INVALID_SOCKET;
		}
		std::cout << "client socket: close socket ... complete" << &std::endl;
	}

	void client::shutdown(net::socket::shutdownMode param)
	{
		// TODO: check different calls 
		std::cout << "client socket: shutdown socket ... " << &std::endl;
		if (int32_t ret = ::shutdown(m_socket, param) == SOCKET_ERROR) {
			throw net::exception("Netlib: client shutdown socket failed", ret);
		}
		std::cout << "client socket: shutdown socket ... complete" << &std::endl;
	}

	int32_t client::recieve(char* data, size_t len)
	{
		return ::recv(m_socket, data, len, 0);
	}

	int32_t client::send(const char* data, size_t len)
	{
		return ::send(m_socket, data, len, 0);
	}

	void client::interpretFamily()
	{
		if (m_familyType == net::addrinfo::inetv4)
		{
			sockaddr_in* ptrIpv4ClientAddr = reinterpret_cast<sockaddr_in*>(&m_sockaddrStorage);
			m_address.addr_size = static_cast<size_t>(INET_ADDRSTRLEN);
			m_address.port = ntohs(ptrIpv4ClientAddr->sin_port);
			inet_ntop(net::addrinfo::inetv4, ptrIpv4ClientAddr, reinterpret_cast<PSTR>(m_address.address), m_address.addr_size);
			return;
		}
		if (m_familyType == net::addrinfo::inetv6)
		{
			sockaddr_in6* ptrIpv6ClientAddr = reinterpret_cast<sockaddr_in6*>(&m_sockaddrStorage);
			m_address.addr_size = static_cast<size_t>(INET6_ADDRSTRLEN);
			m_address.port = ntohs(ptrIpv6ClientAddr->sin6_port);
			inet_ntop(net::addrinfo::inetv6, ptrIpv6ClientAddr, reinterpret_cast<PSTR>(m_address.address), m_address.addr_size);
			return;
		}
		else
		{
			throw std::runtime_error("Netlib: client could not recognize address family");
		}
	}

} // ! namespace net