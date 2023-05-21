#include <gtest/gtest.h>
#include <cstring>

#include "net.hpp"
#include "exceptions/net_exception.hpp"
#include "net_api.hpp"


namespace net
{
	TEST(NetApiTest, TranslatePort) {
		EXPECT_EQ(api::translatePort<int32_t>("3000"), 3000);
		EXPECT_EQ(api::translatePort<int32_t>("65535"), 65535);
	}

	TEST(NetApiTest, TranslateAddress) {
		EXPECT_EQ(std::strcmp(api::translateAddress("localhost"), "127.0.0.1"), 0);
		EXPECT_EQ(std::strcmp(api::translateAddress("address"), "address"), 0);
	}

	TEST(NetExceptionTest, ThrowNetException) {
		ASSERT_THROW({
			throw net::exception("Net simple expception");
		}, net::exception);

		ASSERT_THROW({
			bool isError = true;
			net::throw_exception_on(isError, "Condition throwable exception");
		}, net::exception);

		ASSERT_NO_THROW({
			int32_t ret = 1;
			net::throw_exception_on_poll(ret, "Poll call returned positive");
		});

		ASSERT_THROW({
			int32_t ret = 0;
			net::throw_exception_on_poll(ret, "Poll call returned zero");
		}, net::exception);

		ASSERT_THROW({
			int32_t ret = net::pollc::poll_error;
			net::throw_exception_on_poll(ret, "Poll call returned poll error");
		}, net::exception);

		ASSERT_NO_THROW({
			int32_t ret = 1;
			net::throw_exception_on_revents(ret, net::pollc::in, "Revents return positive");
		});

		ASSERT_THROW({
			int32_t ret = 0;
			net::throw_exception_on_revents(ret, pollc::in, "Test");
		}, net::exception);

		ASSERT_THROW({
			int32_t ret = -100;
			net::throw_exception_on_revents(ret, pollc::in, "Test");
		}, net::exception);
	}

	TEST(NetStandartExceptionTest, CatchNetException) {
		const std::string expected = "Net simple expception code: 0";
		try{ 
			throw net::exception("Net simple expception"); 
		}
		catch (const net::exception& e) {
			EXPECT_EQ(std::string{ e.what() }, expected);
		}
	}

	TEST(NetStandartExceptionTest, CatchStandartException) {
		const std::string expected = "Cpp standart expception code: 0";
		try {
			throw net::exception("Cpp standart expception");
		}
		catch (const std::exception& e) {
			EXPECT_EQ(std::string{ e.what() }, expected);
		}
	}

	TEST(NetStandartExceptionTest, CatchExceptionOn) {
		const std::string expected = "Exception wrapper code: 0";
		try {
			bool isError = true;
			net::throw_exception_on(isError, "Exception wrapper");
		}
		catch (const std::exception& e) {
			EXPECT_EQ(std::string{ e.what() }, expected);
		}
	}

	TEST(NetStandartExceptionTest, CatchExceptionOnPollZero) {
		const std::string expected = "Test unexpected poll error, code: 0";
		try {
			int32_t ret = 0;
			net::throw_exception_on_poll(ret, "Test");
		}
		catch (const std::exception& e) {
			EXPECT_EQ(std::string{ e.what() }, expected);
		}
	}

	TEST(NetStandartExceptionTest, CatchExceptionOnPollError) {
		const std::string expected = "Test call poll failed, code: 0";
		try {
			int32_t ret = net::pollc::poll_error;
			net::throw_exception_on_poll(ret, "Test");
		}
		catch (const std::exception& e) {
			EXPECT_EQ(std::string{ e.what() }, expected);
		}
	}
} // namespace net
