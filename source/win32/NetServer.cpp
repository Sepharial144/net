#include "NetServer.hpp"
#include "exceptions/SocketException.hpp"

namespace net
{
	server::server(const net::addrinfo::SockSetting& setting, const int32_t port, const size_t default_len)
		:m_serverSetting(setting),
		m_address{ nullptr },
		m_defaultPort{ port }
	{
		if (port < static_cast<int32_t>(0))
			throw std::logic_error("Netlib: server port should not be negative");

		std::cout << "Server initializing ..." << &std::endl;
		initializationWSA();

		sockaddr_in address = { 0 };
		address.sin_family = m_serverSetting.aiFamily;
		address.sin_port = htons(port);
		address.sin_addr.s_addr = INADDR_ANY;

		m_socket = ::socket(m_serverSetting.aiFamily, m_serverSetting.aiSocktype, m_serverSetting.aiProtocol);
		if (m_socket == INVALID_SOCKET) {
			// TODO: clean from message
			std::cout << "Server socket failed: " << WSAGetLastError() << &std::endl;
			throw net::exception("Netlib: server create socket failed");
		}
		if (int32_t ret = ::bind(m_socket, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR)
		{
			// TODO: clean from message
			std::cout << "Server bind ... failed: " << WSAGetLastError() << &std::endl;
			close();
			throw net::exception("Netlib: server bind socket failed", ret);
		}

		int32_t countConnections = 10; // SOMAXCONN;
		std::cout << "Server set connections ... " << countConnections << &std::endl;
		listening(countConnections);
		std::cout << "Server initializing ... complete" << &std::endl;
	}

	server::server(const net::addrinfo::SockSetting& setting, const char* addr, const char* port, const size_t default_len)
		:m_serverSetting(setting),
		 m_address{ addr },
		 m_defaultPort{ port }
	{
		std::cout << "Server initializing ..." << &std::endl;

		initializationWSA();

		struct addrinfo hints = { 0 };
		struct addrinfo* pAddrInfo = nullptr;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = m_serverSetting.aiFamily;
		hints.ai_socktype = m_serverSetting.aiSocktype;
		hints.ai_protocol = m_serverSetting.aiProtocol;
		hints.ai_flags = m_serverSetting.aiFlags; // investigate this value


		// TODO: implement certain address
		if (int32_t ret = ::getaddrinfo(nullptr, port, &hints, &pAddrInfo) != 0)
		{
			// TODO: clean from message
			std::cout << "Server getaddrinfo failed: " << WSAGetLastError() << "ret value: " << ret << &std::endl;
			throw net::exception("Netlib: server get addrinfo failed", ret);
		}
		std::cout << "Server getaddrinfo ... complete" << &std::endl;

		std::cout << "Server create socket ... " << &std::endl;
		m_socket = ::socket(pAddrInfo->ai_family, pAddrInfo->ai_socktype, pAddrInfo->ai_protocol);
		if (m_socket == INVALID_SOCKET) {
			// TODO: clean from message
			std::cout << "Server socket failed: " << WSAGetLastError() << &std::endl;
			freeaddrinfo(pAddrInfo);
			throw net::exception("Netlib: server create socket failed");
		}
		std::cout << "Server create socket ... complete" << &std::endl;

		std::cout << "Server bind ... " << &std::endl;
		if (int32_t ret = ::bind(m_socket, pAddrInfo->ai_addr, static_cast<int>(pAddrInfo->ai_addrlen)) == SOCKET_ERROR)
		{
			// TODO: clean from message
			std::cout << "Server bind ... failed: " << WSAGetLastError() << &std::endl;
			close();
			throw net::exception("Netlib: server bind failed", ret);
		}
		std::cout << "Server bind... complete" << &std::endl;

		// TODO: reorganize set connections
		int32_t countConnections = 10; // SOMAXCONN;
		std::cout << "Server set connections ... " << countConnections << &std::endl;
		listening(countConnections);
		freeaddrinfo(pAddrInfo);
		std::cout << "Server initializing ... complete" << &std::endl;
	}

	void server::initializationWSA()
	{
		std::cout << "Server initialization WSA ..." << &std::endl;
		if (int32_t ret = ::WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0) {
			// TODO: clean from message
			std::cout << "Server WSAStartup failed with error : " << ret << &std::endl;
			throw net::exception("Netlib: server initialization WSA failed", ret);
		}
		std::cout << "Server initialization WSA ... complete" << &std::endl;
	}

	void server::listening(const int32_t count_connections)
	{
		// TODO: realize count connections in strcuture or different function
		std::cout << "Server listening ... " << &std::endl;
		if (int32_t ret = ::listen(m_socket, count_connections) == SOCKET_ERROR)
		{
			// TODO: clean from message
			std::cout << "Server listening ... error: " << WSAGetLastError() << &std::endl;
			throw net::exception("Netlib: server initialization WSA failed", ret);
		}
		std::cout << "Server listening ... complete" << &std::endl;
	}

	server::~server()
	{
		close();
	}

	void server::close()
	{
		std::cout << "Server close socket ..." << &std::endl;
		if (m_socket != INVALID_SOCKET)
		{
			std::cout << "Server socket is not invalid, close ... " << &std::endl;
			if (int32_t ret = closesocket(m_socket) != SOCKET_ERROR)
			{
				throw net::exception("Netlib: server socket close failed", ret);
			}
			m_socket = INVALID_SOCKET;
		}
		WSACleanup();
		std::cout << "Server close socket ... complete" << &std::endl;
	}


	int32_t server::waitConnection(net::client& client)
	{
		std::cout << "Server wait connection ... " << &std::endl;
		client.m_socket = SOCKET_ERROR;

		client.m_socket = ::accept(m_socket, nullptr, nullptr);

		if (client.m_socket == SOCKET_ERROR)
		{
			std::cout << "Server client socket error: " << &std::endl;
			return SOCKET_ERROR;
		}

		std::cout << "Server connected client socket id ... " << client.m_socket << &std::endl;
		return 1;
	}
} // ! namespace net