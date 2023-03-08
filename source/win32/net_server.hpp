#ifndef _NET_SERVER_HPP_
#define _NET_SERVER_HPP_

#include "net_client.hpp"
#include "BasicServer.hpp"

namespace net
{
	class server: public BasicServer
	{
	public:
		explicit server(const char* addr, const char* port, const size_t default_len);
		~server();

		void listening();
		int32_t waitConnection(client& client);

	private:
		const char* m_address = nullptr;
		const char* m_defaultPort = nullptr;
	};
} // ! namespace net

#endif // !_NET_SERVER_HPP_