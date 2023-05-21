#include <gtest/gtest.h>

#include "TCPTest.hpp"
#include "net.hpp"
#include <thread>
#include <chrono>
#include <string>
#include <cstring>
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
			net::socket_t tcp_client = {0};

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
						throw net::exception("Client write status error");
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

TEST_F(TCPTest, TCPCommunicationAsynchronous)
{
	EXPECT_NO_THROW(([this](){
		const std::string message{"Asynchronous socket!"};
		std::array<char, 1024ul> serverRequest = { 0 };
		std::array<char, 1024ul> clientRequest = { 0 };

		net::pollfd_s serverSocketArray[1];
        net::pollfd_s fdArray[10];

		int32_t countClientFd = 0;
        int32_t currentSocketFd = 0;
		int32_t nfds = 0; 
        bool serverIsRun = true;

		constexpr int32_t messageLimit = 50;
		int32_t serverCountMessage = 0;
		int32_t clientCountMessage = 0;

		net::settings::server_t settings{
			net::settings::aifamily::inetv4,
			net::settings::aisocktype::stream,
			net::settings::aiprotocol::tcp,
			net::settings::aiflags::passive,
			10ul
    	};

		std::thread serverThread = std::thread([&]{
            net::socket_t tcp_server = net::make_async_server(settings, "localhost", 3000);

			serverSocketArray[0].fd = tcp_server;
        	serverSocketArray[0].events = net::pollc::in;

			constexpr uint64_t timeout = 3 * 60 * 100; // wait 3 minute
			int32_t ret = net::poll(serverSocketArray, 1, timeout);
			std::cout << "Server poll call ... " << &std::endl;
			net::throw_exception_on_poll(ret, "Server poll call");
			std::cout << "Server poll call ... complete" << &std::endl;

			std::cout << "Server check revents ..." << &std::endl;
			int32_t retEvents = serverSocketArray[0].revents;
			net::throw_exception_on_revents(retEvents, net::pollc::in, "Server check revents ...");
			std::cout << "Server check revents ... complete" << &std::endl;

			std::cout << "Accept connection ..." << &std::endl;

			net::socket_t tcp_client  = {0};
			ret = net::wait_async_connection(tcp_server, tcp_client , 10);
			net::throw_exception_on(ret == 0, "Accept connection ... failed");

			fdArray[currentSocketFd].fd = tcp_client;
        	fdArray[currentSocketFd].events = net::pollc::in;
			++countClientFd;
			++nfds;
			net::free(tcp_server);
			std::cout << "Accept connection ... complete" << &std::endl;
			std::cout << "Server is running ..." << &std::endl;

			bool haveMessage = false;

			while(serverIsRun == true)
			{
				std::cout << "Clients poll call ..." << &std::endl;

				int32_t ret = net::poll(fdArray, nfds, timeout);
				net::throw_exception_on_poll(ret, "Clients poll call ...");
				std::cout << "Call clients poll ... complete" << &std::endl;


				for(size_t i = 0ul; i < nfds; ++i)
				{
					net::pollfd_s currentPollFd = fdArray[i];
					std::cout << "Client revents ... socket idx: " << i << &std::endl;
					std::cout << "Client revents ... returned event: " << currentPollFd.revents << &std::endl;
					net::throw_exception_on_revents(currentPollFd.revents, net::pollc::in, "Client revents ...");
					std::cout << "Client revents ... complete" << &std::endl;

					do 
					{
						if (!haveMessage)
						{
							std::cout << "Read message ..." << &std::endl;
							ret = net::read(currentPollFd.fd, serverRequest.data(), serverRequest.size());
							if (ret > 0)
							{
								std::cout << "Read message ... complete, ret status: " << ret << " " << serverRequest.data() << " len: " << ret << &std::endl;
								serverCountMessage++;
								haveMessage = true;
								continue;
							}

							if (ret < 0)
							{
								if (errno == NET_SOCKET_EAGAIN  || errno == NET_SOCKET_WOULD_BLOCK)
									continue;

								if(errno != EWOULDBLOCK)
								{
									std::cout << "Read message ... error: " << ret << " errno: " << errno <<&std::endl;
									net::free(currentPollFd.fd);
								}
								serverIsRun = false;
								break;
							}

							if (ret == net::status::disconnected)
							{
								std::cout << "Read message ... disconnected: " << ret << " errno: " << errno <<&std::endl;
								net::free(currentPollFd.fd);
								serverIsRun = false;
								break;
							}
						}

						std::cout << "Write message ... " << &std::endl;
						ret = net::write(currentPollFd.fd, serverRequest.data(), ret);
						if (ret > 0)
						{
							haveMessage = false;
							std::cout << "Write message ... complete, ret status: " << ret << &std::endl;
							continue;
						}

						if (ret < 0)
						{
							if (errno == NET_SOCKET_EAGAIN  || errno == NET_SOCKET_WOULD_BLOCK)
								continue;

							std::cout << "Write message ...  error: " << ret << " errno: " << errno <<&std::endl;
							net::free(currentPollFd.fd);
							serverIsRun = false;
							break;
						}
						if (ret == net::status::disconnected)
						{
							std::cout << "Write message ... disconnected: " << ret << " errno: " << errno <<&std::endl;
							net::free(currentPollFd.fd);
							serverIsRun = false;
							break;
						}
					} while(true); 
				} // end of loop through pollable descriptors 
			} // end of serving running.
        });

		std::thread connectionThread = std::thread([&]{
			net::socket_t tcp_connection = createTCPConnection();

			while (true)
			{
				int32_t ret = net::write(tcp_connection, message.data(), message.size());
				if (!ret)
					throw net::exception("Client write status error");

				if (ret = net::read(tcp_connection, clientRequest.data(), clientRequest.size()) > 0)
				{
					clientCountMessage++;
					if (clientCountMessage != messageLimit)
						std::memset(clientRequest.data(), 0, clientRequest.size());
					if (clientCountMessage == messageLimit)
						break;
				} else 
				{
					throw net::exception("Client read status error");
				}
			}
			std::cout << "Close connection ... " << &std::endl;
			net::shutdown(tcp_connection, net::enumShutdown::both);
			net::free(tcp_connection);
			std::cout << "Close connection ... complete" << &std::endl;
		});
		
		connectionThread.join();
		serverThread.join();

		EXPECT_EQ(serverCountMessage, messageLimit);
		EXPECT_EQ(clientCountMessage, messageLimit);
		EXPECT_EQ(std::strcmp(serverRequest.data(), clientRequest.data()), 0);
	}
	));
}

TEST(TestSecond, TestSecond) {
	EXPECT_NO_THROW({
		int32_t a = 0;
	});
}