#include "net_server.hpp"
#include "BasicSocket.hpp"

namespace net
{
	server::server(const char* addr, int32_t port)
		:BasicSocket{ AF_INET, SOCK_STREAM, IPPROTO_TCP },
		 m_address{ addr },
		 m_defaultPort{ port }
	{
		m_serverSettings.sin_family = AF_INET;
		m_serverSettings.sin_addr.s_addr = inet_addr(m_address);
		// TODO: need to traslate number of port
		m_serverSettings.sin_port = htons(port);
	}


	server::~server()
	{
		close();
	}


	void server::listening()
	{
		std::cout << "Server bind ..." << &std::endl;
		if (::bind(getSocket(), (SOCKADDR*)&m_serverSettings, sizeof(sockaddr_in)) == SOCKET_ERROR)
		{
			std::cout << "Server bind ... failed" << WSAGetLastError() << &std::endl;
			return;
		}
		std::cout << "Server bind ... complete" << &std::endl;

		// TODO: create mehcanizm for max connections
		int32_t countConnections = 10;
		std::cout << "Server set connections ... " << countConnections << &std::endl;
		std::cout << "Server listening ... " << &std::endl;
		if (::listen(getSocket(), countConnections) == SOCKET_ERROR)
		{
			std::cout << "Server listening ... error" << WSAGetLastError() << &std::endl;
			return;
		}

		std::cout << "Server listening ... complete" << WSAGetLastError() << &std::endl;
	}

	int32_t server::waitConnection(net::client& client)
	{
		listening();
		std::cout << "Server wait connection ... " << WSAGetLastError() << &std::endl;
		SOCKET sock = client.getSocket();
		// TODO: create exception when socket is not error;
		sock = SOCKET_ERROR;
		while (sock == SOCKET_ERROR)
		{
			sock = accept(getSocket(), nullptr, nullptr);
		}
		if (sock == SOCKET_ERROR)
		{
			std::cout << "Socket error: " << &std::endl;
			return 0;
		}
		return 1;
	}
} // ! namespace net