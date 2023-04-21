#include "algo.hpp"
#include "net.hpp"
#include "exceptions/SocketException.hpp"

namespace net
{
	client::client()
		: m_sockaddrStorage{ 0 }
		, m_socket { INVALID_SOCKET }
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

		if (m_socket != INVALID_SOCKET)
		{
			shutdown(net::enumShutdown::both);
			std::cout << "client socket: socket is not invalid, try close ..." << &std::endl;
			int32_t ret = ::closesocket(m_socket);
			net::throw_exception_on(ret == SOCKET_ERROR, "Netlib: client close socket failed");
			::WSACleanup();
			m_socket = INVALID_SOCKET;
		}
		std::cout << "client socket: close socket ... complete" << &std::endl;
	}

	void client::shutdown(net::enumShutdown param)
	{
		// TODO: check different calls 
		std::cout << "client socket: shutdown socket ... " << &std::endl;
		int32_t ret = ::shutdown(m_socket, static_cast<int>(param));
		net::throw_exception_on(ret == SOCKET_ERROR, "Netlib: client shutdown socket failed");
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

} // ! namespace net