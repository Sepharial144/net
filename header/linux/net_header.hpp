#ifndef _NET_STANDALONE_HPP_
#define _NET_STANDALONE_HPP_


#include <cstdint>
#include <array>
#include <variant>
#include <cassert>
#include <cstdint>

// linux
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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

#define NET_SOCKET_WOULD_BLOCK EWOULDBLOCK
#define NET_SOCKET_EAGAIN	   EAGAIN

// settings of sockets
#define NET_TCPIP_PACKET_MAX_SIZE 65536
#define NET_IPV4_MAX_PORT 65535

#define NET_SOCKET_ERROR -1
#define NET_SOCKET_EAGAIN_EXPR errno == EAGAIN
#define NET_SOCKET_WBLOCK_EXPR errno == EWOULDBLOCK

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

	typedef int32_t socket_t;
	typedef pollfd pollfd_s;

	// TODO: implement with select
	typedef fd_set fdset_s;

	struct ip_address_s
	{
		u_short port;
		const char* address;
		size_t addr_size;
		net::settings::aifamily type;
	};

	struct sockaddr_storage
	{
		net::ip_address_s ip_address;
		net::socket_t socket;
	};

	/*
	*	Net exceptions
	*/

	#define NET_ERROR_CODE errno
	#define NET_OS_MESSAGE " code: "

	/*
    * 	Net exception wrapper
    */
    void throw_exception_on(const bool is_error, const char* error)
    {
        if (is_error)
           throw net::exception(error);
    }


    void throw_exception_on_poll(const int32_t ret_status, const char* call_placement)
    {
        if (ret_status > 0)
            return;

        // TODO: findout behavior of this value, might not error
        if (ret_status == 0)
        {
            std::string pollError{call_placement};
            pollError.append(" poll timeout error,");
            throw net::exception(pollError.c_str());
        }

        if (ret_status == net::pollc::poll_error)
        {
            std::string pollError{call_placement};
            pollError.append(" call poll failed,");
            throw net::exception(pollError.c_str());
        }
    }


    void throw_exception_on_revents(const int32_t revent, const net::pollc::param exclude, const char* label_excep)
    {
        if (revent > 0)
        {
            return;
        }

		if (revent == 0)
		{
            std::string reventError{label_excep};
            reventError.append(" unexpected revents error");
			throw net::exception(reventError.c_str());
		}

		if (revent != exclude)
		{
            std::string reventError{label_excep};
            reventError.append(" revents error");
			throw net::exception(reventError.c_str());
		}
    }


    /*
    * 	Net exception
    */
    class exception : public std::exception
    {
    public:
        explicit exception(const char* error)
        : m_errorCode{ NET_ERROR_CODE }
		{
			std::stringstream ss;
			ss << error << NET_OS_MESSAGE << std::to_string(m_errorCode);
			m_error = ss.str();
		}

        explicit exception(const char* error, int32_t status_code)
        : m_errorCode{ NET_ERROR_CODE }
		{
			std::stringstream ss;
			ss << error << NET_OS_MESSAGE << m_errorCode << " return code " << status_code;
			m_error = ss.str();
		}

        virtual ~exception() noexcept
		{

		}

        virtual const char* what() const noexcept
		{
        	return m_error.c_str();
    	}

        int32_t errorCode() const
		{
			return m_errorCode;
		}

    protected:
        std::string m_error;
        int32_t m_errorCode;
    };


	socket_t make_server(net::settings::server_t& setting, const char* address, int32_t port, net::socket::type sock_param)
	{
		std::cout << "Server initializing ..." << &std::endl;

		if (port < static_cast<int32_t>(0))
			throw std::logic_error("Netlib: server port should not be negative");

		socket_t sockServer = ::socket(setting.aiFamily, setting.aiSocktype, setting.aiProtocol);
		net::throw_exception_on(sockServer < 0, "Netlib: server create socket failed");

		constexpr int32_t on = 1;
		int32_t ret = ::setsockopt(sockServer, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
		net::throw_exception_on(ret < 0, "Netlib: asynchronous server set socket option failed");

		if (sock_param == net::socket::type::nonblocking)
		{
			ret = ::ioctl(sockServer, FIONBIO, &sock_param);
			net::throw_exception_on(ret < 0, "Netlib: asynchronous server ioctl call failed");
		}

		struct sockaddr_in addr = { 0 };
		addr.sin_family = setting.aiFamily;
		addr.sin_port = ::htons(port);
		// TODO: create for certain address macro
		addr.sin_addr.s_addr = ::htonl(INADDR_ANY);

		ret = ::bind(sockServer, (struct sockaddr*)&addr, sizeof(addr));
		net::throw_exception_on(ret < 0, "Netlib: server bind failed");

	    ret = ::listen(sockServer, 1);
        net::throw_exception_on(ret < 0, "Netlib: server listening failed");

        std::cout << "Server initializing ... complete" << &std::endl;

        return sockServer;
	}

	socket_t make_connection(settings::connection_t& setting, const char* address, const char* port, net::socket::type sock_param)
	{
		std::cout << "Connection initialization ..." << &std::endl;

        socket_t sockConnection = ::socket(setting.aiFamily, setting.aiSocktype, setting.aiProtocol);

		constexpr int32_t on = 1;
		int32_t ret = ::setsockopt(sockConnection, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
		net::throw_exception_on(ret < 0, "Netlib: connection set socket option failed");

		struct hostent *hostInfo = ::gethostbyname(address);
		if (hostInfo == nullptr)
			throw net::exception("Netlib: error while resolve address name");

        struct sockaddr_in addr = { 0 };
        addr.sin_family = setting.aiFamily;
		addr.sin_port = ::htons(net::api::translatePort<int32_t>(port));
        addr.sin_addr.s_addr = ::inet_addr(::inet_ntoa(*(struct in_addr*)hostInfo->h_addr_list[0]));

		if (sock_param == net::socket::type::nonblocking)
		{
			ret = ::ioctl(sockConnection, FIONBIO, &sock_param);
			net::throw_exception_on(ret < 0, "Netlib: connection ioctl call failed");
		
			do{
				std::this_thread::sleep_for(std::chrono::microseconds(37));
				ret = ::connect(sockConnection, (struct sockaddr*)&addr, sizeof(addr));
				std::cout << "Connection async status: " << ret << " errno: " << errno << &std::endl;
			} while(ret != 0);	
			std::cout << "Connection async status ready: " << ret << " errno: " << errno << &std::endl; 
		} else 
		{
			ret = ::connect(sockConnection, (struct sockaddr*)&addr, sizeof(addr));
			net::throw_exception_on(ret < 0, "Netlib: connection failed");
		}

        std::cout << "Connection initialization ... complete" << &std::endl;
        return sockConnection;
	}

	int32_t  wait_connection(net::socket_t& sock_server, socket_t& sock_client, int32_t connections)
	{
		std::cout << "Server wait connection ... " << &std::endl;
        net::throw_exception_on(sock_client > 0, "Netlib: provided client is not closed");

		do
		{
			sock_client = ::accept(sock_server, nullptr, nullptr);
		} while(sock_client < 0 && (errno == EAGAIN || errno == EWOULDBLOCK));
		if (sock_client < 0)
		{
			std::cout << "Asynchronous server client socket got accept error" << &std::endl;
			return 0;
		}

		std::cout << "Server got connection socket id ... " << sock_client << &std::endl;
		return 1;
	}

	socket_t make_async_connection(settings::connection_t& setting, const char* address, const char* port)
	{
		std::cout << "Asynchronous server wait connection ... " << &std::endl;
        net::throw_exception_on(sock_client > 0, "Netlib: provided client is not closed");
		
		do
		{
			sock_client = ::accept(sock_server, nullptr, nullptr);
		} while(sock_client < 0 && (errno == EAGAIN || errno == EWOULDBLOCK));
		if (sock_client < 0)
		{
			std::cout << "Asynchronous server client socket got accept error" << &std::endl;
			return 0;
		}

		std::cout << "Asynchronous serverr got connection socket id ... " << sock_client << &std::endl;
		return 1;
	}

	void set_pollfd(net::pollfd_s pollfd_array[], uint64_t array_len, net::pollc::param param)
	{
		for(size_t i = 0ul; 0 < array_len; ++i)
			pollfd_array[i].events = param;
	}

	int32_t poll(net::pollfd_s* pollfd_array, uint64_t array_len, int64_t timeout)
	{
		return ::poll(pollfd_array, array_len, timeout);
	}

	void interpret_address(socket_t& sockfd, ip_address_s& ip_addr)
	{
		struct sockaddr_in sockAddress = {0};
		size_t szAddress = sizeof(sockAddress);
		int32_t ret =  ::getpeername(sockfd, (struct sockaddr*)(&sockAddress), (socklen_t*)(&szAddress));
		net::throw_exception_on(ret == -1, "Netlib: interpret address failed");

		ip_addr.address = ::inet_ntoa(sockAddress.sin_addr);
		ip_addr.addr_size = std::strlen(ip_addr.address);
		ip_addr.type = (ip_addr.addr_size == INET6_ADDRSTRLEN)? net::settings::aifamily::inetv6 : net::settings::aifamily::inetv4;
		ip_addr.port = ::htons(sockAddress.sin_port);
	}

	bool is_connected(socket_t& sockfd)
	{
		struct sockaddr_in sockAddress = {0};
		size_t szAddress = sizeof(sockAddress);
		int32_t ret = ::getpeername(sockfd, (struct sockaddr*)(&sockAddress), (socklen_t*)(&szAddress));
		if (ret != -1)
			return true;
		return false;
	}

	int32_t  read(net::socket_t& socket, char* data, size_t len)
	{
		return ::recv(socket, data, len, 0);
	}

	int32_t write(net::socket_t& socket, const char* data, size_t len)
	{
		return ::send(socket, data, len, 0);
	}

	void shutdown(net::socket_t& socket, net::enumShutdown param)
	{
		// TODO: check different calls 
		std::cout << "shutdown socket ... " << &std::endl;
		int32_t ret = ::shutdown(socket, static_cast<int>(param));
		net::throw_exception_on(ret < 0, "Netlib: client shutdown socket failed");
		std::cout << "shutdown socket ... complete" << &std::endl;
	}

	void free(net::socket_t& socket)
	{
		std::cout << "Close socket ..." << &std::endl;
		if (socket != 0)
		{
			std::cout << "Close socket try ... " << &std::endl;
			int32_t ret = ::close(socket);
			net::throw_exception_on(ret < 0, "Netlib: close failed");
		}
		std::cout << "Close socket ... complete" << &std::endl;
	}

} // !namespace net

#endif // !_NET_STANDALONE_HPP_