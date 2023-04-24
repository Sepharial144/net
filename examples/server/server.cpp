#include "net.hpp"

#include <iostream>
#include <array>
#include <thread> 

using namespace net;

int main()
{
    const char* address = "127.0.0.1";
    //const char* port = "3000";
    int32_t port = 3000;

    constexpr size_t messageSize = 1024ul;
    std::array<char, messageSize> request = { 0 };
    
    uint32_t countConnections = 10u;

    net::addrinfo::SockSetting settings{
        net::addrinfo::aifamily::inetv4,
        net::addrinfo::aisocktype::stream,
        net::addrinfo::aiprotocol::tcp,
        net::addrinfo::aiflags::passive,
        nullptr,
        countConnections
    };

    try
    {
        net::server tcp_server(settings, port);
        //net::server server(settings, address, port, messageSize);
        net::client client;

        std::cout << "Client size: " << sizeof(client) << &std::endl;
        std::cout << "Wait connection... " << address << ":" << port << &std::endl;
        int32_t status = tcp_server.waitConnection(client);

        std::cout << "Connected! " << status << &std::endl;

        int32_t len = 0;

        do
        {
            std::cout << "Do: " << request.data() << &std::endl;
            len = client.recieve(request.data(), messageSize);
            if (len > 0)
            {
                std::cout << "Message received: " << request.data() << &std::endl;
                std::cout << "Message len: " << len << " " << request.size() << &std::endl;
                status = client.send(request.data(), request.size());
                if (status == request.size())
                    std::cout << "Message sent : " << request.data() << &std::endl;
            }

        } while (len > 0);
        std::cout << "Disconnect!" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Catch error: " << e.what() << &std::endl;
    }
    system("PAUSE");

    return 0;
}