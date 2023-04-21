#include "net.hpp"
#include "net_exception.hpp"

#include <sstream>

namespace net
{
    void throw_exception_on(const bool is_error, const char* error)
    {
        if (is_error)
           throw net::exception(error);
    }


#if defined(_WIN32) && !defined(linux)
    exception::exception(const char* error)
        : m_errorCode{ ::WSAGetLastError() }
    {
        std::stringstream ss;
        ss << " wsa code: " << std::to_string(m_errorCode);
        m_error = ss.str();
    }

    exception::exception(const char* error, int32_t status_code)
        : m_errorCode{ ::WSAGetLastError() }
    {
        std::stringstream ss;
        ss << " wsa code: " << m_errorCode << " return code " << status_code;
        m_error = ss.str();
    }

    int32_t exception::errorCode() const
    {
        return m_errorCode;
    }

#elif defined(linux) && !defined(_WIN32)
    exception::exception(const char* error)
        : m_error{error}
    {}

    exception::exception(const char* error, int32_t status_code)
        : m_error{ error }
    {
        m_error.append(" return code: ");
        m_error.append(std::to_string(status_code));
    }

#endif

    exception::~exception() noexcept
    {
    }

    const char* exception::what() const noexcept {
        return m_error.c_str();
    }
} // !namespace net