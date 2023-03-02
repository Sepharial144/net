#include "net_server.hpp"
#include "BasicSocket.hpp"

namespace net
{
	server::server(const char* addr, const char* port)
		:m_address{ addr },
		 m_defaultPort{ port }
	{
		SOCKET& socket = socketReference();
		socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (socket == INVALID_SOCKET)
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

	server::~server()
	{
	}

	void server::listening()
	{
		SOCKET& socket = socketReference();
		if (::bind(socket, (SOCKADDR*)&m_serverSettings, sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
			printf("Server: bind() failed.\n");
		}
		else
			printf("Server: bind() is OK.\n");

		int32_t countConnections = 10;
		if (::listen(socket, countConnections) == SOCKET_ERROR)
		{
			printf("Server: Error listening on socket.\n");
		}
		else
			printf("Server: listen() is OK.\n");
	}

	int32_t server::waitConnection(net::client& client)
	{
		listening();
		SOCKET& socket = socketReference();
		client.m_socket = SOCKET_ERROR;
		while (client.m_socket == SOCKET_ERROR)
		{
			client.m_socket = accept(socket, nullptr, nullptr);
		}
		if (client.m_socket == SOCKET_ERROR)
		{
			std::cout << "Socket error: " << std::endl;
			return 0;
		}
		return 1;
	}
} // ! namespace net