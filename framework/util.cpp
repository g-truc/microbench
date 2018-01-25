#include <cstdarg>
#include <cstdio>
#include "util.hpp"

std::string message_format(const char* Message, ...)
{
	std::size_t const STRING_BUFFER(4096);
	char Buffer[STRING_BUFFER];
	va_list List;

	if(Message == nullptr)
		return std::string();

	va_start(List, Message);
	vsprintf(Buffer, Message, List);
	va_end(List);

	return Buffer;
}
