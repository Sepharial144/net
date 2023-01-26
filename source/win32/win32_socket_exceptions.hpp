#ifndef _WIN_SOCKET_EXCEPTIONS_HPP_
#define _WIN_SOCKET_EXCEPTIONS_HPP_

#include "winsock2.h"

#if defined(_WIN32) && !defined(__linux__)

/*
namespace net {
	namespace socket {
		extern std::map<SocketExceptions, const char*> ExceptionsMap;
		extern enum SocketExceptions;
	}
} //namespace net
*/

#endif

#endif // !_WIN_SOCKET_EXCEPTIONS_HPP_