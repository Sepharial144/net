#include "win32_common_api.hpp"
#include "exceptions/SocketException.hpp"

namespace net
{
	namespace api
	{
		void initializeWSA()
		{
			std::cout << "Netlib api: initialization WSA ..." << &std::endl;
			if (WSA_CALL_COUNT == 0ul)
			{
				std::cout << "Netlib api: call WSA ..." << &std::endl;
				int32_t ret = ::WSAStartup(MAKEWORD(2, 2), &WSA_DATA);
				net::throw_exception_on(ret != 0, "Netlib: initialization WSA failed");
				/*
				if (int32_t ret = ::WSAStartup(MAKEWORD(2, 2), &WSA_DATA) != 0) {
					throw net::exception("Netlib: initialization WSA failed", ret);
				}
				*/
				std::cout << "Netlib api: call WSA ... complete" << &std::endl;
			}
			++WSA_CALL_COUNT;
			std::cout << "Netlib api: WSA_CALL_COUNT ... " << WSA_CALL_COUNT << &std::endl;
			std::cout << "Netlib api: initialization WSA ... complete" << &std::endl;
		}


		void uninitializeWSA()
		{
			std::cout << "Netlib api: uninitialization WSA ..." << &std::endl;
			--WSA_CALL_COUNT;
			if (WSA_CALL_COUNT == 0ul)
			{
				int32_t ret = ::WSACleanup();
				net::throw_exception_on(ret != 0, "Netlib: uninitialization WSA failed");
				/*
				if (int32_t ret = ::WSACleanup() != 0) {
					throw net::exception("Netlib: uninitialization WSA failed", ret);
				}
				*/
				std::cout << "Netlib api: call uninit WSA ... complete" << &std::endl;
			}
			std::cout << "Netlib api: uninitialization WSA ... complete" << &std::endl;

		}
	} // namespace api
} // namespace net