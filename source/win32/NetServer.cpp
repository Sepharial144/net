#include "NetServer.hpp"

namespace net
{

	server::server(net::addrinfo::SockSetting& setting, const char* addr, const char* port, const size_t default_len)
		:m_serverSetting(setting),
		 m_address{ addr },
		 m_defaultPort{ port }
	{
		std::cout << "Server initializing ..." << &std::endl;

		std::cout << "Server init WSA ..." << &std::endl;
		int32_t res = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		if (res != 0) {
			std::cout << "Server WSAStartup failed with error : " << res << &std::endl;
		}
		std::cout << "Server init WSA ... complete" << &std::endl;

		struct addrinfo hints = { 0 };
		struct addrinfo* pAddrInfo = nullptr;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = m_serverSetting.aiFamily;
		hints.ai_socktype = m_serverSetting.aiSocktype;
		hints.ai_protocol = m_serverSetting.aiProtocol;
		hints.ai_flags = m_serverSetting.aiFlags; // investigate this value

		res = ::getaddrinfo(nullptr, port, &hints, &pAddrInfo);
		if (res != 0)
		{
			std::cout << "Server getaddrinfo failed: " << WSAGetLastError() << &std::endl;
			::WSACleanup();
		}
		else
		{
			std::cout << "Server getaddrinfo ... complete" << &std::endl;
		}

		std::cout << "Server create socket ... " << &std::endl;
		m_socket = ::socket(pAddrInfo->ai_family, pAddrInfo->ai_socktype, pAddrInfo->ai_protocol);
		if (m_socket == INVALID_SOCKET) {
			std::cout << "Server socket failed: " << WSAGetLastError() << &std::endl;
			freeaddrinfo(pAddrInfo);
			WSACleanup();
		}
		else
		{
			std::cout << "Server create socket ... complete" << &std::endl;
		}

		std::cout << "Server bind ... " << &std::endl;
		if (::bind(m_socket, pAddrInfo->ai_addr, static_cast<int>(pAddrInfo->ai_addrlen)) == SOCKET_ERROR)
		{
			std::cout << "Server bind ... failed: " << WSAGetLastError() << &std::endl;
			close();
			::WSACleanup();
		}
		else
		{
			std::cout << "Server bind... complete" << &std::endl;
		}

		freeaddrinfo(pAddrInfo);

		int32_t countConnections = 10; // SOMAXCONN;
		std::cout << "Server set connections ... " << countConnections << &std::endl;
		std::cout << "Server listening ... " << &std::endl;
		if (::listen(m_socket, countConnections) == SOCKET_ERROR)
		{
			std::cout << "Server listening ... error: " << WSAGetLastError() << &std::endl;
		}
		else
		{
			std::cout << "Server listening ... complete" << &std::endl;
		}

		std::cout << "Server initializing ... complete" << &std::endl;
	}

	server::~server()
	{
		close();
	}

	void server::close()
	{
		std::cout << "Server: close socket ..." << &std::endl;
		if (m_socket != INVALID_SOCKET)
		{
			std::cout << "Server: close socket ... failed" << WSAGetLastError() << &std::endl;
			closesocket(m_socket);
		}
		WSACleanup();
		std::cout << "Server: close socket ... success" << &std::endl;
	}


	int32_t server::waitConnection(net::client& client)
	{
		std::cout << "Server wait connection ... " << &std::endl;
		client.m_socket = SOCKET_ERROR;

		// TODO implement count connections
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