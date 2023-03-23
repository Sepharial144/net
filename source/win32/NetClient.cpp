#include "NetServer.hpp"
#include "NetClient.hpp"
#include "algo.hpp"

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
		if (m_socket != INVALID_SOCKET)
		{
			std::cout << "client socket: close socket ... failed" << ::WSAGetLastError() << &std::endl;
			::closesocket(m_socket);
		}
		::WSACleanup();
		std::cout << "client socket: close socket ... success" << &std::endl;
	}

	void client::shutdown(const int16_t param)
	{
		// param
		// SD_RECEIVE
		// SD_SEND
		// SD_BOTH
		if (::shutdown(m_socket, param) == SOCKET_ERROR) {
			std::cout << "Client shutdown failed with error: " << ::WSAGetLastError() << &std::endl;
			::closesocket(m_socket);
			::WSACleanup();
			return;
		}
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