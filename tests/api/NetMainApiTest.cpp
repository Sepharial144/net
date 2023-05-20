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
	EXPECT_NO_THROW(([this]{
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
            net::socket_t tcp_server = net::make_async_server(settings, "127.0.0.1", 3000);

			serverSocketArray[0].fd = tcp_server;
        	serverSocketArray[0].events = net::pollc::in;

			constexpr uint64_t timeout = 3 * 60 * 100; // wait 3 minute
			int32_t ret = ::poll(serverSocketArray, 1, timeout);
			if (ret == 0) 
			{
				std::cout << "Poll call ... timeout: " << ret << " errno: "<< errno << &std::endl; 
				throw net::exception("Server poll call timout error");
			}
			if (ret == net::pollc::poll_error)
			{
				std::cout << "Poll call ... error: " << ret << " errno: "<< errno << &std::endl; 
				throw net::exception("Server poll call error");
			}

			std::cout << "Poll call ... complete" << &std::endl;

			std::cout << "Check server events ..." << std::endl;
			int32_t retEvents = serverSocketArray[0].revents;

			if (retEvents == 0)
			{
				std::cout << "Check server events ... good" << std::endl;
				throw net::exception("Server unexpected error");
			}

			if (retEvents != net::pollc::in)
			{
				// error revent retEvents
				std::cout << "Check server events ... revent error: " << retEvents << std::endl;
				serverIsRun = false;
				throw net::exception("Server revent error");
			}
			std::cout << "Check server events ... complete" << std::endl;

			std::cout << "Accept connection ..." << std::endl;
			net::socket_t tcp_client = ::accept(tcp_server, nullptr, nullptr);
			if (tcp_client < 0)
			{
				std::cout << "Accept connection ... fail" << std::endl;
				if (errno != EWOULDBLOCK)
				{
					// error
					std::cout << "Accept connection ...error" << std::endl;
					serverIsRun = false;
					throw net::exception("Server accept connection error");
				}
				throw net::exception("Server accept unexpected error");
			}

			fdArray[currentSocketFd].fd = tcp_client;
        	fdArray[currentSocketFd].events = net::pollc::in;
			++countClientFd;
			++nfds;
			net::free(tcp_server);
			std::cout << "Accept connection ... complete" << std::endl;
			std::cout << "Server is running ..." << std::endl;

			bool haveMessage = false;

			while(serverIsRun == true)
			{
				std::cout << "Clients poll call ..." << std::endl;

				int32_t ret = net::poll(fdArray, nfds, timeout);
				if (ret == 0) 
				{
					std::cout << "Clients poll call ... failed, timeout " << ret << " errno: "<< errno << std::endl;
					break;
				}
				if (ret == net::pollc::poll_error)
				{
					std::cout << "Clients poll call ... failed, got error " << ret << " errno: "<< errno << &std::endl; 
					break;
				}
				std::cout << "Call clients poll ... complete" << &std::endl;


				for(size_t i = 0ul; i < nfds; ++i)
				{
					net::pollfd_s currentPollFd = fdArray[i];
					std::cout << "Check client events ... socket idx: " << i << &std::endl;

					std::cout << "Returned event: " << currentPollFd.revents << std::endl;

					if (currentPollFd.revents == 0)
						std::cout << "Check client events ... is good: " << currentPollFd.revents << &std::endl;

					if (currentPollFd.revents != net::pollc::in)
					{
						std::cout << "Check client events ... error: " << currentPollFd.revents << &std::endl;
						serverIsRun = false;
						break;
					}
					std::cout << "Check client events ... complete" << &std::endl;

					do 
					{
						if (!haveMessage)
						{
							std::cout << "Read message ..." << std::endl;
							ret = net::read(currentPollFd.fd, serverRequest.data(), serverRequest.size());
							if (ret > 0)
							{
								std::cout << "Read message ... complete, ret status: " << ret << " " << serverRequest.data() << " len: " << ret << std::endl;
								serverCountMessage++;
								haveMessage = true;
								continue;
							}

							if (ret < 0)
							{
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
								std::cout << "Read message ... disconnected: " << ret << "errno: " << errno <<&std::endl;
								net::free(currentPollFd.fd);
								serverIsRun = false;
								break;
							}
						}

						std::cout << "Write message ... " << std::endl;
						ret = net::write(currentPollFd.fd, serverRequest.data(), ret);
						if (ret > 0)
						{
							haveMessage = false;
							std::cout << "Write message ... complete, ret status: " << ret << std::endl;
							continue;
						}

						if (ret < 0)
						{
							std::cout << "Write message ...  error: " << ret << " errno: " << errno <<&std::endl;
							net::free(currentPollFd.fd);
							serverIsRun = false;
							break;
						}
						if (ret == net::status::disconnected)
						{
							std::cout << "Write message ... disconnected: " << ret << "errno: " << errno <<&std::endl;
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
				//std::this_thread::sleep_for(std::chrono::milliseconds(50));
				int32_t ret = net::write(tcp_connection, message.data(), message.size());
				if (!ret)
					throw net::exception("Client write status error");

				if (ret = net::read(tcp_connection, clientRequest.data(), clientRequest.size()) > 0)
				{
					clientCountMessage++;
					//std::memset(clientRequest.data(), 0, clientRequest.size());
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
	()));
}

TEST(TestSecond, TestSecond) {
	EXPECT_NO_THROW({
		int32_t a = 0;
	});
}