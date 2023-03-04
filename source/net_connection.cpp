#include "net_connection.hpp"

namespace net
{
	connection::connection(const char* addr, int32_t port)
		:BasicSocket{ AF_INET, SOCK_STREAM, IPPROTO_TCP },
		 m_address{ addr },
		 m_defaultPort{ port }
	{
		m_connectionSettings.sin_family = AF_INET;
		m_connectionSettings.sin_addr.s_addr = inet_addr("127.0.0.1");
		m_connectionSettings.sin_port = htons(m_defaultPort);
	}

	connection::~connection()
	{
		close();
	}

	int32_t connection::connect()
	{
		SOCKET sock = getSocket();
		if (::connect(sock, (SOCKADDR*)&m_connectionSettings, sizeof(m_connectionSettings)) == SOCKET_ERROR)
		{
			printf("Client::connect() - Failed to connect.\n");
			WSACleanup();
			return -1;
		}
		printf("Client::connect() - success.\n");
		return 1;
	}
} // ! namespace net