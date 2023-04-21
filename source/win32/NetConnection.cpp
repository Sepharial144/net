#include "net.hpp"
#include "win32_common_api.hpp"
#include "exceptions/SocketException.hpp"

namespace net
{
	// TODO: create connection for different family
	connection::connection(const char* addr, const char* port)
		: m_wsaData{ 0 }
		, m_connectionSettings{ nullptr }
		, m_socket{ INVALID_SOCKET }
		, m_address { addr }
		, m_defaultPort{ port }
		, m_defaultLengthMessage{ 0ul }
	{
		std::cout << "Connection initialization ..." << &std::endl;

		net::api::initializeWSA(m_wsaData);

		struct addrinfo hints = { 0 };

		// TODO: create connection for different family
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// TODO: check getaddrinfo is correct
		std::cout << "Connection: get addrinfo ..." << &std::endl;
		int32_t ret = ::getaddrinfo(nullptr, port, &hints, &m_connectionSettings);
		net::throw_exception_on(ret != 0, "Netlib: connection getaddrinfo failed");
		std::cout << "Connection: get addrinfo ... complete" << &std::endl;

		std::cout << "Connection: create socket ..." << &std::endl;
		m_socket = ::socket(m_connectionSettings->ai_family, m_connectionSettings->ai_socktype, m_connectionSettings->ai_protocol);
		net::throw_exception_on(m_socket == INVALID_SOCKET, "Netlib: connection create socket failed");
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
			int32_t ret = ::closesocket(m_socket);
			net::throw_exception_on(ret != SOCKET_ERROR, "Netlib: connection close socket failed");
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
			std::cout << "Connection: failed to connect" << &std::endl;
			return SOCKET_ERROR;
		}
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