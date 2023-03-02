#include "net_connection.hpp"

namespace net
{
	connection::connection(const char* addr, const char* port)
		:m_address{ addr },
		 m_defaultPort{ port }
	{
		int32_t res = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		if (res != NO_ERROR)
		{
			printf("Connection: Error at WSAStartup().\n");
			WSACleanup();
		}
		else
			printf("Connection: WSAStartup() is OK.\n");

		m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_socket == INVALID_SOCKET)

		{
			printf("Client: socket() - Error at socket(): %ld\n", WSAGetLastError());
			WSACleanup();
		}
		else
			printf("Client: socket() is OK.\n");

		m_connectionSettings.sin_family = AF_INET;
		m_connectionSettings.sin_addr.s_addr = inet_addr("127.0.0.1");
		m_connectionSettings.sin_port = htons(3000);
	}

	connection::~connection()
	{
		closesocket(m_socket);
		WSACleanup();
	}

	int32_t connection::connect()
	{
		if (::connect(m_socket, (SOCKADDR*)&m_connectionSettings, sizeof(m_connectionSettings)) == SOCKET_ERROR)
		{
			printf("Client: connect() - Failed to connect.\n");
			WSACleanup();
			return -1;
		}
		return 1;
	}

	int32_t connection::send(const char* data, size_t len)
	{
		return ::send(m_socket, data, static_cast<int>(len), 0);
	}

	int32_t connection::read(char* data, size_t len)
	{
		return ::recv(m_socket, (char*)data, static_cast<int>(len), 0);
	}
} // ! namespace net