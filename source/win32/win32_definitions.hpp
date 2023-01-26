#ifndef _WIN_SOCKET_DEFINITIONS_HPP_
#define _WIN_SOCKET_DEFINITIONS_HPP_

// common includes
#include <cassert>

#if defined(_WIN32) && !defined(__linux__)
	// windows

	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <iostream>

	//#pragma comment (lib, "Ws2_32.lib")
	//#pragma comment (lib, "Mswsock.lib")
	//#pragma comment (lib, "AdvApi32.lib")
#endif

#endif // !_WIN_SOCKET_DEFINITIONS_HPP_