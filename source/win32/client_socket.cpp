#include "client_socket.hpp"

namespace net {
	namespace socket {
		ClientSocket::ClientSocket(addrinfo::SockSetting& settings, 
			                       addrinfo::ConnectionSetting& conn_settings,
							       uint32_t buffer_length, 
								   int32_t default_port)
		:m_Settings{ settings },
		 m_connectionSetting{ conn_settings },
		 m_bufferLength{ buffer_length },
		 m_defaultPort{ conn_settings.port },
		 m_socket{ INVALID_SOCKET },
		 m_clientSettings{0u},
	     m_requestBuffer{nullptr},
		 m_socketType{ socketType::CONNECTION },
		 m_status{ status::Initializing }
		{
			//assert(buffer_length > 0 && buffer_length <= TCPIP_PACKET_MAX_SIZE);

			int32_t res = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
			if (res != NO_ERROR) {
				error("WSAStartup failed", "WSAStartup  failed with error", WSAGetLastError());
				WSACleanup();
				m_status = status::Error;
			}
			else {
				std::cout << "WSAStartup successfully.\n";
				m_status = status::Created;
			}

			m_requestBuffer = new char[m_bufferLength];

			m_socket = ::socket(m_Settings.aiFamily, m_Settings.aiSocktype, m_Settings.aiProtocol);
			if (m_socket == INVALID_SOCKET) {
				error("InitSocketSettings()", "Init socket failed with error:", WSAGetLastError());
				WSACleanup();
			}

			m_clientSettings.sin_family = m_Settings.aiFamily;
			m_clientSettings.sin_addr.s_addr = inet_addr(m_connectionSetting.ip_address);
			m_clientSettings.sin_port = htons(default_port);
		}

		ClientSocket::~ClientSocket()
		{
			close();
			if (m_requestBuffer != nullptr)
				delete[] m_requestBuffer;
		}

		status ClientSocket::connect()
		{
			int32_t res = ::connect(m_socket, (SOCKADDR*)&m_clientSettings, sizeof(m_clientSettings));

			if (res == SOCKET_ERROR) {
				error("Connect()", "Connection failed with error", WSAGetLastError());
				res = ::closesocket(m_socket);
				if (res == SOCKET_ERROR)
					error("Close after connection()", "Connection and close failed with error", WSAGetLastError());
				WSACleanup();
				m_status = status::Error;
			}
			else {
				m_status = status::Connected;
			}
			return m_status;
		}

		status ClientSocket::shutdown(shutdownMode mode)
		{
			int32_t res = ::shutdown(m_socket, static_cast<int>(mode));
			if (res == 0)
			{
				switch (mode)
				{
				case shutdownMode::RECEIVE:
					m_status = status::STOPED_RECEIVE;
					break;
				case shutdownMode::SEND:
					m_status = status::STOPED_SEND;
					break;
				case shutdownMode::RECIVE_AND_SEND:
					m_status = status::STOPED_RECIVE_SEND;
					break;
				default:
					break;
				}
			}
			else {
				error("Shutdown failed", "failed to shutdown socket:", WSAGetLastError());
				m_status = status::Error;
			}
			return m_status;
		}

		status ClientSocket::status()
		{
			return m_status;
		}

		status ClientSocket::close()
		{
			int32_t res = ::shutdown(m_socket, static_cast<int>(shutdownMode::RECIVE_AND_SEND));
			if (res == SOCKET_ERROR) {
				error("Close()", "shutdown function failed with error:", WSAGetLastError());
				m_status = status::Error;
			}
			else {
				m_status = status::STOPED_RECIVE_SEND;
			}

			res = ::closesocket(m_socket);
			if (res == SOCKET_ERROR) {
				error("Close()", "closesocket function failed with error:", WSAGetLastError());
				m_status = status::Error;
			}
			else {
				m_status = status::Closed;
			}
			WSACleanup();
			return m_status;
		}

		void ClientSocket::error(const char* title, const char* message, int32_t value)
		{
			std::cout << "[ERROR]: " << title << " | " << message << std::endl;
			std::cout << "Value: " << value << std::endl;
			std::cout << "------------------\n";
		}

		int32_t ClientSocket::send(const char* data, size_t len)
		{
			return ::send(m_socket, data, static_cast<int>(len), 0);
		}

		int32_t ClientSocket::read(char* data, size_t len)
		{
			return ::recv(m_socket, (char*)data, static_cast<int>(len), 0);
		}
	} // !namespace Socket
} // !namespace Net