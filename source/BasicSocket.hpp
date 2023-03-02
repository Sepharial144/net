#ifndef _NET_BASIC_SOCKET_HPP_
#define _NET_BASIC_SOCKET_HPP_


#if defined(_WIN32) && !defined(__linux__)

#include "win32_definitions.hpp"

#elif defined(__linux__) && !defined(_WIN32)

#include "linux_definitions.h"

#endif

namespace net
{
	class BasicSocket
	{
	public:
		explicit BasicSocket();
		explicit BasicSocket(const size_t bits);
		~BasicSocket();

		void close();

	protected:
		SOCKET& socketReference();

	private:
		WSADATA m_wsaData = {0};
		SOCKET m_socket = { INVALID_SOCKET };
		size_t m_bitsCount = {0};
	};
} // ! namespace net

#endif // !_NET_BASIC_SOCKET_HPP_