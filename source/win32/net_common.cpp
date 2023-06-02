#include "net.hpp"
#include "win32_common_api.hpp"
#include "exceptions/net_exception.hpp"

namespace net
{
	socket_t make_server(net::settings::server_t& setting, const char* address, int32_t port, net::socket::type sock_param)
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

		constexpr int32_t on = 1;
		int32_t ret = ::setsockopt(sockServer, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
		net::throw_exception_on(ret != SOCKET_ERROR, "Netlib: Netlib: asynchronous server set socket option failed");

		if (sock_param == net::socket::type::nonblocking)
		{
			ret = ::ioctlsocket(sockServer, FIONBIO, (u_long*)&sock_param);
			net::throw_exception_on(ret == SOCKET_ERROR, "Netlib: asynchronous server ioctl call failed");
		}

		ret = ::bind(sockServer, (sockaddr*)&sockAddress, sizeof(sockaddr));
		net::throw_exception_on(ret == SOCKET_ERROR, "Netlib: server bind failed");

		ret = ::listen(sockServer, setting.countConnection);
		net::throw_exception_on(ret == SOCKET_ERROR, "Netlib: server listening failed");

		std::cout << "Server initializing ... complete" << &std::endl;
		return sockServer;
	}

	socket_t make_server(net::settings::server_t& setting, const char* address, const char* port, net::socket::type sock_param)
	{
		std::cout << "Server initializing ..." << &std::endl;

		WSADATA wsaData = { 0 };
		net::api::initializeWSA(wsaData);

		struct addrinfo hints = { 0 };
		struct addrinfo* pAddrInfo = nullptr;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = setting.aiFamily;
		hints.ai_socktype = setting.aiSocktype;
		hints.ai_protocol = setting.aiProtocol;
		hints.ai_flags = setting.aiFlags; // investigate this value


		// TODO: implement certain address
		std::cout << "Server get addrinfo ... " << &std::endl;
		int32_t ret = ::getaddrinfo(nullptr, port, &hints, &pAddrInfo);
		net::throw_exception_on(ret != 0, "Netlib: server get addrinfo failed");
		std::cout << "Server get addrinfo ... complete" << &std::endl;

		socket_t sockServer = ::socket(pAddrInfo->ai_family, pAddrInfo->ai_socktype, pAddrInfo->ai_protocol);
		if (sockServer == INVALID_SOCKET)
		{
			::freeaddrinfo(pAddrInfo);
			::WSACleanup();
			throw net::exception("Netlib: server create socket failed");
		}

		constexpr int32_t on = 1;
		ret = ::setsockopt(sockServer, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
		net::throw_exception_on(ret != SOCKET_ERROR, "Netlib: Netlib: asynchronous server set socket option failed");

		if (sock_param == net::socket::type::nonblocking)
		{
			ret = ::ioctlsocket(sockServer, FIONBIO, (u_long*)&sock_param);
			net::throw_exception_on(ret == SOCKET_ERROR, "Netlib: asynchronous server ioctl call failed");
		}

		ret = ::bind(sockServer, pAddrInfo->ai_addr, pAddrInfo->ai_addrlen);
		net::throw_exception_on(ret == SOCKET_ERROR, "Netlib: server bind failed");

		ret = ::listen(sockServer, SOMAXCONN);
		net::throw_exception_on(ret == SOCKET_ERROR, "Netlib: server listening failed");

		::freeaddrinfo(pAddrInfo);
		std::cout << "Server initializing ... complete" << &std::endl;

		return sockServer;
	}

	//TODO: implement hash of client
	int32_t wait_connection(net::socket_t& sock_server, net::socket_t& sock_client, int32_t connections)
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
	socket_t make_connection(net::settings::connection_t& setting, const char* address, const char* port, net::socket::type sock_param)
	{
		std::cout << "Connection initialization ..." << &std::endl;

		//WSADATA wsaData = { 0 };
		net::api::initializeWSA(setting.wsaData);

		struct addrinfo hints = { 0 };
		struct addrinfo* sockAddress = nullptr;

		// TODO: create connection for different family
		hints.ai_family = setting.aiFamily;
		hints.ai_socktype = setting.aiSocktype;
		hints.ai_protocol = setting.aiProtocol;

		std::cout << "Connection: get addrinfo ..." << &std::endl;
		int32_t ret = ::getaddrinfo(nullptr, port, &hints, &sockAddress);
		net::throw_exception_on(ret != 0, "Netlib: connection getaddrinfo failed");
		net::throw_exception_on(sockAddress == nullptr, "Netlib: connection sockaddress is null");
		std::cout << "Connection: get addrinfo ... complete" << &std::endl;

		std::cout << "Connection: create socket ..." << &std::endl;
		socket_t sockConnection = ::socket(sockAddress->ai_family, 
										   sockAddress->ai_socktype, 
										   sockAddress->ai_protocol);
		if (sockConnection == INVALID_SOCKET)
		{
			api::releaseAddrinfo(sockAddress);
			throw net::exception("Netlib: connection create socket failed");
		}

		constexpr int32_t on = 1;
		ret = ::setsockopt(sockConnection, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
		net::throw_exception_on(ret < 0, "Netlib: asynchronous connection set socket option failed");

		if (sock_param == net::socket::type::nonblocking)
		{
			ret = ::ioctlsocket(sockConnection, FIONBIO, (u_long*)&sock_param);
			net::throw_exception_on(ret == SOCKET_ERROR, "Netlib: asynchronous connection ioctl call failed");
		}

		std::cout << "Connection: create socket ... complete" << &std::endl;

		// TODO: create non-bloking sockets
		std::cout << "Connection: connect to host ..." << &std::endl;
		ret = ::connect(sockConnection, sockAddress->ai_addr, 
			                            sockAddress->ai_addrlen);
		if (ret == SOCKET_ERROR);
		{
			api::releaseAddrinfo(sockAddress);
			throw net::exception("Netlib: connection failed");
		}

		api::releaseAddrinfo(sockAddress);
		std::cout << "Connection: connect to host ... complete" << &std::endl;

		return sockConnection;
	}


	int32_t read(net::socket_t& socket, char* data, size_t len)
	{
		return ::recv(socket, data, len, 0);
	}


	int32_t write(net::socket_t& socket, const char* data, size_t len)
	{
		return ::send(socket, data, len, 0);
	}


	socket_t make_async_connection(settings::connection_t& setting, const char* address, const char* port)
	{
		std::cout << "Connection asynchronous initialization ..." << &std::endl;

		net::api::initializeWSA(setting.wsaData);

		struct addrinfo hints = { 0 };
		struct addrinfo* sockAddress = nullptr;

		hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

		int32_t ret = ::getaddrinfo(nullptr, port, &hints, &sockAddress);
		net::throw_exception_on(ret != 0, "Netlib: async connection getaddrinfo failed");
		net::throw_exception_on(sockAddress == nullptr, "Netlib: async connection sockaddress is null");

		socket_t sockConnection = ::socket(sockAddress->ai_family, 
										   sockAddress->ai_socktype, 
										   sockAddress->ai_protocol);
		if (sockConnection == INVALID_SOCKET)
		{
			api::releaseAddrinfo(sockAddress);
			throw net::exception("Netlib: connection create socket failed");
		}

		u_long nonBlockMode = 1;
		ret = ::ioctlsocket(sockConnection, FIONBIO, &nonBlockMode);
		if (ret == SOCKET_ERROR)
		{
			api::releaseAddrinfo(sockAddress);
			throw_exception_on(ret == SOCKET_ERROR, "Netlib: connection ioscl call error");
		}

		ret = ::connect(sockConnection, sockAddress->ai_addr, sockAddress->ai_addrlen);
		if (ret == SOCKET_ERROR);
		{
			api::releaseAddrinfo(sockAddress);
			throw net::exception("Netlib: connection failed");
		}

        std::cout << "Connection asynchronous initialization ... complete" << &std::endl;
        return sockConnection;
	}

/*
	int64_t  poll_read(pollfd_t& poll_array, uint64_t socket_fd, int64_t timeout)
	{
		poll_array.events = POLLRDNORM;
		return ::WSAPoll(&poll_array, socket_fd, timeout);
	}

	int64_t poll_write(pollfd_t& poll_array, uint64_t socket_fd, int64_t timeout)
	{
		poll_array.events = POLLWRNORM;
		return ::WSAPoll(&poll_array, socket_fd, timeout);
	}
*/

	void shutdown(net::socket_t& socket, net::enumShutdown param)
	{
		// TODO: check different calls 
		std::cout << "shutdown socket ... " << &std::endl;
		int32_t ret = ::shutdown(socket, static_cast<int>(param));
		net::throw_exception_on(ret == SOCKET_ERROR, "Netlib: client shutdown socket failed");
		std::cout << "shutdown socket ... complete" << &std::endl;
	}

	void free(net::socket_t& socket)
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

	void free(net::socket_t& socket, ::addrinfo* sock_address)
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

	int32_t poll(net::pollfd_s* pollfd_array, uint64_t array_len, int64_t timeout)
	{
		return ::WSAPoll(pollfd_array, array_len, timeout);
	}

	void interpret_address(socket_t& sockfd, ip_address_s& ip_addr)
	{
		/*
		struct sockaddr_storage sockAddress = {0};
		size_t szAddress = sizeof(sockAddress);
		if(ip_addr.type == net::settings::aifamily::inetv4)
			::inet::ntop(AF_INET, )  ip_addr.address

		if(ip_addr.type == net::settings::aifamily::inetv6)
		ip_addr.address  (ip_addr.type == net::settings::aifamily::inetv4)

		
		//int32_t ret =  ::getpeername(sockfd, (struct sockaddr*)(&sockAddress), (socklen_t*)(&szAddress));
		net::throw_exception_on(ret == -1, "Netlib: interpret address failed");

		ip_addr.address = ::inet_ntoa(sockAddress.sin_addr);
		ip_addr.addr_size = std::strlen(ip_addr.address);
		ip_addr.type = (ip_addr.addr_size == INET6_ADDRSTRLEN)? net::settings::aifamily::inetv6 : net::settings::aifamily::inetv4;
		ip_addr.port = ::htons(sockAddress.sin_port);
		*/
	}

	bool is_connected(socket_t& sockfd)
	{
		/*
		struct sockaddr_in sockAddress = {0};
		size_t szAddress = sizeof(sockAddress);
		int32_t ret = ::getpeername(sockfd, (struct sockaddr*)(&sockAddress), (socklen_t*)(&szAddress));
		if (ret != -1)
			return true;
		*/
		return false;
		
	}

	void release()
	{
		::WSACleanup();
	}
} // namespace net