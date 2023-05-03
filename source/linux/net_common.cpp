#include "net.hpp"
#include "exceptions/net_exception.hpp"
#include "net_api.hpp"

namespace net { 

    socket_t make_server(net::settings::server_t& setting, const char* address, int32_t port)
	{
		std::cout << "Server initializing ..." << &std::endl;

		if (port < static_cast<int32_t>(0))
			throw std::logic_error("Netlib: server port should not be negative");

        struct sockaddr_in addr = { 0 };
        socket_t sockServer = ::socket(setting.aiFamily, setting.aiSocktype, setting.aiProtocol);
        net::throw_exception_on(sockServer < 0, "Netlib: server create socket failed");

        addr.sin_family = setting.aiFamily;
        addr.sin_port = ::htons(port);
        // TODO: create for certain address macro
        addr.sin_addr.s_addr = ::htonl(INADDR_ANY);

        int32_t ret = ::bind(sockServer, (struct sockaddr*)&addr, sizeof(addr));
        net::throw_exception_on(ret < 0, "Netlib: server bind failed");

        ret = ::listen(sockServer, 1);
        net::throw_exception_on(ret < 0, "Netlib: server listening failed");

        std::cout << "Server initializing ... complete" << &std::endl;

        return sockServer;
	}

	//TODO: implement hash of client
	int32_t wait_connection(net::socket_t& sock_server, net::socket_t& sock_client, int32_t connections)
	{
		std::cout << "Server wait connection ... " << &std::endl;
		//TODO: implement throw error when client in valid
        net::throw_exception_on(sock_client > 0, "Netlib: provided client is not closed");

		sock_client = ::accept(sock_server, nullptr, nullptr);
		//sock_client = ::accept(sock_server, reinterpret_cast<sockaddr*>(&client.m_sockaddrStorage), nullptr);
        net::throw_exception_on(sock_client < 0, "Netlib: error while connect client");
		if (sock_client < 0)
		{
			std::cout << "Server client socket got accept error" << &std::endl;
			return 0;
		}

		// How to find address of client by socket?
		//client.m_familyType = m_serverSetting.aiFamily;
		//net::api::interpretFamilyAddress(client.m_sockaddrStorage, client.m_address, m_serverSetting.aiFamily);

		std::cout << "Server got connection socket id ... " << sock_client << &std::endl;
		return 1;
	}

    	//TODO: fix int port to char
	socket_t make_connection(net::settings::connection_t& setting, const char* address, const char* port)
	{
		std::cout << "Connection initialization ..." << &std::endl;

        socket_t sockConnection = ::socket(setting.aiFamily, setting.aiSocktype, setting.aiProtocol);
        // ::socket(setting.aiFamily, setting.aiSocktype, 0);

        struct sockaddr_in addr = { 0 };
        addr.sin_family = setting.aiFamily;
		addr.sin_port = ::htons(net::api::translatePort<int32_t>(port));
        addr.sin_addr.s_addr = ::htonl(INADDR_LOOPBACK);
        int32_t ret = ::connect(sockConnection, (struct sockaddr*)&addr, sizeof(addr));
        net::throw_exception_on(ret < 0, "Netlib: connection failed");

        std::cout << "Connection initialization ... complete" << &std::endl;
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

	void shutdown(net::socket_t& socket, net::enumShutdown param)
	{
		// TODO: check different calls 
		std::cout << "shutdown socket ... " << &std::endl;
		int32_t ret = ::shutdown(socket, static_cast<int>(param));
		net::throw_exception_on(ret < 0, "Netlib: client shutdown socket failed");
		std::cout << "shutdown socket ... complete" << &std::endl;
	}

	void free(net::socket_t& socket)
	{
		std::cout << "Close socket ..." << &std::endl;
		if (socket != 0)
		{
			std::cout << "Close socket try ... " << &std::endl;
			int32_t ret = ::close(socket);
			net::throw_exception_on(ret < 0, "Netlib: close failed");
		}
		std::cout << "Close socket ... complete" << &std::endl;
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