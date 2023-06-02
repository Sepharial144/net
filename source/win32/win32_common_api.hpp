#ifndef _WIN32_COMMON_API_HPP_
#define _WIN32_COMMON_API_HPP_

#include "net.hpp"
#include "win32_definitions.hpp"

namespace net
{
    namespace api {

        void initializeWSA(WSADATA& wsa);
        void releaseAddrinfo(::addrinfo* p_sockaddr);
        void interpretFamilyAddress(sockaddr_storage& addressStorage, 
                                    net::ip_address_s& address, 
                                    net::settings::aifamily family);
  
    } // !namespace api
} // !namespace net

#endif // !_WIN32_COMMON_API_HPP_