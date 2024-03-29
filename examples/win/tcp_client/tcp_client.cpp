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
        net::socket_t tcp_connection = net::make_connection(settings, argv[1], argv[2]);
        std::array<char, 1024ul> request = { 0 };

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