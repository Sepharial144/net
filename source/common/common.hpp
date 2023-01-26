#ifndef _NET_COMMON_HPP_
#define _NET_COMMON_HPP_

namespace net
{
    namespace common {
#define ASCII_DEL_LETTER 0x7F // 127u - DEL

        // TODO: improve rank for another numbers not ranked - 1025 for example
        // which mean that 1024 - 2^10 and 2^0 which
        constexpr size_t getRank(size_t num);

        // TODO: upgrade algorithm to not ranked number
        size_t findLastReadableLetter(const char* data, size_t len, size_t limit);

    } // !namespace common
} // !namespace net

#endif // !_NET_HPP
