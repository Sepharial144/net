#include "BasicSocket.hpp"

namespace net
{
	BasicSocket::BasicSocket() 
	{
	}

	BasicSocket::BasicSocket(const size_t bits)
		:m_bitsCount{ bits }
	{
		int32_t res = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		if (res != NO_ERROR)
		{
			printf("base socket: Error at WSAStartup().\n");
		}
		else
			printf("base socket: WSAStartup() is OK.\n");
	}

	BasicSocket::~BasicSocket()
	{
		close();
	}

	void BasicSocket::close()
	{
		if (m_socket != INVALID_SOCKET)
			closesocket(m_socket);
		WSACleanup();
	}

	SOCKET& BasicSocket::socketReference()
	{
		return m_socket;
	}

} // ! namespace net