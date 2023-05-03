#include "win32_common_api.hpp"
#include "exceptions/net_exception.hpp"

namespace net
{
	namespace api
	{
		void initializeWSA(WSADATA& wsa)
		{
			std::cout << "Initialization WSA ..." << &std::endl;
			int32_t ret = ::WSAStartup(MAKEWORD(2, 2), &wsa);
			net::throw_exception_on(ret != 0, "Netlib: initialization WSA failed");
			std::cout << "Initialization WSA ... complete" << &std::endl;
		}

		void releaseAddrinfo(::addrinfo* p_sockaddr)
		{
			if (p_sockaddr != nullptr)
				::freeaddrinfo(p_sockaddr);
			p_sockaddr = nullptr;
		}

		void interpretFamilyAddress(sockaddr_storage& addressStorage, net::ipAddress& address, net::settings::aifamily family)
		{
			if (family == net::settings::inetv4)
			{
				sockaddr_in* ptrIpv4ClientAddr = reinterpret_cast<sockaddr_in*>(&addressStorage);
				address.addr_size = static_cast<size_t>(INET_ADDRSTRLEN);
				address.port = ::ntohs(ptrIpv4ClientAddr->sin_port);
				::inet_ntop(net::settings::inetv4, ptrIpv4ClientAddr, reinterpret_cast<PSTR>(address.address), address.addr_size);
				return;
			}
			if (family == net::settings::inetv6)
			{
				sockaddr_in6* ptrIpv6ClientAddr = reinterpret_cast<sockaddr_in6*>(&addressStorage);
				address.addr_size = static_cast<size_t>(INET6_ADDRSTRLEN);
				address.port = ::ntohs(ptrIpv6ClientAddr->sin6_port);
				::inet_ntop(net::settings::inetv6, ptrIpv6ClientAddr, reinterpret_cast<PSTR>(address.address), address.addr_size);
				return;
			}
			throw std::runtime_error("Netlib: could not recognize address family");
		}
	} // namespace api
} // namespace net