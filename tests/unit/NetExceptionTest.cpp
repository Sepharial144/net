#include "exceptions/net_exception.hpp"

#include <gtest/gtest.h>
#include <cstring>


TEST(NetExceptionTest, ThrowNetException) {
    EXPECT_THROW({
        throw net::exception("Net simple expception");
    })
    EXPECT_THROW({
        bool isError = true;
        net::throw_exception_on(isError, "Condition throwable exception");
    })
}

TEST(NetStandartExceptionTest, CatchNetException) {
    const char* nameException = "Net simple expception";
    try
    {
        throw net::exception(nameException);
    }
    catch (const net::exception& e)
    {
        EXPECT_EQ(std::strcmp(e.what(), nameException), 0);
    }
}

TEST(NetStandartExceptionTest, CatchStandartException) {
    const char* nameException = "Cpp standart expception";
    try
    {
        throw net::exception(nameException);
    }
    catch (const std::exception& e)
    {
        EXPECT_EQ(std::strcmp(e.what(), nameException), 0);
    }
}