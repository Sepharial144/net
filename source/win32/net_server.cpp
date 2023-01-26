#include "net_server.hpp"

namespace net
{
	server::server()
		:m_wsaData{0},
		 m_socket{ INVALID_SOCKET },
		 m_clientSocket{ INVALID_SOCKET },
		 m_address { nullptr},
		 m_defaultPort{ nullptr }
	{
		int32_t res = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		if (res != NO_ERROR)
		{
			printf("Server: Error at WSAStartup().\n");
			WSACleanup();
		}
		else
			printf("Server: WSAStartup() is OK.\n");
	}

	server::server(const char* addr, const char* port)
		:m_wsaData{ 0 },
		 m_address{ addr },
		 m_defaultPort{ port },
		 m_socket{ INVALID_SOCKET },
		 m_clientSocket{ INVALID_SOCKET }
	{
		int32_t res = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		if (res != NO_ERROR)
		{
			printf("Server: Error at WSAStartup().\n");
			WSACleanup();
		}
		else
			printf("Server: WSAStartup() is OK.\n");

		m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_socket == INVALID_SOCKET)
		{
			printf("Server: Error at socket(): %ld\n", WSAGetLastError());
			WSACleanup();
		}
		else
			printf("Server: socket() is OK.\n");

		m_serverSettings.sin_family = AF_INET;
		m_serverSettings.sin_addr.s_addr = inet_addr(m_address);
		// TODO: need to traslate number of port
		m_serverSettings.sin_port = htons(3000);
	}

	void server::listening()
	{
		if (::bind(m_socket, (SOCKADDR*)&m_serverSettings, sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
			printf("Server: bind() failed.\n");
			WSACleanup();
		}
		else
			printf("Server: bind() is OK.\n");

		int32_t countConnections = 10;
		if (::listen(m_socket, countConnections) == SOCKET_ERROR)
		{
			printf("Server: Error listening on socket.\n");
			WSACleanup();
		}
		else
			printf("Server: listen() is OK.\n");
	}

	server::~server()
	{
		closesocket(m_socket);
		WSACleanup();
	}

	int32_t server::waitConnection(net::client& client)
	{
		listening();
		client.m_socket = SOCKET_ERROR;
		while (client.m_socket == SOCKET_ERROR)
		{
			client.m_socket = accept(m_socket, nullptr, nullptr);
		}
		if (client.m_socket == SOCKET_ERROR)
		{
			std::cout << "Socket error: " << std::endl;
			return 0;
		}
		return 1;
	}
} // ! namespace net