#include "net.hpp"
#include "exceptions/net_exception.hpp"
#include "net_api.hpp"
#include "logger/logger.hpp"

#include <cstring>
#include <thread>
#include <chrono>

namespace net { 

    socket_t make_server(net::settings::server_t& setting, const char* address, int32_t port, net::socket::type sock_param = net::socket::type::blocking)
	{
		logger::info("Server initializing ...");
		if (port < static_cast<int32_t>(0))
			throw std::logic_error("Netlib: server port should not be negative");

        socket_t sockServer = ::socket(setting.aiFamily, setting.aiSocktype, setting.aiProtocol);
        net::throw_exception_on(sockServer < 0, "Netlib: server create socket failed");

		constexpr int32_t on = 1;
		int32_t ret = ::setsockopt(sockServer, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
		net::throw_exception_on(ret < 0, "Netlib: asynchronous server set socket option failed");

		if (sock_param == net::socket::type::nonblocking)
		{
			ret = ::ioctl(sockServer, FIONBIO, &sock_param);
			net::throw_exception_on(ret < 0, "Netlib: asynchronous server ioctl call failed");
		}

		struct sockaddr_in addr = { 0 };
        addr.sin_family = setting.aiFamily;
        addr.sin_port = ::htons(port);
        // TODO: create for certain address macro
        addr.sin_addr.s_addr = ::htonl(INADDR_ANY);

        ret = ::bind(sockServer, (struct sockaddr*)&addr, sizeof(addr));
        net::throw_exception_on(ret < 0, "Netlib: server bind failed");

        ret = ::listen(sockServer, 1);
        net::throw_exception_on(ret < 0, "Netlib: server listening failed");

		logger::info("Server initializing ... complete");

        return sockServer;
	}

	int32_t wait_connection(net::socket_t& sock_server, net::socket_t& sock_client, int32_t connections)
	{
		logger::info("Server wait connection ... ");
        net::throw_exception_on(sock_client > 0, "Netlib: provided client is not closed");

		do
		{
			sock_client = ::accept(sock_server, nullptr, nullptr);
		} while(sock_client < 0 && (errno == EAGAIN || errno == EWOULDBLOCK));
		if (sock_client < 0)
		{
			logger::info("Asynchronous server client socket got accept error");
			return 0;
		}

		logger::info("Server got connection socket id ... ", sock_client);

		return 1;
	}

	[[deprecated]]
	socket_t make_async_server(net::settings::server_t& setting, const char* address, int32_t port)
	{
		logger::info("Asynchronous server initializing ...");

		if (port < static_cast<int32_t>(0))
			throw std::logic_error("Netlib: server port should not be negative");

        socket_t sockServer = ::socket(setting.aiFamily, setting.aiSocktype, setting.aiProtocol);
        net::throw_exception_on(sockServer < 0, "Netlib: asynchronous server create socket failed");

		constexpr int32_t on = 1;
		int32_t ret = ::setsockopt(sockServer, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
		net::throw_exception_on(ret < 0, "Netlib: asynchronous server set socket option failed");
		
		int32_t nonBlockParam = 1;
		ret = ::ioctl(sockServer, FIONBIO, &nonBlockParam);
		net::throw_exception_on(ret < 0, "Netlib: asynchronous server ioctl call failed");

		struct sockaddr_in addr = { 0 };
        addr.sin_family = setting.aiFamily;
        addr.sin_port = ::htons(port);
        // TODO: create for certain address macro
        addr.sin_addr.s_addr = ::htonl(INADDR_ANY);

        ret = ::bind(sockServer, (struct sockaddr*)&addr, sizeof(addr));
        net::throw_exception_on(ret < 0, "Netlib: server bind failed");

        ret = ::listen(sockServer, 1);
        net::throw_exception_on(ret < 0, "Netlib: server listening failed");

		logger::info("Asynchronous server initializing ... complete");

        return sockServer;
	}

	int32_t wait_async_connection(net::socket_t& sock_server, net::socket_t& sock_client, int32_t connections)
	{
		logger::info("Asynchronous server wait connection ... ");
        net::throw_exception_on(sock_client > 0, "Netlib: provided client is not closed");
		
		do
		{
			sock_client = ::accept(sock_server, nullptr, nullptr);
		} while(sock_client < 0 && (errno == EAGAIN || errno == EWOULDBLOCK));
		if (sock_client < 0)
		{
			logger::info("Asynchronous server client socket got accept error");
			return 0;
		}

		logger::info("Asynchronous server got connection socket id ... ", sock_client);
		return 1;
	}

    //TODO: fix int port to char
	socket_t make_connection(net::settings::connection_t& setting, 
							 const char* address, 
							 const char* port, 
							 net::socket::type sock_param = net::socket::type::blocking)
	{
		std::cout << "Connection initialization ..." << &std::endl;

        socket_t sockConnection = ::socket(setting.aiFamily, setting.aiSocktype, setting.aiProtocol);

		constexpr int32_t on = 1;
		int32_t ret = ::setsockopt(sockConnection, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
		net::throw_exception_on(ret < 0, "Netlib: connection set socket option failed");

		struct hostent *hostInfo = ::gethostbyname(address);
		if (hostInfo == nullptr)
			throw net::exception("Netlib: error while resolve address name");

        struct sockaddr_in addr = { 0 };
        addr.sin_family = setting.aiFamily;
		addr.sin_port = ::htons(net::api::translatePort<int32_t>(port));
        addr.sin_addr.s_addr = ::inet_addr(::inet_ntoa(*(struct in_addr*)hostInfo->h_addr_list[0]));

		if (sock_param == net::socket::type::nonblocking)
		{
			ret = ::ioctl(sockConnection, FIONBIO, &sock_param);
			net::throw_exception_on(ret < 0, "Netlib: connection ioctl call failed");
		
			do{
				std::this_thread::sleep_for(std::chrono::microseconds(37));
				ret = ::connect(sockConnection, (struct sockaddr*)&addr, sizeof(addr));
				logger::info("Connection async status: ", ret, " errno: ", errno);
			} while(ret != 0);	
			logger::info("Connection async status ready: ", ret, " errno: ", errno);
		} else 
		{
			ret = ::connect(sockConnection, (struct sockaddr*)&addr, sizeof(addr));
			net::throw_exception_on(ret < 0, "Netlib: connection failed");
		}

		logger::info("Connection initialization ... complete");
        return sockConnection;
	}

	namespace socket
	{
		bool not_ready() 
		{ 
			logger::info("Socket not ready ...");
			return NET_SOCKET_EAGAIN_EXPR  || NET_SOCKET_WBLOCK_EXPR; 
		} 
	} // namespace socket

    int32_t read(net::socket_t& socket, char* data, size_t len)
	{
		return ::recv(socket, data, len, 0);
	}

	int32_t write(net::socket_t& socket, const char* data, size_t len)
	{
		return ::send(socket, data, len, 0);
	}

	void shutdown(net::socket_t& socket, net::enumShutdown param)
	{
		// TODO: check different calls 
		logger::info("Shutdown socket ... ");
		int32_t ret = ::shutdown(socket, static_cast<int>(param));
		net::throw_exception_on(ret < 0, "Netlib: client shutdown socket failed");
		logger::info("Shutdown socket ... complete");
	}

	void free(net::socket_t& socket)
	{
		logger::info("Close socket ...");
		if (socket != 0)
		{
			logger::info("Close socket try ... ");
			int32_t ret = ::close(socket);
			net::throw_exception_on(ret < 0, "Netlib: close failed");
		}
		logger::info("Close socket ... complete");
	}

	void set_pollfd(net::pollfd_s pollfd_array[], uint64_t array_len, net::pollc::param param)
	{
		for(size_t i = 0ul; 0 < array_len; ++i)
			pollfd_array[i].events = param;
	}

	int32_t poll(net::pollfd_s* pollfd_array, uint64_t array_len, int64_t timeout)
	{
		return ::poll(pollfd_array, array_len, timeout);
	}

	void interpret_address(socket_t& sockfd, ip_address_s& ip_addr)
	{
		struct sockaddr_in sockAddress = {0};
		size_t szAddress = sizeof(sockAddress);
		int32_t ret =  ::getpeername(sockfd, (struct sockaddr*)(&sockAddress), (socklen_t*)(&szAddress));
		net::throw_exception_on(ret == -1, "Netlib: interpret address failed");

		ip_addr.address = ::inet_ntoa(sockAddress.sin_addr);
		ip_addr.addr_size = std::strlen(ip_addr.address);
		ip_addr.type = (ip_addr.addr_size == INET6_ADDRSTRLEN)? net::settings::aifamily::inetv6 : net::settings::aifamily::inetv4;
		ip_addr.port = ::htons(sockAddress.sin_port);
	}

	bool is_connected(socket_t& sockfd)
	{
		struct sockaddr_in sockAddress = {0};
		size_t szAddress = sizeof(sockAddress);
		int32_t ret = ::getpeername(sockfd, (struct sockaddr*)(&sockAddress), (socklen_t*)(&szAddress));
		if (ret != -1)
			return true;
		return false;
	}

    server::server(const net::settings::server_t& settings, const int32_t port)
		: m_serverSetting{ settings }
		, m_socket{ 0 }
		, m_address{ nullptr }
		, m_defaultPort{ port }
	{
    }

    server::server(const net::settings::server_t& settings, const char* addr, const char* port)
		: m_serverSetting{ settings }
		, m_socket{ 0 }
		, m_address{ addr }
		, m_defaultPort{ port }
	{
    }
} // namespace net