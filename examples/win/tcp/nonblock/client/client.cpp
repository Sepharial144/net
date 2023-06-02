#include "net.hpp"
#include "exceptions/net_exception.hpp"

#include <iostream>
#include <array>
#include <cstdint>
#include <thread>
#include <chrono>

int main(int argc, char* argv[])
{
    net::settings::connection_t settings{
        net::settings::aifamily::inetv4,
        net::settings::aisocktype::stream,
        net::settings::aiprotocol::tcp,
        net::settings::aiflags::passive,
        0
    };

    try
    {

        net::socket_t tcp_connection = net::make_connection(settings, argv[1], argv[2], net::socket::type::nonblocking);

        std::array<char, 1024ul> request = { 0 };
        net::pollfd_s fdArray = {0};
        fdArray.fd = tcp_connection;
        fdarray.events = POLLWRNORM;

        std::string message{ "Super Message!" };
        constexpr uint64_t timeout = DEFAULT_WAIT;

        if(int32_t ret = net::poll(fdArray, 1, timeout))
        {
            std::cout << "Connection established ... " << argv[1] << ":" <<argv[2] << &std::endl;
            std::cout << "send data ... " << argv[1] << ":" <<argv[2] << std::endl;
            net::write(tcp_connection, message.data(), message.size());
        } else 
        {
            std::cout << "Connection failed ... " << argv[1] << ":" <<argv[2] << &std::endl;
            std::cout << "poll status ... " << ret << &std::endl;
        }

        bool hasMessage = false;
        bool serverIsRun = true;
        while(serverIsRun)
        {
            int32_t ret = net::poll(fdArray, 1, timeout);
            if(ret != SOCKET)
            {
                std::cout << "read ... " << &std::endl;
                ret = net::read(tcp_connection, request.data(), request.size());
                std::cout << "read ... " << request.data() << " len: " << ret << &std::endl;
            } else {
                std::cout << "read ... error ... " << ret << &std::endl;
            }

            if(int32_t ret = net::poll(fdArray, 1, timeout))
            {
                std::cout << "write ... " << &std::endl;
                ret = net::write(tcp_connection, request.data(), request.size());
                std::cout << "writen ... " << request.data() << " len: " << ret << &std::endl;
                std::memset(request.data(), 0, request.size());
                continue;
            }
            else
            {
                std::cout << "write ... error ... " << ret << &std::endl;
            }
        }

/*
        if (fdArray.revents & POLLWRNORM)
        {
            std::cout << "Connection established ... " << argv[1] << ":" <<argv[2] << std::endl;
            std::cout << "send data ... " << argv[1] << ":" <<argv[2] << std::endl;
        }

        fdarray.events = POLLRDNORM;
        ret = ::WSAPoll(&fdArray, 1, DEFAULT_WAIT);
        net::throw_exception_on(ret == SOCKET_ERROR, "WsaPoll read failed");

        if (fdArray.revents & POLLRDNORM)
        {
            std::cout << "read data ... " << argv[1] << ":" <<argv[2] << std::endl
            ret = net::read(tcp_connection, request.data(), request.size())
            std::cout << "read data ... " << ret << " bytes, message: " << request.data() << std::endl;
        }
*/

        /*
        int32_t count = 0;
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (int32_t ret = net::read(tcp_connection, request.data(), request.size()))
            {
                ++count;
                std::cout << "Request: " << request.data() << " " << ret << &std::endl;
                if (count == 2)
                    break;
                ret = net::write(tcp_connection, request.data(), ret);
                if (ret)
                    std::cout << "Request sent: " << request.data() << " " << ret << &std::endl;
            }
        }
        */
        std::cout << "Connection close ..." << &std::endl;
        net::shutdown(tcp_connection, net::enumShutdown::both);
        net::free(tcp_connection);
        net::release();
        std::cout << "Connection close ... complete" << &std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Client got error: " << e.what() << std::endl;
    }
    
    system("PAUSE");
    return 0;
}