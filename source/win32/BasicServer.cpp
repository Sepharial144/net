#include "BasicServer.hpp"

namespace net
{
	BasicServer::BasicServer()
	{
		std::cout << "BasicServer: create basic server ..." << &std::endl;
		std::cout << "BasicServer: create basic server ... complete" << &std::endl;
	}

	BasicServer::BasicServer(int32_t net_param, 
							 int32_t sock_type, 
							 int32_t connection_type, 
							 const char* port, 
							 const size_t default_len)
		:m_defaultLenMessage{ default_len }
	{
		std::cout << "BasicServer: create server ..." << &std::endl;

		std::cout << "BasicServer: init WSA ..." << &std::endl;
		int32_t res = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		if (res != 0) {
			std::cout << "BasicServer: WSAStartup failed with error : " << res << &std::endl;
		}
		std::cout << "BasicServer: init WSA ... complete" << &std::endl;

		struct addrinfo hints = { 0 };
		struct addrinfo* pAddrInfo = nullptr;

		// AF_INET, SOCK_STREAM, IPPROTO_TCP,
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE; // investigate this value

		res = ::getaddrinfo(nullptr, port, &hints, &pAddrInfo);
		if (res != 0)
		{
			std::cout << "Server getaddrinfo failed: " << WSAGetLastError() << &std::endl;
			::WSACleanup();
		}
		else
		{
			std::cout << "Server initializing ... complete" << &std::endl;
		}

		m_socket = ::socket(pAddrInfo->ai_family, pAddrInfo->ai_socktype, pAddrInfo->ai_protocol);
		if (m_socket == INVALID_SOCKET)
		{
			std::cout << "BasicServer: create server ... failed:" << WSAGetLastError() << &std::endl;
			freeaddrinfo(pAddrInfo);
			::WSACleanup();
		}
		else
			std::cout << "BasicServer: create server ... success" << &std::endl;

		std::cout << "Server bind ..." << &std::endl;
		//if (::bind(m_socket, (SOCKADDR*)&pAddrInfo, pAddrInfo->ai_addrlen) == SOCKET_ERROR)
		if (::bind(m_socket, pAddrInfo->ai_addr, static_cast<int>(pAddrInfo->ai_addrlen)) == SOCKET_ERROR)
		{
			std::cout << "Server bind ... failed: " << WSAGetLastError() << &std::endl;
			close();
			::WSACleanup();
		}

		freeaddrinfo(pAddrInfo);

		std::cout << "Server bind ... complete" << &std::endl;
	}

	BasicServer::~BasicServer()
	{
		std::cout << "BasicServer: destruct server ..." << &std::endl;
		close();
	}

	void BasicServer::close()
	{
		std::cout << "BasicServer: close server ..." << &std::endl;
		if (m_socket != INVALID_SOCKET)
		{
			std::cout << "BasicServer: close server ... failed: " << WSAGetLastError() << &std::endl;
			closesocket(m_socket);
		}
		std::cout << "BasicServer: close socket ... success" << &std::endl;
	}

	SOCKET BasicServer::getSocket() const
	{
		return m_socket;
	}


	int32_t BasicServer::send(const char* data, size_t len)
	{
		std::cout << "BasicServer: send ... " << &std::endl;
		return ::send(m_socket, data, len, 0);
		std::cout << "BasicServer: send ... complete" << &std::endl;
	}


	int32_t BasicServer::recieve(char* data, size_t len)
	{
		return ::recv(m_socket, data, len, 0);
	}


	size_t BasicServer::sendFrame(const char* data, size_t len)
	{
		size_t count = len / m_defaultLenMessage;
		size_t part = len - count * m_defaultLenMessage;
		size_t result = 0ul;
		for (size_t i = 0ul; i <= count; ++i)
		{
			size_t status = ::send(m_socket, data, m_defaultLenMessage, 0);
			if (status == m_defaultLenMessage)
			{
				result += status;
				data += m_defaultLenMessage;
			}
			else
			{
				return 0ul;
			}
		}
		//TODO: create inner function to send message
		if (part != 0ul)
			result += ::send(m_socket, data, part, 0);

		return (result == count * m_defaultLenMessage) ? result : 0ul;
	}
}