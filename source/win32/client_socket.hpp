#ifndef _CLIENT_SOCKET_HPP_
#define _CLIENT_SOCKET_HPP_

#include "win32_definitions.hpp"
#include "socket_context.hpp"

namespace net {
	namespace socket {
		class ClientSocket
		{
		public:
			explicit ClientSocket(addrinfo::SockSetting& settings, 
				                  addrinfo::ConnectionSetting& conn_settings, 
								  uint32_t buffer_length, 
								  int32_t default_port);
			~ClientSocket();
			status connect();
			status shutdown(shutdownMode mode);
			status status();
			net::socket::status close();
			void error(const char* title, const char* message, int32_t value);
			int32_t send(const char* data, size_t len);
			int32_t read(char* data, size_t len);

		private:
			addrinfo::SockSetting m_Settings;
			addrinfo::ConnectionSetting m_connectionSetting;
			uint32_t m_bufferLength;

			WSADATA m_wsaData;
			const char* m_defaultPort;
			SOCKET m_socket;
			struct sockaddr_in m_clientSettings;

			char* m_requestBuffer;
			socketType m_socketType;
			net::socket::status m_status;
		};

	} // !namespace socket
} // !namespace net

#endif // !_CLIENT_SOCKET_HPP_
