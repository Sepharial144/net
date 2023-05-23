#ifndef _NET_HPP_
#define _NET_HPP_

#include <cstdint>
#include <array>
#include <variant>

#if defined(_WIN32) && !defined(__linux__)

#include "win32_definitions.hpp"

#elif defined(linux) && !defined(_WIN32)

#include "linux_definitions.hpp"

#endif

// settings of sockets
#define NET_TCPIP_PACKET_MAX_SIZE 65536
#define NET_IPV4_MAX_PORT 65535

#if defined(_WIN32) && !defined(linux)
	#define NET_SOCKET_ERROR SOCKET_ERROR
#elif defined(linux) && !defined(_WIN32)
	#define NET_SOCKET_ERROR -1
	#define NET_SOCKET_EAGAIN_EXPR errno == EAGAIN
	#define NET_SOCKET_WBLOCK_EXPR errno == EWOULDBLOCK
#endif

namespace net {

	// TODO: impleemnt for windows
	enum status: int16_t
	{
		error = -1,
		disconnected = 0,
	};

	namespace socket {
		enum type: int16_t
		{
			blocking = 0,
			nonblocking = 1
		};


	} // namespace socket


#if defined(linux) && !defined(_WIN32)
	namespace pollc {

		enum param: int16_t
		{
			in          = POLLIN,  // data could be read
			prioriry    = POLLPRI, // prioirory data could be read
			out         = POLLOUT, // write data would be nonblocking
			pollerror   = POLLERR, // poll error
			polldisconn = POLLHUP, // disconnected
			notclose    = POLLNVAL // file descriptor is not closed
		};

		// XPG4.2
		enum xparam: int16_t
		{
			xin 		 = POLLRDNORM, // data could be read
			xin_prioriry = POLLRDBAND, // prioirory data could be read
			xout         = POLLWRNORM, // data could be write
			xout_prioriry = POLLWRBAND // prioirory data could be write
		};

		enum timeout: int16_t
		{
			immediately = 0,
			forever = -1
		};

		enum poll_ret: int16_t
		{
			poll_timeout = 0,
			poll_error = -1
		} ;
	} //namespace pollc
#endif

	enum enumShutdown : int16_t
	{
		receive = NET_SOCKET_RECEIVE, // shutdown receive messages on socket
		send = NET_SOCKET_SEND,	  	  // shutdown send messages on socket
		both = NET_SOCKET_BOTH		  // shutdown receive and send messages on socket
	};


	namespace settings {
		enum aifamily : int32_t {
			unspecified = AF_UNSPEC,	// The address family is unspecified.
			inetv4 = AF_INET,			// The Internet Protocol version 4 (IPv4)address family.
			ipx = AF_IPX,
			appletalk = AF_APPLETALK,
			inetv6 = AF_INET6,			// The Internet Protocol version 6 (IPv6)address family.
			irda = AF_IRDA,				// The Infrared Data Association(IrDA) address family.This address family is only supported if the computer has an infrared portand driver installed.

#if defined(_WIN32) && !defined(linux)
			netbios = AF_NETBIOS,
			bluetooth = AF_BTH,			// The Bluetooth address family.This address family is only supported if a Bluetooth adapter is installed on Windows Server 2003 or later.
#endif
		};

		enum aisocktype : int32_t {
			stream = SOCK_STREAM,	// socket stream mode
			dgram = SOCK_DGRAM,		// socket datagramm
			raw = SOCK_RAW,			// socket raw
			rdm = SOCK_RDM,
			seqpacket = SOCK_SEQPACKET,
		};

		enum aiprotocol : int32_t {
			icmp = IPPROTO_ICMP,
			igmp = IPPROTO_IGMP,
			tcp  = IPPROTO_TCP,
			udp  = IPPROTO_UDP,
		};

		enum aiflags : int32_t {
			passive = AI_PASSIVE,
			cannonname = AI_CANONNAME,
			numerichost = AI_NUMERICHOST,
			numericserv = AI_NUMERICSERV,
			all = AI_ALL,
			addrconfig = AI_ADDRCONFIG,
			v4mapped = AI_V4MAPPED,

#if defined(_WIN32) && !defined(linux)
			authoritative = AI_NON_AUTHORITATIVE,
			secure = AI_SECURE,
			return_prefered_names = AI_RETURN_PREFERRED_NAMES,
			fqdn = AI_FQDN,
			fileserver = AI_FILESERVER
#endif
		};

		// socket setting to pass into construtor of base socket
		struct server_t {
			aifamily aiFamily;			// = aifamily::FAM_AF_INET;
			aisocktype aiSocktype;		// = aisocktype::TYPE_SOCK_STREAM;
			aiprotocol aiProtocol;		// = aiprotocol::PROTOCOL_TCP;
			aiflags aiFlags;			// = aiflags::FLAG_AI_PASSIVE;
			uint32_t countConnection;
		};

		struct connection_t
		{
			aifamily aiFamily;     // = aifamily::FAM_AF_INET;
			aisocktype aiSocktype; // = aisocktype::TYPE_SOCK_STREAM;
			aiprotocol aiProtocol; // = aiprotocol::PROTOCOL_TCP;
			aiflags aiFlags;       // = aiflags::FLAG_AI_PASSIVE;

			#if defined(_WIN32) && !defined(linux)
			WSADATA wsaData;
			#endif
		};

		// connection setting to pass into construtor for client socket
		struct ConnectionSetting {
			aifamily aiFamily;      // = aifamily::FAM_AF_INET;
			const char* ip_address; // = "127.0.0.1";
			const char* port;
			//int port;// = 0;
		};
	} // !namespace settings

#if defined(_WIN32) && !defined(linux)
	typedef SOCKET socket_t;
	typedef WSAPOLLFD pollfd_s;
#elif defined(linux) && !defined(_WIN32)
	typedef int32_t socket_t;
	typedef pollfd pollfd_s;

	// TODO: implement with select
	typedef fd_set fdset_s;
#endif

	struct ip_address_s
	{
		u_short port;
		uint8_t address[INET6_ADDRSTRLEN];
		size_t addr_size;
		net::settings::aifamily type;
	};

	struct sockaddr_storage
	{
		net::ip_address_s ip_address;
		net::socket_t socket;
	};


#if defined(_WIN32) && !defined(linux)
	// TODO: make sockets asynchronous
	socket_t make_server(net::settings::server_t& setting, const char* address, int32_t port);
	socket_t make_server(net::settings::server_t& setting, const char* address, const char* port);
	socket_t make_connection(settings::connection_t& setting, const char* address, const char* port);
#elif defined(linux) && !defined(_WIN32)
	socket_t make_server(net::settings::server_t& setting, const char* address, int32_t port, net::socket::type sock_param);
	socket_t make_connection(settings::connection_t& setting, const char* address, const char* port, net::socket::type sock_param);

	[[deprecated]]
	socket_t make_async_server(net::settings::server_t& setting, const char* address, int32_t port);

	[[deprecated]]
	int32_t  wait_async_connection(net::socket_t& sock_server, socket_t& sock_client, int32_t connections);
#endif
	int32_t  wait_connection(net::socket_t& sock_server, socket_t& sock_client, int32_t connections);
	socket_t make_async_connection(settings::connection_t& setting, const char* address, const char* port);

#if defined(linux) && !defined(_WIN32)	
	void set_pollfd(net::pollfd_s pollfd_array[], uint64_t array_len, net::pollc::param param);
	int32_t poll(net::pollfd_s* pollfd_array, uint64_t array_len, int64_t timeout);
	void interpret_address(socket_t& sockfd, ip_address_s& ip_addr);
	bool is_connected(socket_t& sockfd);
#endif

	int32_t  read(net::socket_t& socket, char* data, size_t len);
	int32_t write(net::socket_t& socket, const char* data, size_t len);
	void shutdown(net::socket_t& socket, net::enumShutdown param);

	void free(net::socket_t& socket);
#if defined(_WIN32) && !defined(linux)
	void free(net::socket_t& socket, ::addrinfo* sock_address);
	void release();
#endif

	class server;
	class client;
	class connection;

	/*
	*	Server 
	*/
	class server final
	{
	public:
		explicit server(const net::settings::server_t& settings,
			const int32_t port);

		explicit server(const net::settings::server_t& settings,
			const char* addr,
			const char* port);
		~server();
		void close();
		int32_t waitConnection(client& client);

	private:
#if defined(_WIN32) && !defined(linux)
		WSADATA m_wsaData;
#endif
		net::settings::server_t m_serverSetting;
		socket_t m_socket;
		const char* m_address;
		std::variant<int32_t, const char*> m_defaultPort;
	};


	/*
	*	Client
	*/
	class client final
	{
	private:
		friend class server;

	public:
		explicit client();
		~client();
		void close();
		void shutdown(net::enumShutdown param);
		int32_t recieve(char* data, size_t len);
		int32_t send(const char* data, size_t len);

	private:
#if defined(_WIN32) && !defined(linux)
		sockaddr_storage m_sockaddrStorage;
#endif
		socket_t m_socket;
		net::settings::aifamily m_familyType;
		ip_address_s m_address;
	};


	/*
	*	Connection
	*/
	class connection final
	{
	public:
		explicit connection() = delete;
		explicit connection(const char* addr, const char* port);
		~connection();
		void close();
		int32_t connect();
		int32_t recieve(char* data, size_t len);
		int32_t send(const char* data, size_t len);
		size_t sendFrame(const char* data, size_t len);

	private:
#if defined(_WIN32) && !defined(linux)
		WSADATA m_wsaData;
		struct addrinfo* m_connectionSettings;
#endif
		socket_t m_socket;
		const char* m_address;
		const char* m_defaultPort = nullptr;
		size_t m_defaultLengthMessage;
	};

} // !namespace net

#endif // !_NET_HPP_