#include "net_server.hpp"
#include "net_client.hpp"
#include "common.hpp"

namespace net
{
	client::client(size_t len_message)
		:m_socket{ -1 }
		, m_lenMessage{ len_message }
	{
	}

	client::~client()
	{}

	void client::close()
	{
		if (m_socket > 0)
			::close(m_socket);
	}

	int32_t client::send(const char* data, size_t len)
	{
		return ::send(m_socket, data, len, 0);
	}

	size_t client::sendFrame(const char* data, size_t len)
	{
		size_t count = len / m_lenMessage;
		size_t part = len - count * m_lenMessage;
		size_t result = 0ul;
		for (size_t i = 0ul; i <= count; ++i)
		{
			size_t status = ::send(m_socket, data, m_lenMessage, 0);
			if (status == m_lenMessage)
			{
				result += status;
				data += m_lenMessage;
			}
			else
			{
				return 0ul;
			}
		}
		if (part != 0ul)
			result += ::send(m_socket, data, part, 0);

		return (result == count * m_lenMessage) ? result : 0ul;
	}

	int32_t client::read(char* data, size_t len)
	{
		int32_t status = ::recv(m_socket, data, len, 0);
		size_t rank = common::getRank(1024);
		if (status > 0)
			return common::findLastReadableLetter(data, len, rank);
		return 0;
	}

} // ! namespace net