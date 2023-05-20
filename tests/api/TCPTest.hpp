#ifndef _NET_TCP_TEST_HPP_
#define _NET_TCP_TEST_HPP_

#include <gtest/gtest.h>
#include "net.hpp"
#include "exceptions/net_exception.hpp"

#include <thread>
#include <chrono>

class TCPTest: public ::testing::Test
{
public:
    TCPTest() {}
    ~TCPTest() {}

    net::socket_t createTCPServer()
    {
        return net::make_server(m_server, m_address, m_port);
    }

    net::socket_t createTCPConnection()
    {
        return net::make_connection(m_connection, m_address, m_connectionPort);
    }

protected:
    void SetUp() override
    {
        m_server = net::settings::server_t{
			net::settings::aifamily::inetv4,
			net::settings::aisocktype::stream,
			net::settings::aiprotocol::tcp,
			net::settings::aiflags::passive,
			10ul
		};
        m_connection = net::settings::connection_t{
			net::settings::aifamily::inetv4,
			net::settings::aisocktype::stream,
			net::settings::aiprotocol::tcp,
			net::settings::aiflags::passive
		};
    }

private:
    const int32_t messageLimit = 5;
    net::settings::server_t m_server;
    net::settings::connection_t m_connection;
    const char* m_address = "localhost";
	const char* m_connectionPort = "3000";
	const int32_t m_port = 3000;
};



#endif // !_NET_TCP_TEST_HPP_