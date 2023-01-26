#ifndef _LISTENING_SOCKET_HPP_
#define _LISTENING_SOCKET_HPP_

#include "socket_context.hpp"
#include "win32_definitions.hpp"

/*
namespace net
{
	namespace socket
	{
		class ListeningSocket
		{
		public:
			explicit ListeningSocket(addrinfo::SockSetting& settings, uint32_t buffer_length, const char* default_port);
			~ListeningSocket();

			status waitConnection(SOCKET& connection);
			status shutdown(shutdownMode mode);
			status status();
			net::socket::status close();
			void error(const char* title, const char* message, int32_t value) const;

		private:
			net::socket::status init();

		private:
			addrinfo::SockSetting m_Settings;
			int32_t iSendResult;
			uint32_t m_BufferLength;
			char* m_requestBuffer;

			WSADATA m_wsaData;
			int32_t m_initResult;
			const char* m_defaultPort;
			SOCKET m_socket;

			struct addrinfo* m_ptrInfo;
			struct addrinfo m_Hints;

			socketType m_socketType = socketType::SERVER;
			net::socket::status m_status = status::Initializing;
		};
	} // !namespace socket
} // !namescape net

*/
#endif //! _LISTENING_SOCKET_HPP_
