#include "NetConnection.hpp"
#include "exceptions/SocketException.hpp"

namespace net
{
	// TODO: create connection for different family
	connection::connection(const char* addr, const char* port)
		:m_address{ addr },
		 m_defaultPort{ port }
	{
		std::cout << "Connection initialization ..." << &std::endl;

		std::cout << "Connection: init WSA ..." << &std::endl;
		if (int32_t ret = ::WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0) {
			throw net::exception("Netlib: connection init WSA failed", ret);
		}
		std::cout << "Connection: init WSA ... complete" << &std::endl;

		struct addrinfo hints = { 0 };

		// TODO: create connection for different family
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// TODO: check getaddrinfo is correct
		if (int32_t ret = ::getaddrinfo(nullptr, port, &hints, &m_connectionSettings) != 0) {
			throw net::exception("Netlib: connection getaddrinfo failed", ret);
		}

		m_socket = ::socket(m_connectionSettings->ai_family, m_connectionSettings->ai_socktype, m_connectionSettings->ai_protocol);
		if (m_socket == INVALID_SOCKET)
		{
			throw net::exception("Netlib: connection create socket failed");
		}
		std::cout << "Connection: create socket ... complete" << &std::endl;
		std::cout << "Connection initialization ... complete" << &std::endl;
	}


	connection::~connection()
	{
		close();
	}


	void connection::close()
	{
		std::cout << "Connection: close socket ..." << &std::endl;
		if (m_connectionSettings != nullptr)
		{
			::freeaddrinfo(m_connectionSettings);
			m_connectionSettings = nullptr;
		}

		if (m_socket != INVALID_SOCKET)
		{
			if (int32_t ret = ::closesocket(m_socket))
			{
				throw net::exception("Netlib: connection close socket failed");
			}
			m_socket = INVALID_SOCKET;
			::WSACleanup();
		}
		std::cout << "Connection: close socket ... comlete" << &std::endl;
	}


	int32_t connection::connect()
	{
		std::cout << "Connection to server ..." << &std::endl;
		// TODO: create non-bloking sockets
		if (int32_t ret = ::connect(m_socket, m_connectionSettings->ai_addr, m_connectionSettings->ai_addrlen) == SOCKET_ERROR)
		{
			// TODO: there is no reason to use cout message or will use throw instead return status
			printf("Client::connect() - Failed to connect.\n");
			std::cout << "Connection: failed to connect" << &std::endl;
			::WSACleanup();
			return SOCKET_ERROR;
		}
		printf("Client::connect() - success.\n");
		std::cout << "Connection to server ... complete" << &std::endl;
		return 1;
	}


	int32_t connection::recieve(char* data, size_t len)
	{
		return ::recv(m_socket, data, len, 0);
	}


	int32_t connection::send(const char* data, size_t len)
	{
		return ::send(m_socket, data, len, 0);
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