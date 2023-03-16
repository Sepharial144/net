#include "NetConnection.hpp"

namespace net
{
	connection::connection(const char* addr, const char* port)
		:m_address{ addr },
		 m_defaultPort{ port }
	{
		//m_connectionSettings.sin_family = AF_INET;
		//m_connectionSettings.sin_addr.s_addr = ::inet_addr(m_address);
		//m_connectionSettings.sin_port = ::htons(m_defaultPort);

		std::cout << "Connection: init WSA ..." << &std::endl;
		int32_t res = ::WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		if (res != 0) {
			std::cout << "Connection: WSAStartup failed with error : " << res << std::endl;
		}
		std::cout << "Connection: init WSA ... complete" << &std::endl;

		struct addrinfo hints = { 0 };

		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// TODO: check getaddrinfo is correct
		res = ::getaddrinfo(nullptr, port, &hints, &m_connectionSettings);
		if (res != 0) {
			std::cout << "Connection error while getaddrinfo: " << ::WSAGetLastError() << std::endl;
			::WSACleanup();
		}

		m_socket = ::socket(m_connectionSettings->ai_family, m_connectionSettings->ai_socktype, m_connectionSettings->ai_protocol);
		if (m_socket == INVALID_SOCKET)

		{
			std::cout << "Connection: create socket ... failed:" << ::WSAGetLastError() << &std::endl;
			::WSACleanup();
		}
		else
			std::cout << "Connection: create socket ... success" << &std::endl;
	}


	connection::~connection()
	{
		close();
	}


	void connection::close()
	{
		std::cout << "Connection: close socket ..." << &std::endl;
		if (m_socket != INVALID_SOCKET)
		{
			std::cout << "Connection: close socket ... failed" << WSAGetLastError() << &std::endl;
			::closesocket(m_socket);
			::freeaddrinfo(m_connectionSettings);
		}
		::WSACleanup();
		std::cout << "Connection: close socket ... success" << &std::endl;
	}


	int32_t connection::connect()
	{
		std::cout << "Connection socket: " << m_socket << &std::endl;
		if (::connect(m_socket, m_connectionSettings->ai_addr, m_connectionSettings->ai_addrlen) == SOCKET_ERROR)
		{
			printf("Client::connect() - Failed to connect.\n");
			::WSACleanup();
			return -1;
		}
		printf("Client::connect() - success.\n");
		return 1;
	}


	int32_t connection::recieve(char* data, size_t len)
	{
		std::cout << "Connection: recieve ... " << m_socket << &std::endl;
		int res = ::recv(m_socket, data, len, 0);
		std::cout << "Connection: recieve res ... " << res << &std::endl;
		return res;
	}


	int32_t connection::send(const char* data, size_t len)
	{
		std::cout << "Connection: send ... " << m_socket << &std::endl;
		int32_t res = ::send(m_socket, data, len, 0);
		std::cout << "Connection: send ... complete" << &std::endl;
		return res;
	}

	size_t connection::sendFrame(const char* data, size_t len)
	{
		size_t count = len / m_defaultLengthMessage;
		size_t part = len - count * m_defaultLengthMessage;
		size_t result = 0ul;
		for (size_t i = 0ul; i <= count; ++i)
		{
			size_t status = ::send(m_socket, data, m_defaultLengthMessage, 0);
			if (status == m_defaultLengthMessage)
			{
				result += status;
				data += m_defaultLengthMessage;
			}
			else
			{
				return 0ul;
			}
		}
		//TODO: create inner function to send message
		if (part != 0ul)
			result += ::send(m_socket, data, part, 0);

		return (result == count * m_defaultLengthMessage) ? result : 0ul;
	}


} // ! namespace net