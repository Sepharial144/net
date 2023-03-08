#include "net_server.hpp"
#include "BasicServer.hpp"

namespace net
{
	server::server(const char* addr, const char* port, const size_t default_len)
		:BasicServer{ AF_INET, SOCK_STREAM, IPPROTO_TCP, port, default_len },
		 m_address{ addr },
		 m_defaultPort{ port }
	{
		std::cout << "Server initializing ..." << &std::endl;
		std::cout << "Server initializing ... complete" << &std::endl;
	}

	server::~server()
	{
		close();
	}

	void server::listening()
	{
		// TODO: create mehcanizm for max connections
		int32_t countConnections = 10; // SOMAXCONN;
		std::cout << "Server set connections ... " << countConnections << &std::endl;
		std::cout << "Server listening ... " << &std::endl;
		std::cout << "Server listening socket size: " << sizeof(SOCKET) << std::endl;
		if (::listen(getSocket(), countConnections) == SOCKET_ERROR)
		{
			std::cout << "Server listening ... error: " << WSAGetLastError() << &std::endl;
			return;
		}

		std::cout << "Server listening ... complete" << &std::endl;
	}

	int32_t server::waitConnection(net::client& client)
	{
		listening();
		std::cout << "Server wait connection ... " << &std::endl;
		SOCKET sock = client.getSocket();
		// TODO: create exception when socket is not error;
		sock = SOCKET_ERROR;
		//while (sock == SOCKET_ERROR)
		//{
		sock = accept(getSocket(), nullptr, nullptr);
		std::cout << "Server connected client ... " << sock << &std::endl;
		//}
		if (sock == SOCKET_ERROR)
		{
			std::cout << "Socket error: " << &std::endl;
			return 0;
		}
		return 1;
	}
} // ! namespace net