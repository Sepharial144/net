#include <gtest/gtest.h>

class NetApiTest: public ::testing::Test
{
	public:
		NetApiTest() {};
		~NetApiTest() {};
};

TEST_F(NetApiTest, testApi) {
	SUCCEED();
}