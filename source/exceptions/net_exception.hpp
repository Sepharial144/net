#ifndef _NET_SOCKET_EXCEPTION_HPP_
#define _NET_SOCKET_EXCEPTION_HPP_

#include <exception>
#include <string>

namespace net
{
    /*
    * Exception wrapper
    */
    void throw_exception_on(const bool is_error, const char* error);

    /*
    * Exception
    */
    class exception : public std::exception
    {
    public:
        explicit exception(const char* error);
        explicit exception(const char* error, int32_t status_code);
        virtual ~exception() noexcept;
        virtual const char* what() const noexcept;

#if defined(_WIN32) && !defined(linux)
        int32_t exception::errorCode() const;
#endif

    protected:
        std::string m_error;
#if defined(_WIN32) && !defined(linux)
        int32_t m_errorCode;
#endif
    };

} // !namespace net

#endif // !_NET_SOCKET_EXCEPTION_HPP_