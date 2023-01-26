#include "common.hpp"

#include <math.h>
#include <cstdint>

namespace net
{
    namespace common {
        // TODO: improve rank for another numbers not ranked - 1025 for example
        // which mean that 1024 - 2^10 and 2^0 which
        constexpr size_t getRank(size_t num)
        {
            double result = 0.0f;
            double value = 0.0f;
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

    } // !namespace common
} // !namespace net