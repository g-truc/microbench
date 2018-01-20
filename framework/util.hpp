#pragma once

#include <string>

std::string message_format(const char* Message, ...);

template <typename T, std::size_t N>
constexpr std::size_t countof(T const (&)[N]) noexcept
{
	return N;
}

template <class C>
std::size_t countof(C const & c)
{
	return c.size();
}
