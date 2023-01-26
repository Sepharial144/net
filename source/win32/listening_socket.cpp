#include "listening_socket.hpp"

/*
namespace net {
	namespace socket {
		ListeningSocket::ListeningSocket(addrinfo::SockSetting& settings, uint32_t buffer_length, const char* default_port) 
		:m_Settings{ settings },
		 iSendResult{ 0u },
		 m_BufferLength{ buffer_length },
		 m_requestBuffer{nullptr},
		 m_initResult{ 0u },
		 m_defaultPort{ default_port },
		 m_socket{ INVALID_SOCKET },
		 m_socketType{ socketType::SERVER },
		 m_status{ status::Initializing }
		{
			//assert(buffer_length > 0 && buffer_length <= TCPIP_PACKET_MAX_SIZE);

			m_initResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);

			if (m_initResult != 0) {
				error("WSAStartup failed", "WSAStartup  failed with error", WSAGetLastError());
				WSACleanup();
				m_status = status::Error;
			}
			else {
				std::cout << "WSAStartup successfully.\n";
				m_status = status::Created;
			}

			m_requestBuffer = new char[m_BufferLength];
			m_ptrInfo = nullptr;
			init();
		}

		ListeningSocket::~ListeningSocket()
		{
			close();
			if (m_requestBuffer != nullptr)
				delete[] m_requestBuffer;
		}

		status ListeningSocket::init()
		{

			ZeroMemory(&m_Hints, sizeof(m_Hints));
			m_Hints.ai_family = m_Settings.aiFamily;
			m_Hints.ai_socktype = m_Settings.aiSocktype;
			m_Hints.ai_protocol = m_Settings.aiProtocol;
			m_Hints.ai_flags = m_Settings.aiFlags;

			// Resolve the server address and port
			m_initResult = ::getaddrinfo(NULL, (PCSTR)m_defaultPort, &m_Hints, &m_ptrInfo);

			if (m_initResult != 0) {

				error("CreateSocket() failed", "failed to create socket", WSAGetLastError());
				WSACleanup();
				m_status = status::Error;
			}
			else {
				printf("GetaddrInfo successfully.\n");
				m_status = status::Created;
			}

			// Listening socket for connecting to this socket
			m_socket = ::socket(m_ptrInfo->ai_family, m_ptrInfo->ai_socktype, m_ptrInfo->ai_protocol);

			if (m_socket == INVALID_SOCKET) {
				error("Socket failed", "failed to listen socket", WSAGetLastError());
				::freeaddrinfo(m_ptrInfo);
				WSACleanup();
				m_status = status::Error;
			}
			else {
				std::cout << "Listenning successfully.\n";
				m_status = status::Listening;
			}

			// Setup the TCP listening socket
			m_initResult = bind(m_socket, m_ptrInfo->ai_addr, (int)m_ptrInfo->ai_addrlen);

			if (m_initResult == SOCKET_ERROR) {
				error("Bind error", "failed to bind socket", WSAGetLastError());
				::freeaddrinfo(m_ptrInfo);
				close();
				WSACleanup();
				m_status = status::Error;
			}
			else {
				std::cout << "Bind address successfully.\n";
				m_status = status::Binded;
			}

			return m_status;
		}

		status ListeningSocket::waitConnection(SOCKET& connection)
		{
			connection = accept(m_socket, nullptr, nullptr);
			if (connection == INVALID_SOCKET) {
				error("Accept", "failed to accept socket", WSAGetLastError());
				//printf("accept failed with error: %d\n", WSAGetLastError());
				::closesocket(m_socket);
				WSACleanup();
				m_status = status::Error;
				return m_status;
			}
			else {
				std::cout << "Accept socket successfully.\n";
				return status::Connected;
			}
		}

		void ListeningSocket::error(const char* title, const char* message, int value) const
		{
			std::cout << "[ERROR]: " << title << " | " << message << std::endl;
			std::cout << "Value: " << value << std::endl;
			std::cout << "------------------\n";
		}

		status ListeningSocket::shutdown(shutdownMode mode)
		{
			m_initResult = ::shutdown(m_socket, (int)mode);

			if (m_initResult == 0)
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
				m_status = status::Error;;
			}
			return m_status;
		}

		status ListeningSocket::status()
		{
			return m_status;
		}

		status ListeningSocket::close()
		{
			m_initResult = ::shutdown(m_socket, (int)shutdownMode::RECIVE_AND_SEND);

			if (m_initResult == SOCKET_ERROR) {
				error("Close()", "shutdown function failed with error:", WSAGetLastError());
				m_status = status::Error;
			}
			else {
				m_status = status::STOPED_RECIVE_SEND;
			}

			m_initResult = closesocket(m_socket);

			if (m_initResult == SOCKET_ERROR) {
				error("Close()", "closesocket function failed with error:", WSAGetLastError());
				m_status = status::Error;
			}
			else {
				m_status = status::Closed;
			}
			WSACleanup();
			return m_status;
		}
	} // !namespace socket
} // !namespace net
*/
