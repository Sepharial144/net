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
		explicit client();
		explicit client(const size_t len_message);
		~client();
		void close();
		void shutdown(const int16_t param);
		int32_t recieve(char* data, size_t len);
		int32_t send(const char* data, size_t len);

	private:
		SOCKET m_socket = { INVALID_SOCKET };
		size_t m_lenMessage = { 0ul };
	};
} // ! namespace net

#endif // !_NET_CLIENT_HPP_