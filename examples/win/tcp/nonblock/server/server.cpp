#include "net.hpp"

#include <iostream>
#include <array>
#include <thread>
#include <chrono>

using namespace net;

int main()
{
    const char* address = "127.0.0.1";
    int32_t port = 3000;

    net::settings::server_t settings{
        net::settings::aifamily::inetv4,
        net::settings::aisocktype::stream,
        net::settings::aiprotocol::tcp,
        net::settings::aiflags::passive,
        10ul
    };

    try
    {
        socket_t tcp_server = net::make_server(settings, address, port, net::socket::type::nonblocking);
        std::array<char, 1024ul> request = { 0 };

        std::string message{"Asynchronous socket!"};
        net::pollfd_s serverSocketArray[1];
        net::pollfd_s fdArray[10];

        int32_t countClientFd = 0;
        int32_t currentSocketFd = 0;
        int32_t nfds = 0; 
        bool serverIsRun = true;

        std::cout << "Poll call ..." << &std::endl;

        serverSocketArray[0].fd = tcp_server;
        serverSocketArray[0].events = POLLWRDNORM;
        constexpr uint64_t timeout = DEFAULT_WAIT; // wait 3 minute
        int32_t ret = net::poll(serverSocketArray, 1, timeout);
        net::throw_exception_on(ret == SOCKET_ERROR, "Server poll error");
        if (ret == SOCKET_ERROR)
        {
            throw net::exception("Server poll error");
        }
        std::cout << "Poll call ... complete" << &std::endl;

        std::cout << "Check server events ..." << std::endl;
        int32_t retEvents = serverSocketArray[0].revents;
        if (retEvents == 0)
            std::cout << "Check server events ... good" << std::endl;

        if (retEvents != POLLRDNORM)
        {
            throw net::exception("Server revent error")
        }

        std::cout << "Accept connection ..." << std::endl;
        socket_t tcp_client = ::accept(tcp_server, nullptr, nullptr);
        net::throw_exception_on(tcp_client == INVALID_SOCKET, "Server accepting error");

        // add client to structure
        fdArray[currentSocketFd].fd = tcp_client;
        fdArray[currentSocketFd].events = POLLWRNORM;
        ++countClientFd;
        ++nfds;
        std::cout << "Accept connection ... complete" << std::endl;

        std::cout << "Server is running ..." << std::endl;

        bool haveMessage = false;

        while(serverIsRun == true)
        {
            std::cout << "Clients poll call ..." << std::endl;
            int32_t ret = net::poll(fdArray, nfds, timeout);
            if (ret == 0) 
            {
                std::cout << "Clients poll call ... failed, timeout " << ret << " errno: "<< WSAGetLastError() << std::endl;
                break;
            }
            if (ret == POLLERR)
            {
                std::cout << "Clients poll call ... failed, got error " << ret << " errno: "<< WSAGetLastError() << &std::endl; 
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

                if (currentPollFd.revents != POLLWRNORM)
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
                        ret = net::read(currentPollFd.fd, request.data(), request.size());
                        if (ret < 0)
                        {
                            std::cout << "Read message ... error: " << ret << " errno: " << WSAGetLastError() <<&std::endl;
                            net::free(currentPollFd.fd);
                            break;
                        }

                        if (ret == net::status::disconnected)
                        {
                            std::cout << "Read message ... disconnected: " << ret << "errno: " << WSAGetLastError() <<&std::endl;
                            net::free(currentPollFd.fd);
                            break;
                        }

                        std::cout << "Read message ... complete, ret status: " << ret << " " << request.data() << " len: " << ret << std::endl;
                        haveMessage = true;
                        continue;
                    }

                    std::cout << "Write message ... " << std::endl;
                    ret = net::write(currentPollFd.fd, request.data(), ret);
                    if (ret < 0)
                    {
                        std::cout << "Write message ...  error: " << ret << " errno: " << WSAGetLastError() <<&std::endl;
                        net::free(currentPollFd.fd);
                        break;
                    }
                    if (ret == net::status::disconnected)
                    {
                        std::cout << "Write message ... disconnected: " << ret << "errno: " << WSAGetLastError() <<&std::endl;
                        net::free(currentPollFd.fd);
                        break;
                    }
                    haveMessage = false;
                    std::cout << "Write message ... complete, ret status: "<< ret << std::endl;

                } while(true); 
            } // end of loop through pollable descriptors 
        } // end of serving running.

        std::cout << "Close server ... " << &std::endl;
        net::free(tcp_server);
        std::cout << "Close server ... complete" << &std::endl;
        std::cout << "Close connection ... " << &std::endl;
        net::shutdown(tcp_client, net::enumShutdown::both);
        net::free(tcp_client);
        std::cout << "Close connection ... complete" << &std::endl;
        
    }
    catch (const std::exception& e)
    {
        std::cout << "Catch error: " << e.what() << &std::endl;
    }

    return 0;
}