#include "net.hpp"
#include "win32_common_api.hpp"
#include "exceptions/SocketException.hpp"

namespace net
{
	server::server(const net::addrinfo::SockSetting& settings, const int32_t port)
		: m_wsaData{ 0 }
		, m_serverSetting{ settings }
		, m_socket{ INVALID_SOCKET }
		, m_address{ nullptr }
		, m_defaultPort{ port }
	{
		if (port < static_cast<int32_t>(0))
			throw std::logic_error("Netlib: server port should not be negative");

		// TODO: implement SOMAXCON_HINT(N)
		if (m_serverSetting.countConnection == 0u)
			m_serverSetting.countConnection = SOMAXCONN;

		std::cout << "Server initializing ..." << &std::endl;
		net::api::initializeWSA(m_wsaData);

		sockaddr_in address = { 0 };
		address.sin_family = m_serverSetting.aiFamily;
		address.sin_port = ::htons(port);
		address.sin_addr.s_addr = INADDR_ANY;

		initListeningSocket(nullptr, 
							m_serverSetting.aiFamily, 
							m_serverSetting.aiSocktype, 
							m_serverSetting.aiProtocol, 
							(sockaddr*)&address, 
							sizeof(address));

		std::cout << "Server initializing ... complete" << &std::endl;
	}

	server::server(const net::addrinfo::SockSetting& settings, const char* addr, const char* port)
		: m_wsaData{ 0 }
		, m_serverSetting{ settings }
		, m_socket{ INVALID_SOCKET }
		, m_address{ addr }
		, m_defaultPort{ port }
	{
		std::cout << "Server initializing ..." << &std::endl;

		net::api::initializeWSA(m_wsaData);

		struct addrinfo hints = { 0 };
		struct addrinfo* pAddrInfo = nullptr;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = m_serverSetting.aiFamily;
		hints.ai_socktype = m_serverSetting.aiSocktype;
		hints.ai_protocol = m_serverSetting.aiProtocol;
		hints.ai_flags = m_serverSetting.aiFlags; // investigate this value


		// TODO: implement certain address
		std::cout << "Server get addrinfo ... " << &std::endl;
		int32_t ret = ::getaddrinfo(nullptr, port, &hints, &pAddrInfo);
		net::throw_exception_on(ret != 0, "Netlib: server get addrinfo failed");
		std::cout << "Server get addrinfo ... complete" << &std::endl;

		initListeningSocket(&pAddrInfo,
			pAddrInfo->ai_family,
			pAddrInfo->ai_socktype,
			pAddrInfo->ai_protocol,
			pAddrInfo->ai_addr,
			pAddrInfo->ai_addrlen);

		::freeaddrinfo(pAddrInfo);
		std::cout << "Server initializing ... complete" << &std::endl;
	}

	void server::initListeningSocket(PADDRINFOA* pAddrInfo, 
									 int32_t family, 
									 int32_t socket_type, 
									 int32_t protocol, 
									 sockaddr* ai_address, 
									 int32_t ai_addrlen)
	{
		std::cout << "Server listening ... " << &std::endl;
		m_socket = ::socket(family, socket_type, protocol);
		if (m_socket == INVALID_SOCKET) {
			if (pAddrInfo != nullptr)
			{
				::freeaddrinfo(*pAddrInfo);
				::WSACleanup();
				pAddrInfo = nullptr;
			}
			throw net::exception("Netlib: server create socket failed");
		}

		int32_t ret = ::bind(m_socket, ai_address, ai_addrlen);
		net::throw_exception_on(ret == SOCKET_ERROR, "Netlib: server bind failed");

		ret = ::listen(m_socket, m_serverSetting.countConnection);
		net::throw_exception_on(ret == SOCKET_ERROR, "Netlib: server listening failed");

		std::cout << "Server listening ... complete" << &std::endl;
	}

	server::~server()
	{
		close();
	}

	void server::close()
	{
		// TODO: WSACleanup only with a good way, check it 
		std::cout << "Server close socket ..." << &std::endl;
		if (m_socket != INVALID_SOCKET)
		{
			std::cout << "Server socket is not invalid, close ... " << &std::endl;
			int32_t ret = ::closesocket(m_socket);
			net::throw_exception_on(ret != SOCKET_ERROR, "Netlib: server socket close failed");
			::WSACleanup();
			m_socket = INVALID_SOCKET;
		}
		std::cout << "Server close socket ... complete" << &std::endl;
	}


	int32_t server::waitConnection(net::client& client)
	{
		std::cout << "Server wait connection ... " << &std::endl;
		client.m_socket = SOCKET_ERROR;

		client.m_socket = ::accept(m_socket, reinterpret_cast<sockaddr*>(&client.m_sockaddrStorage), nullptr);

		if (client.m_socket == SOCKET_ERROR)
		{
			std::cout << "Server client socket error after accepting!" << &std::endl;
			return SOCKET_ERROR;
		}

		client.m_familyType = m_serverSetting.aiFamily;
		net::api::interpretFamilyAddress(client.m_sockaddrStorage, client.m_address, m_serverSetting.aiFamily);

		std::cout << "Server sockaddr_in ip: "   << client.m_address.address << "\n"
				  << "Server sockaddr_in port: " << client.m_address.port << &std::endl;

		std::cout << "Server connected client socket id ... " << client.m_socket << &std::endl;
		return 1;
	}
} // ! namespace net