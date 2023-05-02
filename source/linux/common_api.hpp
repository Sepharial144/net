#ifndef _COMMON_API_HPP_
#define _COMMON_API_HPP_

#include "net.hpp"

namespace net
{
    namespace api {

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
  
    } // namespace api
} // namespace net

#endif // !_COMMON_API_HPP_