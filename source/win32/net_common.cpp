#include "net.hpp"
#include "win32_common_api.hpp"
#include "exceptions/net_exception.hpp"

namespace net
{
	socket_t make_server(settings::SockSetting& setting, const char* address, int32_t port)
	{
		std::cout << "Server initializing ..." << &std::endl;

		if (port < static_cast<int32_t>(0))
			throw std::logic_error("Netlib: server port should not be negative");

		WSADATA wsaData = { 0 };
		net::api::initializeWSA(wsaData);

		sockaddr_in sockAddress = { 0 };
		sockAddress.sin_family = setting.aiFamily;
		sockAddress.sin_port = ::htons(port);
		sockAddress.sin_addr.s_addr = INADDR_ANY;

		socket_t sockServer = ::socket(setting.aiFamily, setting.aiSocktype, setting.aiProtocol);
		net::throw_exception_on(sockServer == INVALID_SOCKET, "Netlib: server create socket failed");

		int32_t ret = ::bind(sockServer, (sockaddr*)&sockAddress, sizeof(sockaddr));
		net::throw_exception_on(ret == SOCKET_ERROR, "Netlib: server bind failed");

		ret = ::listen(sockServer, setting.countConnection);
		net::throw_exception_on(ret == SOCKET_ERROR, "Netlib: server listening failed");

		std::cout << "Server initializing ... complete" << &std::endl;
		return sockServer;
	}

	//TODO: implement hash of client
	int32_t wait_connection(socket_t& sock_server, socket_t& sock_client, int32_t connections)
	{
		std::cout << "Server wait connection ... " << &std::endl;
		//TODO: implement throw error when client in valid
		sock_client = SOCKET_ERROR;

		sock_client = ::accept(sock_server, nullptr, nullptr);
		//sock_client = ::accept(sock_server, reinterpret_cast<sockaddr*>(&client.m_sockaddrStorage), nullptr);

		if (sock_client == SOCKET_ERROR)
		{
			std::cout << "Server client socket got accept error" << &std::endl;
			return SOCKET_ERROR;
		}

		// How to find address of client by socket?
		//client.m_familyType = m_serverSetting.aiFamily;
		//net::api::interpretFamilyAddress(client.m_sockaddrStorage, client.m_address, m_serverSetting.aiFamily);

		std::cout << "Server got connection socket id ... " << sock_client << &std::endl;
		return 1;
	}

	//TODO: fix int port to char
	socket_t make_connection(settings::SockSetting& setting, const char* address, const char* port)
	{
		std::cout << "Connection initialization ..." << &std::endl;

		WSADATA wsaData = { 0 };
		net::api::initializeWSA(wsaData);

		struct addrinfo hints = { 0 };
		struct addrinfo* sockAddress = nullptr;

		// TODO: create connection for different family
		hints.ai_family = setting.aiFamily;
		hints.ai_socktype = setting.aiSocktype;
		hints.ai_protocol = setting.aiProtocol;

		// TODO: check getaddrinfo is correct
		std::cout << "Connection: get addrinfo ..." << &std::endl;
		int32_t ret = ::getaddrinfo(nullptr, port, &hints, &setting.sockAddress);
		net::throw_exception_on(ret != 0, "Netlib: connection getaddrinfo failed");
		std::cout << "Connection: get addrinfo ... complete" << &std::endl;

		std::cout << "Connection: create socket ..." << &std::endl;
		socket_t sockConnection = ::socket(sockAddress->ai_family, 
										   sockAddress->ai_socktype, 
										   sockAddress->ai_protocol);
		if (sockConnection == INVALID_SOCKET)
		{
			if (setting.sockAddress != nullptr)
				::freeaddrinfo(setting.sockAddress);
			setting.sockAddress = nullptr;
			throw net::exception("Netlib: connection create socket failed");
		}
		std::cout << "Connection: create socket ... complete" << &std::endl;

		// TODO: create non-bloking sockets
		std::cout << "Connection: connect to host ..." << &std::endl;
		ret = ::connect(sockConnection, sockAddress->ai_addr, 
			                            sockAddress->ai_addrlen);
		if (ret == SOCKET_ERROR);
		{
			if (setting.sockAddress != nullptr)
				::freeaddrinfo(setting.sockAddress);
			setting.sockAddress = nullptr;
			throw net::exception("Netlib: connection failed");
		}

		//::freeaddrinfo(sockAddress);
		std::cout << "Connection: connect to host ... complete" << &std::endl;

		return sockConnection;
	}

	int32_t read(socket_t& socket, char* data, size_t len)
	{
		return ::recv(socket, data, len, 0);
	}

	int32_t write(socket_t& socket, const char* data, size_t len)
	{
		return ::send(socket, data, len, 0);
	}

	void shutdown(socket_t& socket, net::enumShutdown param)
	{
		// TODO: check different calls 
		std::cout << "shutdown socket ... " << &std::endl;
		int32_t ret = ::shutdown(socket, static_cast<int>(param));
		net::throw_exception_on(ret == SOCKET_ERROR, "Netlib: client shutdown socket failed");
		std::cout << "shutdown socket ... complete" << &std::endl;
	}

	void free(socket_t& socket)
	{
		std::cout << "Close socket ..." << &std::endl;
		if (socket != INVALID_SOCKET)
		{
			std::cout << "Close socket try ... " << &std::endl;
			int32_t ret = ::closesocket(socket);
			net::throw_exception_on(ret != SOCKET_ERROR, "Netlib: close failed");
		}
		std::cout << "Close socket ... complete" << &std::endl;
	}

	void free(socket_t& socket, ::addrinfo* sock_address)
	{
		std::cout << "Close socket ..." << &std::endl;
		if (sock_address != nullptr)
		{
			::freeaddrinfo(sock_address);
			sock_address = nullptr;
		}

		if (socket != INVALID_SOCKET)
		{
			std::cout << "Close socket try ... " << &std::endl;
			int32_t ret = ::closesocket(socket);
			net::throw_exception_on(ret != SOCKET_ERROR, "Netlib: close failed");
		}
		std::cout << "Close socket ... complete" << &std::endl;
	}

	void release()
	{
		::WSACleanup();
	}
} // namespace net