#ifndef _NET_SERVER_HPP_
#define _NET_SERVER_HPP_

#include "net_client.hpp"


namespace net
{
	class server: public BasicSocket
	{
	public:
		explicit server(const char* addr, int32_t port);
		~server();

		void listening();
		int32_t waitConnection(client& client);

	private:
		const char* m_address = nullptr;
		int32_t m_defaultPort = {0};
		sockaddr_in m_serverSettings = {0};
	};
} // ! namespace net

#endif // !_NET_SERVER_HPP_