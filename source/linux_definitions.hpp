#ifndef _LINUX_SOCKET_DEFINITIONS_HPP_
#define _LINUX_SOCKET_DEFINITIONS_HPP_

// common includes
#include <cassert>
#include <cstdint>
#include <iostream>

#if defined(linux) && !defined(_WIN32)
	// linux
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>

/*
* Socket error
*/
#define NET_SOCKET_ERROR -1
// IIQ: is connection reset on linux as -1?
#define NET_CONNECTION_RESET -1
/*
* Socket shutdown parameter
*/
#define NET_SOCKET_RECEIVE SHUT_RD
#define NET_SOCKET_SEND	   SHUT_WR
#define NET_SOCKET_BOTH    SHUT_RDWR

#endif

#endif // !_LINUX_SOCKET_DEFINITIONS_HPP_