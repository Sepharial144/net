#include "net_api.hpp"
#include <cstring>

namespace net::api 
{
    const char* translateAddress(const char* address)
    {
        return (std::strcmp(address, "localhost") == 0)? 
            "127.0.0.1": 
            address;
    }
} // namespace net::api