#ifndef _WIN_SOCKET_DEFINITIONS_HPP_
#define _WIN_SOCKET_DEFINITIONS_HPP_

// common includes
#include <cassert>
#include <cstdint>
#include <iostream>

#if defined(_WIN32) && !defined(linux)
// windows

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

/*
* Socket error
*/
#define NET_SOCKET_ERROR SOCKET_ERROR
// is connection reset on linux as -1?
#define NET_CONNECTION_RESET WSAECONNRESET

/*
* Socket shutdown parameter
*/
#define NET_SOCKET_RECEIVE SD_RECEIVE
#define NET_SOCKET_SEND	   SD_SEND
#define NET_SOCKET_BOTH	   SD_BOTH

#define NET_SOCKET_WOULD_BLOCK EWOULDBLOCK
#define NET_SOCKET_EAGAIN	   

#endif

#endif // !_WIN_SOCKET_DEFINITIONS_HPP_