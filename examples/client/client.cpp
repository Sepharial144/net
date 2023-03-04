#include "net.hpp"
#include "net_connection.hpp"

#include <iostream>
#include <array>

int main()
{
    const char* address = "127.0.0.1";
    const char* port = "3000";

    std::array<char, 1024> request;
    std::string message = "Super message";

    net::connection client(address, port);

    if (client.connect())
    {
        size_t len = message.size();
        int32_t status = client.send(message.data(), message.size());

        std::cout << "Message sent len: " << len << " status: " << status << std::endl;

        int32_t requestLen = SOCKET_ERROR;
        while (requestLen == SOCKET_ERROR)
        {
            requestLen = client.read(request.data(), request.size());
            if (requestLen > 0)
            {
                std::cout << "Request len: " << requestLen << std::endl;
                //size_t rank = common::getRank(request.size());
                //std::cout << "Rank: " << rank << std::endl;
                //size_t lenMessage = common::findLastReadableLetter(request.data(), request.size(), rank);
                //std::cout << "Len message: " << request.data() << "| " << lenMessage << std::endl;
                //uint8_t letter = *(request.data() + lenMessage);
                //std::cout << "Symbol: " << (uint16_t)letter << std::endl;
                std::cout << "Symbol2: " << (uint8_t)126 << std::endl;
                break;
            }
            if (requestLen == 0 || requestLen == WSAECONNRESET)
            {
                printf("Client: Connection Closed.\n");
                break;
            }
            else
                printf("Client: recv() is OK.\n");
            if (requestLen < 0)
            {
                std::cout << "Bad request len: " << requestLen << std::endl;
                break;
            }
            else
                printf("Client: Bytes received - %ld.\n", requestLen);

        }
    }

    system("PAUSE");

    return 0;
}