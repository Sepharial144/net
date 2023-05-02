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

    net::settings::SockSetting settings{
        net::settings::aifamily::inetv4,
        net::settings::aisocktype::stream,
        net::settings::aiprotocol::tcp,
        net::settings::aiflags::passive,
        nullptr,
        10ul
    };

    try
    {
        net::socket_t tcp_server = net::make_server(settings, address, port);
        net::socket_t tcp_client;
        
        if (net::wait_connection(tcp_server, tcp_client, 10))
        {
            std::cout << "Connection good" << &std::endl;
            std::string message = "Good message!";
            int32_t ret = net::write(tcp_client, message.data(), message.size());
            std::cout << "Write message: " << message.data() << " " << ret << &std::endl;

            std::array<char, 1024ul> request = { 0 };

            while(true)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                if (int32_t ret = read(tcp_client, request.data(), request.size()))
                {
                    std::cout << "Request: " << request.data() << " " << ret << &std::endl;
                    ret = net::write(tcp_client, request.data(), ret);
                    if(ret)
                        std::cout << "Request sent: " << ret << &std::endl;
                    break;
                }
            }
        }
        std::cout << "Close server ... " << &std::endl;
        net::free(tcp_server, settings.sockAddress);
        net::free(tcp_server);
        std::cout << "Close server ... complete" << &std::endl;
        std::cout << "Close connection ... " << &std::endl;
        net::shutdown(tcp_client, net::enumShutdown::both);
        net::free(tcp_client);
        net::release();
        std::cout << "Close connection ... complete" << &std::endl;
        
    }
    catch (const std::exception& e)
    {
        std::cout << "Catch error: " << e.what() << &std::endl;
    }

    system("PAUSE");
    return 0;
}