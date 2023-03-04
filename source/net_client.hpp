#ifndef _NET_CLIENT_HPP_
#define _NET_CLIENT_HPP_

#include "BasicSocket.hpp"


namespace net
{
	class client: public BasicSocket
	{
	private:
		friend class server;

	public:
		explicit client() = delete;
		explicit client(size_t len_message);
		~client();

	private:
		SOCKET getSocket();

	private:
		size_t m_lenMessage;
	};
} // ! namespace net

#endif // !_NET_CLIENT_HPP_