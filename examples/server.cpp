#include "net.hpp"

#include <iostream>
#include <array>

using namespace net;

int main()
{
    const char* address = "127.0.0.1";
    const char* port = "3000";

    net::server server(address, port);

    size_t messageSize = 1024ul;
    net::client client(messageSize);

    std::cout << "Wait connection... " << address << ":" << port << std::endl;
    int32_t status = server.waitConnection(client);

    std::cout << "Connected! " << status << std::endl;

    std::array<char, 1024> request;
    while (true)
    {
        if (client.recieve(request.data(), 1024))
        {
            std::cout << "Check message! status: " << status << " size: " << request.size() << std::endl;
            status = client.send(request.data(), request.size());
            if (status == request.size())
                std::cout << "Message sent : " << request.data() << std::endl;
            break;
        }
    }
    std::cout << "Disconnect!" << std::endl;

    system("PAUSE");

    return 0;
}