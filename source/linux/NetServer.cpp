#include "net_server.hpp"
#include <iostream>

namespace net
{
	server::server()
		:m_socket{ -1 },
		m_clientSocket{ -1 },
		m_address{ nullptr },
		m_defaultPort{ nullptr }
	{
	}

	server::server(const char* addr, const char* port)
		:m_address{ addr },
		m_defaultPort{ port },
		m_socket{ -1 },
		m_serverSettings{ 0 }
	{
		m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
		if (m_socket < 0)
		{
			std::cout << "Linux Server: Error at socket(): " << m_socket << std::endl;
		}
		else
			std::cout << "Linux Server: socket() is OK.\n" << std::endl;

		m_serverSettings.sin_family = AF_INET;
		m_serverSettings.sin_addr.s_addr = htonl(INADDR_ANY);
		// TODO: need to traslate number of port
		m_serverSettings.sin_port = htons(3000);
	}

	void server::listening()
	{
		if (::bind(m_socket, (struct sockaddr*)&m_serverSettings, sizeof(m_serverSettings)) < 0)
			std::cout << "Linux error on binding" << std::endl;
		else
			std::cout << "Linux server bind" << std::endl;

		listen(m_socket, 5);
	}

	server::~server()
	{
		if (m_socket > 0)
			close(m_socket);
	}

	int32_t server::waitConnection(net::client& client)
	{
		listening();
		client.m_socket = ::accept(m_socket, nullptr, nullptr);
		if (client.m_socket < 0)
			std::cout << "Linux server error on accept" << std::endl;
		return 1;
		/*
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
		*/
	}
} // ! namespace net