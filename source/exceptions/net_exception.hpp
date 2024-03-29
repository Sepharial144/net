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
    void throw_exception_on_poll(const int32_t ret_status, const char* call_placement);
    void throw_exception_on_revents(const int32_t revent, 
                                    const net::pollc::param exclude, 
                                    const char* label_excep);

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

        int32_t errorCode() const;

    protected:
        std::string m_error;
        int32_t m_errorCode;
    };

} // !namespace net

#endif // !_NET_SOCKET_EXCEPTION_HPP_