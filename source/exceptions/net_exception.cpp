#include "net.hpp"
#include "net_exception.hpp"

#include <sstream>

#if defined(_WIN32) && !defined(linux)
#define NET_ERROR_CODE ::WSAGetLastError()
#elif defined(linux) && !defined(_WIN32)
#define NET_ERROR_CODE errno
#endif

// TODO: change definition of exception to crossplatform
namespace net
{
    void throw_exception_on(const bool is_error, const char* error)
    {
        if (is_error)
           throw net::exception(error);
    }

#if defined(_WIN32) && !defined(linux)
    exception::exception(const char* error)
        : m_errorCode{ NET_ERROR_CODE }
    {
        std::stringstream ss;
        ss << error << " wsa code: " << std::to_string(m_errorCode);
        m_error = ss.str();
    }

    exception::exception(const char* error, int32_t status_code)
        : m_errorCode{ NET_ERROR_CODE }
    {
        std::stringstream ss;
        ss << error << " wsa code: " << m_errorCode << " return code " << status_code;
        m_error = ss.str();
    }

#elif defined(linux) && !defined(_WIN32)
    exception::exception(const char* error)
        : m_errorCode{ NET_ERROR_CODE }
    {
        std::stringstream ss;
        ss << error << " code: " << std::to_string(m_errorCode);
        m_error = ss.str();
    }

    exception::exception(const char* error, int32_t status_code)
        : m_errorCode{ NET_ERROR_CODE }
    {
        std::stringstream ss;
        ss << error << " code: " << m_errorCode << " return code " << status_code;
        m_error = ss.str();
    }

#endif

    exception::~exception() noexcept
    {
    }

    const char* exception::what() const noexcept {
        return m_error.c_str();
    }

    int32_t exception::errorCode() const
    {
        return m_errorCode;
    }
} // !namespace net