#ifndef _NET_CLIENT_HPP_
#define _NET_CLIENT_HPP_

#include "socket_definition.hpp"

namespace net
{
	class client
	{
	private:
		friend class server;

	public:
		explicit client() = delete;
		explicit client(size_t len_message);
		~client();

		void close();

		int32_t send(const char* data, size_t len);
		size_t sendFrame(const char* data, size_t len);
		int32_t read(char* data, size_t len);

	private:
		int m_socket;
		size_t m_lenMessage;
	};
} // ! namespace net

#endif // !_NET_CLIENT_HPP_