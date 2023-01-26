#ifndef _SOCKET_CONTEXT_HPP_
#define _SOCKET_CONTEXT_HPP_

#include "win32/win32_definitions.hpp"

#if defined(_WIN32) && !defined(__linux__)

//#include "win32/win32_definitions.hpp"

#elif defined(__linux__) && !defined(_WIN32)

#include "../socket/linux/linux_definitions.h"

#endif

#include <cstdint>

// settings of sockets
#define SOCKET_TCPIP_PACKET_MAX_SIZE 65536;
#define MAX_PORT_IPV4 65535;

namespace net {
	namespace socket {

		enum socketType {
			NO_TYPE,
			SERVER,
			CONNECTION,
			CLIENT
		};

		enum shutdownMode {
			RECEIVE = SD_RECEIVE,
			SEND = SD_SEND,
			RECIVE_AND_SEND = SD_BOTH
		};

		enum status {
			Error,
			Initializing,
			Created,
			Binded,
			Listening,
			Connected,
			STOPED_RECEIVE,
			STOPED_SEND,
			STOPED_RECIVE_SEND,
			Closed
		};


	} // !namespace socket


	namespace addrinfo {

		enum aifamily: uint32_t {
			FAM_AF_UNSPEC = AF_UNSPEC,		// The address family is unspecified.
			FAM_AF_INET = AF_INET,			// The Internet Protocol version 4 (IPv4)address family.
			FAM_AF_IPX = AF_IPX,
			FAM_APPLETALK = AF_APPLETALK,
			FAM_AF_NETBIOS = AF_NETBIOS,	// The NetBIOS address family.This address family is only supported if a Windows Sockets provider for NetBIOS is installed.
			FAM_AF_INET6 = AF_INET6,		// The Internet Protocol version 6 (IPv6)address family.
			FAM_AF_IRDA = AF_IRDA,			// The Infrared Data Association(IrDA) address family.This address family is only supported if the computer has an infrared portand driver installed.
			FAM_AF_BTH = AF_BTH,			// The Bluetooth address family.This address family is only supported if a Bluetooth adapter is installed on Windows Server 2003 or later.
		};

		enum aisocktype: uint32_t {
			TYPE_SOCK_STREAM = SOCK_STREAM,	// socket stream mode
			TYPE_SOCK_DGRAM = SOCK_DGRAM,	// socket datagramm
			TYPE_SOCK_RAW = SOCK_RAW,		// socket raw
			TYPE_SOCK_RDM = SOCK_RDM,
			TYPE_SOCK_SEQPACKET = SOCK_SEQPACKET,
		};

		enum aiprotocol: uint32_t {
			PROTOCOL_ICMP = IPPROTO_ICMP,
			PROTOCOL_IGMP = IPPROTO_IGMP,
			PROTOCOL_TCP = IPPROTO_TCP,
			PROTOCOL_UDP = IPPROTO_UDP,
		};

		enum aiflags : uint32_t {
			FLAG_AI_PASSIVE = AI_PASSIVE,
			FLAG_AI_CANONNAME = AI_CANONNAME,
			FLAG_AI_NUMERICHOST = AI_NUMERICHOST,
			FLAG_AI_NUMERICSERV = AI_NUMERICSERV,
			FLAG_AI_ALL = AI_ALL,
			FLAG_AI_ADDRCONFIG = AI_ADDRCONFIG,
			FLAG_AI_V4MAPPED = AI_V4MAPPED,
			FLAG_AI_NON_AUTHORITATIVE = AI_NON_AUTHORITATIVE,
			FLAG_AI_SECURE = AI_SECURE,
			FLAG_AI_RETURN_PREFERRED_NAMES = AI_RETURN_PREFERRED_NAMES,
			FLAG_AI_FQDN = AI_FQDN,
			FLAG_AI_FILESERVER = AI_FILESERVER
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


#endif // !_SOCKET_CONTEXT_HPP_