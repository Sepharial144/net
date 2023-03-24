#include "NetServer.hpp"
#include "NetClient.hpp"
#include "algo.hpp"
#include "exceptions/SocketException.hpp"

namespace net
{
	client::client(const size_t len_message)
		: m_socket{ INVALID_SOCKET }
		, m_lenMessage{ len_message }
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
			m_socket = INVALID_SOCKET;
		}
		::WSACleanup();
		std::cout << "client socket: close socket ... success" << &std::endl;
	}

	void client::shutdown(net::socket::shutdownMode param)
	{
		std::cout << "client socket: shutdown socket parameter: "<< param << " ... " << &std::endl;
		if (int32_t ret = ::shutdown(m_socket, param) == SOCKET_ERROR) {
			// TODO: implement doing with error if close socket
			throw net::exception("Netlib: clientshutdown socket failed", ret);
			//std::cout << "Client shutdown failed with error: " << ::WSAGetLastError() << &std::endl;
			//::closesocket(m_socket);
			//::WSACleanup();
			return;
		}
		std::cout << "client socket: shutdown socket ... complete" << &std::endl;
	}

	int32_t client::recieve(char* data, size_t len)
	{
		std::cout << "client socket: recieve ... " << m_socket << &std::endl;
		int res = ::recv(m_socket, data, len, 0);
		std::cout << "client socket: recieve res ... " << res << &std::endl;
		return res;
	}

	int32_t client::send(const char* data, size_t len)
	{
		std::cout << "client socket: send ... " << m_socket << &std::endl;
		return ::send(m_socket, data, len, 0);
		std::cout << "client socket: send ... complete" << &std::endl;
	}

} // ! namespace net