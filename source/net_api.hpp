#ifndef _NET_API_HPP_
#define _NET_API_HPP_

#include "net.hpp"

namespace net::api
{
    const char* translateAddress(const char* address);

    template<typename Numeric>
    Numeric translatePort(const char* port)
    {
        Numeric result = *port - 48;
        while(char letter = *(++port))
        {
            result *= 10;
            result += letter - 48;
        }
        return result;
    }
} // namespace net::api

#endif // !_NET_API_HPP_