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

namespace net
{
    void throw_exception_on(const bool is_error, const char* error)
    {
        if (is_error)
           throw net::exception(error);
    }


    void throw_exception_on_poll(const int32_t ret_status, const char* call_placement)
    {
        if (ret_status > 0)
            return;

        // TODO: findout behavior of this value, might not error
        if (ret_status == 0)
        {
            std::string pollError{call_placement};
            pollError.append(" poll timeout error,");
            throw net::exception(pollError.c_str());
        }

        if (ret_status == net::pollc::poll_error)
        {
            std::string pollError{call_placement};
            pollError.append(" call poll failed,");
            throw net::exception(pollError.c_str());
        }
    }


    void throw_exception_on_revents(const int32_t revent, const net::pollc::param exclude, const char* label_excep)
    {
        if (revent > 0)
        {
            return;
        }

		if (revent == 0)
		{
            std::string reventError{label_excep};
            reventError.append(" unexpected revents error");
			throw net::exception(reventError.c_str());
		}

		if (revent != exclude)
		{
            std::string reventError{label_excep};
            reventError.append(" revents error");
			throw net::exception(reventError.c_str());
		}
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