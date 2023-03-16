#include "net.hpp"

#include <iostream>
#include <array>
#include <thread> 

using namespace net;

int main()
{
    const char* address = "127.0.0.1";
    const char* port = "3000";

    constexpr size_t messageSize = 1024ul;
    std::array<char, messageSize> request = { 0 };
    net::server server(address, port, messageSize);

    net::client client(messageSize);

    std::cout << "Client size: " << sizeof(client) << &std::endl;
    std::cout << "Wait connection... " << address << ":" << port << &std::endl;
    int32_t status = server.waitConnection(client);

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

    } while(len > 0);
    std::cout << "Disconnect!" << std::endl;

    system("PAUSE");

    return 0;
}