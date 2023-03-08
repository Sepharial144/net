#include "net.hpp"
#include "net_client.hpp"
#include "net_server.hpp"

#include <iostream>
#include <array>
#include <thread> 

using namespace net;

int main()
{
    const char* address = "127.0.0.1";
    const char* port = "3000";

    net::server server(address, port, 1024ul);

    size_t messageSize = 1024ul;
    net::client client(messageSize);

    std::cout << "Wait connection... " << address << ":" << port << std::endl;
    int32_t status = server.waitConnection(client);

    std::cout << "Connected! " << status << std::endl;

    std::array<char, 1024> request;
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        int32_t len = client.recieve(request.data(), 1024ul);
        if (len != SOCKET_ERROR)
        {
            std::cout << "Check message! status: " << len << " size: " << request.size() << std::endl;
            status = client.send(request.data(), request.size());
            if (status == request.size())
                std::cout << "Message sent : " << request.data() << std::endl;
            break;
        }
        else
        {
            std::cout << "status: " << SOCKET_ERROR << std::endl;
        }
    }
    std::cout << "Disconnect!" << std::endl;

    system("PAUSE");

    return 0;
}