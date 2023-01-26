#ifndef _NET_CLIENT_HPP_
#define _NET_CLIENT_HPP_

#include "win32_definitions.hpp"
#include "socket_context.hpp"
#include "client_socket.hpp"

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
		int32_t recieve(char* data, size_t len);

	private:
		WSADATA m_wsaData;
		SOCKET m_socket;
		size_t m_lenMessage;
	};
} // ! namespace net

#endif // !_NET_CLIENT_HPP_