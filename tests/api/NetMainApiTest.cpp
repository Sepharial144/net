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

TEST(CommonTest, ConnectionInformation)
{
	std::thread serverThread = std::thread([&]{

		net::settings::server_t settings{
			net::settings::aifamily::inetv4,
			net::settings::aisocktype::stream,
			net::settings::aiprotocol::tcp,
			net::settings::aiflags::passive,
			10ul
		};

		net::socket_t tcp_server = net::make_server(settings, "localhost", 3000, net::socket::type::blocking);
		net::socket_t tcp_client = {0};
		net::ip_address_s ipAddress = {0};

		if (net::wait_connection(tcp_server, tcp_client, 10))
		{
			net::free(tcp_server);
			EXPECT_EQ(net::is_connected(tcp_client), true);
			net::interpret_address(tcp_client, ipAddress);
			EXPECT_EQ(ipAddress.addr_size, 9);
			EXPECT_EQ(std::string{ipAddress.address}, "127.0.0.1");
			EXPECT_NE(ipAddress.port, 0);
			EXPECT_EQ(ipAddress.type, net::settings::aifamily::inetv4);
		}	
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	std::thread clientThread = std::thread([&]{

		auto connectionSettings = net::settings::connection_t{
			net::settings::aifamily::inetv4,
			net::settings::aisocktype::stream,
			net::settings::aiprotocol::tcp,
			net::settings::aiflags::passive
		};

		net::socket_t tcp_connection = net::make_connection(connectionSettings, "localhost", "3000", net::socket::type::blocking);
		net::shutdown(tcp_connection, net::enumShutdown::both);
		net::free(tcp_connection);
	});

	if (clientThread.joinable())
		clientThread.join();

	if (serverThread.joinable())
		serverThread.join();
}



TEST_F(TCPTest, TCPServerDisconnect)
{
	EXPECT_NO_THROW(([this]{
		const std::string message{"diconnect"};
		std::string request;
		request.resize(message.size());

		//std::array<char, 1024ul> serverRequest = { 0 };
		//std::array<char, 1024ul> clientRequest = { 0 };

		std::thread serverThread = std::thread([&]{
			net::settings::server_t settings{
				net::settings::aifamily::inetv4,
				net::settings::aisocktype::stream,
				net::settings::aiprotocol::tcp,
				net::settings::aiflags::passive,
				10ul
			};

            net::socket_t tcp_server = net::make_server(settings, "localhost", 3000, net::socket::type::blocking);
			net::socket_t tcp_client = {0};

			if (net::wait_connection(tcp_server, tcp_client, 10))
			{
				net::free(tcp_server);
				bool diconnectIncoming = false;
				bool serverIsRunning = true;
				size_t timeout = 3000ul; // wait 3 seconds
				std::chrono::steady_clock::time_point timeCount = std::chrono::steady_clock::now();
				std::chrono::steady_clock::time_point timeDiff;
				while(serverIsRunning == true)
				{
					//timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timeCount).count();
					//std::cout << timeDiff << std::endl;
					//if (timeout <= timeDiff)
					//{
					//	net::free(tcp_client);
					//	throw net::exception("Timeout");
					//}

					int32_t ret = net::read(tcp_client, request.data(), request.capacity());

					if (diconnectIncoming == false)
					{
						if (ret < 0 && (NET_SOCKET_EAGAIN_EXPR || NET_SOCKET_WBLOCK_EXPR))
							continue;
						
						net::throw_exception_on(ret == net::status::disconnected, 
											"Unexpected disconnect from client");
						net::throw_exception_on(ret < 0, 
											"Unexpected server error");

						if (diconnectIncoming = ret > 0 && message == request)
							continue;
					}

					net::throw_exception_on(ret < 0, 
											"Error while waiting diconnect");
					net::throw_exception_on(ret > 0, 
											"Unexpected message received");
					if (ret == net::status::disconnected)
						break;
				}
			} else { throw net::exception("Accept connection failed"); }

			net::free(tcp_client);
		});

		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		std::thread clientThread = std::thread([&]{

			auto connectionSettings = net::settings::connection_t{
				net::settings::aifamily::inetv4,
				net::settings::aisocktype::stream,
				net::settings::aiprotocol::tcp,
				net::settings::aiflags::passive
			};

			net::socket_t tcp_connection = net::make_connection(connectionSettings, "localhost", "3000", net::socket::type::blocking);
			int32_t ret = net::write(tcp_connection, message.data(), message.size());
			net::shutdown(tcp_connection, net::enumShutdown::both);
			net::free(tcp_connection);
		});

		if (clientThread.joinable())
			clientThread.join();

		if (serverThread.joinable())
			serverThread.join();
	
	}()));
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

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

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
        net::pollfd_s fdArray[1];

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
            net::socket_t tcp_server = net::make_server(settings, "localhost", 3000, net::socket::type::nonblocking);

			serverSocketArray[0].fd = tcp_server;
        	serverSocketArray[0].events = net::pollc::in;

			constexpr uint64_t timeout = 3 * 60 * 100; // wait
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
			ret = net::wait_connection(tcp_server, tcp_client , 10);
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
								if (net::socket::not_ready())
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
							std::memset(serverRequest.data(), 0, serverRequest.size());
							std::cout << "Write message ... complete, ret status: " << ret << &std::endl;
							continue;
						}

						if (ret < 0)
						{
							if (net::socket::not_ready())
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

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

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
					if (clientCountMessage == messageLimit)
						break;
					std::memset(clientRequest.data(), 0, clientRequest.size());
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
		std::cout << serverRequest.data() << "|"<< clientRequest.data() << std::endl;
		// TODO: need to fix this
		//EXPECT_EQ(std::strcmp(serverRequest.data(), clientRequest.data()), 0);
	}
	()));
}


TEST_F(TCPTest, TCPCommunicationAsynchronousBoth)
{
	EXPECT_NO_THROW(([this]{
		const std::string message{"Asynchronous socket!"};
		std::array<char, 1024ul> serverRequest = { 0 };
		std::array<char, 1024ul> clientRequest = { 0 };

		net::pollfd_s serverSocketArray[1];
        net::pollfd_s fdArray[1];
		net::pollfd_s fdConnectionArray[1];

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
            net::socket_t tcp_server = net::make_server(settings, "localhost", 3000, net::socket::type::nonblocking);

			serverSocketArray[0].fd = tcp_server;
        	serverSocketArray[0].events = net::pollc::in;

			constexpr uint64_t timeout = 3 * 60 * 100; // wait
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
			ret = net::wait_connection(tcp_server, tcp_client , 10);
			net::throw_exception_on(ret == 0, "Accept connection ... failed");

			fdArray[currentSocketFd].fd = tcp_client;
        	fdArray[currentSocketFd].events = net::pollc::in;
			++countClientFd;
			++nfds;
			std::cout << "Close listening socket ..." << &std::endl;
			net::free(tcp_server);
			std::cout << "Close listening socket ... complete" << &std::endl;
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
							std::cout << "Server read message ..." << &std::endl;
							ret = net::read(currentPollFd.fd, serverRequest.data(), serverRequest.size());
							if (ret > 0)
							{
								std::cout << "Server read message ... complete, ret status: " << ret << " " << serverRequest.data() << " len: " << ret << &std::endl;
								haveMessage = true;
								continue;
							}

							if (ret < 0)
							{
								if (net::socket::not_ready())
								{
									std::cout << "Server read message ... not ready" << &std::endl;
									continue;
								}

								std::cout << "Server read message ... error: " << ret << " errno: " << errno <<&std::endl;
								goto finishServer;
							}

							if (ret == net::status::disconnected)
							{
								std::cout << "Server read message ... disconnected: " << ret << " errno: " << errno <<&std::endl;
								goto finishServer;
							}
						}

						std::cout << "Server write message ... " << &std::endl;
						ret = net::write(currentPollFd.fd, serverRequest.data(), ret);
						if (ret > 0)
						{
							serverCountMessage++;
							if (serverCountMessage == messageLimit)
								goto finishServer;
							haveMessage = false;
							std::memset(serverRequest.data(), 0, serverRequest.size());
							std::cout << "Server write message ... complete, ret status: " << ret << &std::endl;
							continue;
						}

						if (ret < 0)
						{
							if (net::socket::not_ready())
							{
								std::cout << "Server write messag ... not ready" << &std::endl;
								continue;
							}

							std::cout << "Server write messag ...  error: " << ret << " errno: " << errno <<&std::endl;
							goto finishServer;
						}
						if (ret == net::status::disconnected)
						{
							std::cout << "Server write messag ... disconnected: " << ret << " errno: " << errno <<&std::endl;
							net::free(currentPollFd.fd);
							goto finishServer;
						}

					} while(true); 
				} // end of loop through pollable descriptors 
			} // end of serving running.

			goto finishServer;
			finishServer:
				net::free(tcp_client);
				serverIsRun = false;
        });

		std::this_thread::sleep_for(std::chrono::seconds(1));

		std::thread connectionThread = std::thread([&]{

            net::settings::connection_t settings{
                net::settings::aifamily::inetv4,
                net::settings::aisocktype::stream,
                net::settings::aiprotocol::tcp,
                net::settings::aiflags::passive
    	    };

			net::socket_t tcp_connection = net::make_connection(settings, "localhost", "3000", net::socket::type::nonblocking);

/*			std::cout << "Connection poll call ..." << &std::endl;
			constexpr size_t timeout = 60 * 1000 * 3;
			int32_t ret = net::poll(fdConnectionArray, 1, timeout);
			net::throw_exception_on_poll(ret, "Connection poll call ...");
			std::cout << "Call connnection poll ... complete" << &std::endl;

			std::cout << "Connection revents ... complete" << &std::endl;
			net::pollfd_s currentPollConnection = fdConnectionArray[0];
			std::cout << "Connection revents ... returned event: " << currentPollConnection .revents << &std::endl;
			net::throw_exception_on_revents(currentPollConnection.revents, net::pollc::in, "Connection revents ...");
			std::cout << "Connection revents ... complete" << &std::endl;
*/

			int32_t ret = 0;
			bool messageIncoming = false;
			while (true)
			{
				if (!messageIncoming)
				{
					std::cout << "Connection write message ... " << &std::endl;
					ret = net::write(tcp_connection, message.data(), message.size());
					if (ret > 0)
					{
						std::cout << "Connection write message ... complete: " << ret << &std::endl;
						messageIncoming = true;
						continue;
					}

					if (ret < 0)
					{
						if (net::socket::not_ready())
						{
							std::cout << "Connection write message ... not ready" << &std::endl;
							continue;
						}

						net::shutdown(tcp_connection, net::enumShutdown::both);
						net::free(tcp_connection);
						throw net::exception("Client write status error");
					}

					if (ret == net::status::disconnected)
					{
						std::cout << "Connection write message ... disconnected: " << ret << " errno: " << errno <<&std::endl;
						net::shutdown(tcp_connection, net::enumShutdown::both);
						net::free(tcp_connection);
						throw net::exception("Server has gone");
					}
				}


				std::cout << "Connection read message ... " << &std::endl;
				ret = net::read(tcp_connection, clientRequest.data(), clientRequest.size());
				if (ret > 0)
				{
					std::cout << "Connection read message ... complete" << &std::endl;
					clientCountMessage++;
					if (clientCountMessage == messageLimit)
						break;
					messageIncoming = false;
					std::memset(clientRequest.data(), 0, clientRequest.size());
					continue;
				}

				if (ret < 0)
				{
					if (net::socket::not_ready())
					{
						std::cout << "Connection read message ... not ready" << &std::endl;
						continue;
					}

					net::shutdown(tcp_connection, net::enumShutdown::both);
					net::free(tcp_connection);
					throw net::exception("Client read status error");
				}

				if (ret == net::status::disconnected)
				{
					std::cout << "Connection read message ... disconnected: " << ret << " errno: " << errno <<&std::endl;
					net::shutdown(tcp_connection, net::enumShutdown::both);
					net::free(tcp_connection);
					throw net::exception("Server has gone");
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