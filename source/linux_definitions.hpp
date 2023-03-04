#ifndef _LINUX_SOCKET_DEFINITIONS_HPP_
#define _LINUX_SOCKET_DEFINITIONS_HPP_

// common includes
#include <cassert>
#include <iostream>

#if defined(linux) && !defined(_WIN32)
	// linux
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#endif

#endif // !_LINUX_SOCKET_DEFINITIONS_HPP_