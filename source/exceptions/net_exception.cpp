#include "net.hpp"
#include "net_exception.hpp"

#include <sstream>

#if defined(_WIN32) && !defined(linux)
#define NET_ERROR_CODE ::WSAGetLastError()
#define NET_OS_MESSAGE " wsa code: "
#elif defined(linux) && !defined(_WIN32)
#define NET_ERROR_CODE errno
#define NET_OS_MESSAGE " code: "
#endif

// TODO: change definition of exception to crossplatform
namespace net
{
    void throw_exception_on(const bool is_error, const char* error)
    {
        if (is_error)
           throw net::exception(error);
    }

    exception::exception(const char* error)
        : m_errorCode{ NET_ERROR_CODE }
    {
        std::stringstream ss;
        ss << error << NET_OS_MESSAGE << std::to_string(m_errorCode);
        m_error = ss.str();
    }

    exception::exception(const char* error, int32_t status_code)
        : m_errorCode{ NET_ERROR_CODE }
    {
        std::stringstream ss;
        ss << error << NET_OS_MESSAGE << m_errorCode << " return code " << status_code;
        m_error = ss.str();
    }

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