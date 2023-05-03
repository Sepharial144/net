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
    TCPTest() {
        m_settings = net::settings::SockSetting{
			net::settings::aifamily::inetv4,
			net::settings::aisocktype::stream,
			net::settings::aiprotocol::tcp,
			net::settings::aiflags::passive,
			10ul
		};
    }
    ~TCPTest() {}

    net::socket_t createTCPServer()
    {
        return net::make_server(m_settings, m_address, m_port);
    }

    net::socket_t createTCPConnection()
    {
        return net::make_connection(m_settings, m_address, m_connectionPort);
    }

protected:
    void SetUp() override
    {
        m_settings = net::settings::SockSetting{
			net::settings::aifamily::inetv4,
			net::settings::aisocktype::stream,
			net::settings::aiprotocol::tcp,
			net::settings::aiflags::passive,
			10ul
		};
    }

private:
    const int32_t messageLimit = 5;
    net::settings::SockSetting m_settings;
    const char* m_address = "127.0.0.1";
	const char* m_connectionPort = "3000";
	const int32_t m_port = 3000;
};



#endif // !_NET_TCP_TEST_HPP_