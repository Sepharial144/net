#include "net.hpp"
#include "exceptions/net_exception.hpp"

#include <iostream>
#include <array>
#include <cstdint>

int main()
{
    const char* address = "127.0.0.1";
    const char* port = "3000";

    constexpr size_t defaultLen = 1024ul;
    std::array<char, defaultLen> request = {0};
    std::string message = "Echo message";

    try
    {
        net::connection tcp_client(address, port);
        if (tcp_client.connect())
        {
            size_t len = message.size();
            int32_t status = tcp_client.send(message.data(), message.size());

            std::cout << "Message sent len: " << len << " status: " << status << &std::endl;
            std::cout << "Message sent: " << message.data() << &std::endl;

            int32_t requestLen = 0;
            while (requestLen != SOCKET_ERROR)
            {
                requestLen = tcp_client.recieve(request.data(), request.size());
                if (requestLen > 0)
                {
                    std::cout << "Request len: " << requestLen << &std::endl;
                    std::cout << "Request: " << request.data() << &std::endl;
                }
                if (requestLen == 0 || requestLen == WSAECONNRESET)
                {
                    std::cout << "Client: connection reset." << std::endl;
                    break;
                }
                if (requestLen == SOCKET_ERROR)
                    std::cout << "Client: connection closed." << std::endl;
            }
        }
        else 
        {
            throw net::exception("Connection failed");
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Client got error: " << e.what() << std::endl;
    }

    system("PAUSE");

    return 0;
}