#ifndef _WIN32_COMMON_API_HPP_
#define _WIN32_COMMON_API_HPP_

#include "win32_definitions.hpp"

namespace net
{
    namespace api {

        // TODO: make this variable for different thread
        static inline int64_t WSA_CALL_COUNT = 0ul;
        static inline WSADATA WSA_DATA = { 0 };

        void   initializeWSA();
        void uninitializeWSA();
    
    } // !namespace api
} // !namespace net

#endif // !_WIN32_COMMON_API_HPP_