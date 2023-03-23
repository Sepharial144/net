#ifndef _NET_SOCKET_DEFINITION_HPP_
#define _NET_SOCKET_DEFINITION_HPP_

#include "algo.hpp"

#if defined(_WIN32) && !defined(__linux__)

#include "win32_definitions.hpp"

#elif defined(linux) && !defined(_WIN32)

#include "linux_definitions.h"

#endif

#include <cstdint>

// settings of sockets
#define SOCKET_TCPIP_PACKET_MAX_SIZE 65536;
#define MAX_PORT_IPV4 65535;

namespace net {
	namespace socket {
		enum shutdownMode {
			receive = SD_RECEIVE, // shutdown receive messages on socket
			send = SD_SEND,  // shutdown send messages on socket
			both = SD_BOTH   // shutdown receive and send messages on socket
		};
	} // !namespace socket


	namespace addrinfo {

		enum aifamily: uint32_t {
			unspecified = AF_UNSPEC,	// The address family is unspecified.
			inetv4 = AF_INET,			// The Internet Protocol version 4 (IPv4)address family.
			ipx = AF_IPX,
			appletalk = AF_APPLETALK,
			netbios = AF_NETBIOS,		// The NetBIOS address family.This address family is only supported if a Windows Sockets provider for NetBIOS is installed.
			inetv6 = AF_INET6,			// The Internet Protocol version 6 (IPv6)address family.
			irda = AF_IRDA,				// The Infrared Data Association(IrDA) address family.This address family is only supported if the computer has an infrared portand driver installed.
			bluetooth = AF_BTH,			// The Bluetooth address family.This address family is only supported if a Bluetooth adapter is installed on Windows Server 2003 or later.
		};

		enum aisocktype: uint32_t {
			stream = SOCK_STREAM,	// socket stream mode
			dgram = SOCK_DGRAM,		// socket datagramm
			raw = SOCK_RAW,			// socket raw
			rdm = SOCK_RDM,
			seqpacket = SOCK_SEQPACKET,
		};

		enum aiprotocol: uint32_t {
			icmp = IPPROTO_ICMP,
			igmp = IPPROTO_IGMP,
			tcp = IPPROTO_TCP,
			udp = IPPROTO_UDP,
		};

		enum aiflags : uint32_t {
			passive = AI_PASSIVE,
			cannonname = AI_CANONNAME,
			numerichost = AI_NUMERICHOST,
			numericserv = AI_NUMERICSERV,
			all = AI_ALL,
			addrconfig = AI_ADDRCONFIG,
			v4mapped = AI_V4MAPPED,
			authoritative = AI_NON_AUTHORITATIVE,
			secure = AI_SECURE,
			return_prefered_names = AI_RETURN_PREFERRED_NAMES,
			fqdn = AI_FQDN,
			fileserver = AI_FILESERVER
		};

		// socket setting to pass into construtor of base socket
		struct SockSetting {
			aifamily aiFamily;// = aifamily::FAM_AF_INET;
			aisocktype aiSocktype;// = aisocktype::TYPE_SOCK_STREAM;
			aiprotocol aiProtocol;// = aiprotocol::PROTOCOL_TCP;
			aiflags aiFlags;// = aiflags::FLAG_AI_PASSIVE;
		};

		// connection setting to pass into construtor for client socket
		struct ConnectionSetting {
			aifamily aiFamily;// = aifamily::FAM_AF_INET;
			const char* ip_address;// = "127.0.0.1";
			const char* port;
			//int port;// = 0;
		};

	} // !namespace addrinfo
} // !namespace net


#endif // !_NET_SOCKET_DEFINITION_HPP_