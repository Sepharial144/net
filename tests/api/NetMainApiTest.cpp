#include <gtest/gtest.h>

#include "TCPTest.hpp"
#include "net.hpp"
#include <thread>
#include <chrono>
#include <string>
#include <array>


TEST_F(TCPTest, TCPCreateServer)
{
	EXPECT_NO_THROW({
		net::socket_t tcp_server = createTCPServer();
		net::free(tcp_server);
	});
}

TEST_F(TCPTest, TCPCommunication)
{
	EXPECT_NO_THROW(([this]{
		const std::string message = "Test message";
		std::array<char, 1024ul> serverRequest = { 0 };
		std::array<char, 1024ul> clientRequest = { 0 };

		constexpr int32_t messageLimit = 50;
		int32_t serverCountMessage = 0;
		int32_t clientCountMessage = 0;

		std::thread serverThread = std::thread([&]{
            net::socket_t tcp_server = createTCPServer();
			net::socket_t tcp_client;

			if (net::wait_connection(tcp_server, tcp_client, 10))
        	{
				net::free(tcp_server);

				int32_t ret = net::write(tcp_client, message.data(), message.size());
				serverCountMessage++;

				while(true)
				{
					if (int32_t ret = net::read(tcp_client, serverRequest.data(), serverRequest.size()))
					{
						ret = net::write(tcp_client, serverRequest.data(), ret);
						if(ret)
							serverCountMessage++;
						if(!ret)
							throw net::exception("Server return status error");
						if (serverCountMessage == messageLimit)
							break;
						}
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				net::free(tcp_client);
			}
        });

		std::thread connectionThread = std::thread([&]{
			net::socket_t tcp_connection = createTCPConnection();

			while (true)
			{
				if (int32_t ret = net::read(tcp_connection, clientRequest.data(), clientRequest.size()))
				{
					clientCountMessage++;
					if (clientCountMessage == messageLimit)
						break;

					ret = net::write(tcp_connection, clientRequest.data(), ret);
					if (!ret)
						throw net::exception("Client return status error");
				}
			}
			net::shutdown(tcp_connection, net::enumShutdown::both);
			net::free(tcp_connection);
		});
		
		connectionThread.join();
		serverThread.join();

		EXPECT_EQ(serverCountMessage, messageLimit);
		EXPECT_EQ(clientCountMessage, messageLimit);
	}
	()));
}

TEST(TestSecond, TestSecond) {
	EXPECT_NO_THROW({
		int32_t a = 0;
	});
}