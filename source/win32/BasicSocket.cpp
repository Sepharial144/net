#include "BasicSocket.hpp"

namespace net
{
	BasicSocket::BasicSocket()
	{
		std::cout << "BasicSocket: create basic socket ..." << &std::endl;
		initWSA();
		std::cout << "BasicSocket: create basic socket ... complete" << &std::endl;
	}

	BasicSocket::BasicSocket(int32_t net_param, int32_t sock_type, int32_t connection_type)
	{
		std::cout << "BasicSocket: create socket ..." << &std::endl;
		initWSA();

		//m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		m_socket = ::socket(net_param, sock_type, connection_type);
		if (m_socket == INVALID_SOCKET)

		{
			std::cout << "BasicSocket: create socket ... failed:" << WSAGetLastError() << &std::endl;
			WSACleanup();
		}
		else
			std::cout << "BasicSocket: create socket ... success" << &std::endl;
	}

	void BasicSocket::initWSA()
	{
		std::cout << "BasicSocket: init WSA ..." << &std::endl;
		int32_t res = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		if (res != 0) {
			std::cout << "BasicSocket: WSAStartup failed with error : " << res << std::endl;
		}
		std::cout << "BasicSocket: init WSA ... complete" << &std::endl;
	}


	BasicSocket::~BasicSocket()
	{
		std::cout << "BasicSocket: create socket ..." << &std::endl;
		close();
	}


	void BasicSocket::close()
	{
		std::cout << "BasicSocket: close socket ..." << &std::endl;
		if (m_socket != INVALID_SOCKET)
		{
			std::cout << "BasicSocket: close socket ... failed" << WSAGetLastError() << &std::endl;
			closesocket(m_socket);
		}
		WSACleanup();
		std::cout << "BasicSocket: close socket ... success" << &std::endl;
	}

	SOCKET BasicSocket::getSocket() const
	{
		return m_socket;
	}


	int32_t BasicSocket::send(const char* data, size_t len)
	{
		std::cout << "BasicSocket: send ... " << &std::endl;
		return ::send(m_socket, data, len, 0);
		std::cout << "BasicSocket: send ... complete" << &std::endl;
	}


	int32_t BasicSocket::recieve(char* data, size_t len)
	{
		return ::recv(m_socket, data, len, 0);
	}


	size_t BasicSocket::sendFrame(const char* data, size_t len)
	{
		size_t count = len / m_lenMessage;
		size_t part = len - count * m_lenMessage;
		size_t result = 0ul;
		for (size_t i = 0ul; i <= count; ++i)
		{
			size_t status = ::send(m_socket, data, m_lenMessage, 0);
			if (status == m_lenMessage)
			{
				result += status;
				data += m_lenMessage;
			}
			else
			{
				return 0ul;
			}
		}
		if (part != 0ul)
			result += ::send(m_socket, data, part, 0);

		return (result == count * m_lenMessage) ? result : 0;
	}
}