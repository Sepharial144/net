#ifndef _NET_LOGGER_HPP_
#define _NET_LOGGER_HPP_

#include <cstring>
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>

// #include <fmt/std.h>

namespace logger {

namespace {
    template <size_t N>
    constexpr std::array<char, N * 3 + 1> makeBraces()
    {
        std::array<char, N * 3 + 1> line {};
        constexpr const char c[4] = "{} ";

        for (auto i = 4ul; i != line.size(); ++i)
            line[i] = c[i % 3];
        line.back() = '\0';

        return line;
    }
};

template <class... Args>
void fatal(const std::format_string<Args...> fmt, Args&&... args)
{
#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_FATAL_VALUE)
    std::cout
        << std::this_thread::get_id() << ": "
        << std::vformat(fmt.get(), std::make_format_args(args...)) << "\n";
#endif // LOGGING
}

template <class... Args>
void error(const std::format_string<Args...> fmt, Args&&... args)
{
#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_ERROR_VALUE)
    std::cout
        << std::this_thread::get_id() << ": "
        << std::vformat(fmt.get(), std::make_format_args(args...)) << "\n";
#endif // LOGGING
}

template <class... Args>
void warning(const std::format_string<Args...> fmt, Args&&... args)
{
#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_WARNING_VALUE)
    std::cout
        << std::this_thread::get_id() << ": "
        << std::vformat(fmt.get(), std::make_format_args(args...)) << "\n";
#endif // LOGGING
}

template <class... Args>
void info(const std::format_string<Args...> fmt, Args&&... args)
{
#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_INFO_VALUE)
    std::cout
        << std::this_thread::get_id() << ": "
        << std::vformat(fmt.get(), std::make_format_args(args...)) << "\n";
#endif // LOGGING
}

template <class... Args>
void debug(const std::format_string<Args...> fmt, Args&&... args)
{
#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_DEBUG_VALUE)
    std::cout
        << std::this_thread::get_id() << ": "
        << std::vformat(fmt.get(), std::make_format_args(args...)) << "\n";
#endif // LOGGING
}

template <class... Args>
void debug_low(const std::format_string<Args...> fmt, Args&&... args)
{
#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_DEBUG_LOW_VALUE)
    std::cout
        << std::this_thread::get_id() << ": "
        << std::vformat(fmt.get(), std::make_format_args(args...)) << "\n";
#endif // LOGGING
}

template <class... Args>
void debug_medium(const std::format_string<Args...> fmt, Args&&... args)
{
#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_DEBUG_MEDIUM_VALUE)
    std::cout
        << std::this_thread::get_id() << ": "
        << std::vformat(fmt.get(), std::make_format_args(args...)) << "\n";
#endif // LOGGING
}

template <class... Args>
void debug_high(const std::format_string<Args...> fmt, Args&&... args)
{
#if defined(LOGGING) && (LOGGING_LEVEL <= LOGGING_DEBUG_HIGH_VALUE)
    std::cout
        << std::this_thread::get_id() << ": "
        << std::vformat(fmt.get(), std::make_format_args(args...)) << "\n";
#endif // LOGGING
}

} // namespace logger

#endif // !_NET_LOGGER_HPP_