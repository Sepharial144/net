#include "net_connection.hpp"

namespace net
{
	namespace socket
	{
		connection::connection()
			:m_socket{ -1 },
			m_address{ nullptr },
			m_defaultPort{ nullptr },
			m_connectionSettings{ 0 }
		{
			m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
			if (m_socket < 0)
			{
				std::cout << "Linux client: socket() - Error at socket(): " << m_socket << std::endl;
			}
			else
				std::cout << "Client: socket() is OK.\n" << std::endl;
		}

		connection::~connection()
		{
			if (m_socket > 0)
				::close(m_socket);
		}

		int32_t connection::connect(const char* addr, const char* port)
		{
			m_address = addr;
			m_defaultPort = port;
			m_connectionSettings.sin_family = AF_INET;
			// TODO: change INADDR_LOOPBACK to certain address
			m_connectionSettings.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
			// TODO: change port number
			m_connectionSettings.sin_port = htons(3000);

			if (::connect(m_socket, (struct sockaddr*)&m_connectionSettings, sizeof(m_connectionSettings)) < 0)
			{
				std::cout << "Linux client: connect() - Failed to connect.\n" << std::endl;
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
	} // ! namespace socket
} // ! namespace net