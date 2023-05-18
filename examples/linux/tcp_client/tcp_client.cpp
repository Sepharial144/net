#include "net.hpp"
#include "exceptions/net_exception.hpp"

#include <iostream>
#include <array>
#include <cstdint>
#include <thread>
#include <chrono>

int main()
{
    const char* address = "127.0.0.1";
    const char* port = "3000";

    net::settings::connection_t settings{
        net::settings::aifamily::inetv4,
        net::settings::aisocktype::stream,
        net::settings::aiprotocol::tcp,
        net::settings::aiflags::passive
    };

    try
    {
        net::socket_t tcp_connection = net::make_connection(settings, address, port);
        std::array<char, 1024ul> request = { 0 };
        std::string message{"init string"};
        int32_t count = 0;

        int32_t ret = net::write(tcp_connection, message.data(), message.size());
        if (ret)
            std::cout << "Request sent: " << message.data() << " " << ret << &std::endl;
        count++;
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
            if (int32_t ret = net::read(tcp_connection, request.data(), request.size()))
            {
                if (count == 2)
                    break;
                std::cout << "Request: " << request.data() << " " << ret << &std::endl;
                ret = net::write(tcp_connection, request.data(), ret);
                if (ret)
                    std::cout << "Request sent: " << request.data() << " " << ret << &std::endl;
                ++count;
            }
        }
        std::cout << "Connection close ..." << &std::endl;
        net::shutdown(tcp_connection, net::enumShutdown::both);
        net::free(tcp_connection);
        std::cout << "Connection close ... complete" << &std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Client got error: " << e.what() << std::endl;
    }
    return 0;
}