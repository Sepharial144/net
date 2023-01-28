#include "net.hpp"


#include <iostream>
#include <array>

#include <math.h>

using namespace net;


#define ASCII_DEL_LETTER 0x7F // 127u - DEL

// TODO: improve rank for another numbers not ranked - 1025 for example
// which mean that 1024 - 2^10 and 2^0 which
constexpr size_t getRank(size_t num)
{
    double result = 0.0f;
    size_t value = 0ul;
    while (num != static_cast<size_t>(result))
    {
        result = pow(2ul, value);
        ++value;
    }
    return static_cast<size_t>(value - 1.0f);
}

// TODO: upgrade algorithm to not ranked number
size_t findLastReadableLetter(const char* data, size_t len, size_t limit)
{
    constexpr int16_t expect = static_cast<uint16_t>(ASCII_DEL_LETTER);
    // check array is not empty
    if (static_cast<int16_t>(*data) >= expect)
        return 0;
    // check array is full of letters
    uint16_t expect2 = static_cast<uint16_t>(*(data + len));
    if (expect2 < expect)
        return len;

    size_t half = len / 2ul;
    size_t carriage = half;

    for (size_t i = 1ul; i != limit; ++i)
    {
        const char* checkLetter = data + carriage;
        uint16_t letter = static_cast<uint16_t>(*checkLetter);
        half /= 2ul;
        if (letter < expect)
        {
            carriage += half;
            continue;
        }
        carriage -= half;
    }
    return carriage;
}

int main()
{
/*
	std::array<uint8_t, 1024> buffer;
	std::string request = "GET / HTTP/1.1/r/n Accept=dfgdfg";

	net::addrinfo::SockSetting sockSetting;
    sockSetting.aiFamily = net::addrinfo::aifamily::FAM_AF_UNSPEC;
    sockSetting.aiSocktype = net::addrinfo::aisocktype::TYPE_SOCK_STREAM;
    sockSetting.aiProtocol = net::addrinfo::aiprotocol::PROTOCOL_TCP;
    sockSetting.aiFlags = net::addrinfo::aiflags::FLAG_AI_PASSIVE;

	net::addrinfo::ConnectionSetting conSetting;
    conSetting.aiFamily = net::addrinfo::aifamily::FAM_AF_UNSPEC;
    conSetting.ip_address = "127.0.0.1";
    conSetting.port = "3000";

	net::socket::ClientSocket client(sockSetting, conSetting, 1024, 3000);
	client.connect();
	client.send(request.data(), request.size());
    
    system("PAUSE");
*/

    const char* address = "127.0.0.1";
    const char* port = "3000";

    std::array<char, 1024> request;
    std::string message = "Super message";

    net::socket::connection client(address, port);
    
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
                size_t rank = getRank(request.size());
                std::cout << "Rank: " << rank << std::endl;
                size_t lenMessage = findLastReadableLetter(request.data(), request.size(), rank);
                std::cout << "Len message: " << request.data() << "| " << lenMessage << std::endl;
                uint8_t letter = *(request.data() + lenMessage);
                std::cout << "Symbol: " << (uint16_t)letter << std::endl;
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

    /*
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
    */

	return 0;
}