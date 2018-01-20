#include "csv.hpp"
#include <cstdio>
#include <cstring>

std::string format(const char * Message, ...)
{
	assert(Message);
	
	char Text[1024];
	std::memset(Text, 0, sizeof(Text));

	va_list ap;
	va_start(ap, Message);
		std::vsprintf(Text, Message, ap);
	va_end(ap);

	return Text;
}

void csv::log(char const* String, double Convergent, double Min, double Max)
{
	this->Data.push_back(data(String, Convergent, Min, Max));
}

void csv::save(char const* Filename)
{
	FILE* File(fopen(Filename, "a+"));
	assert(File);
	fprintf(File, "%s;%s;%s;%s\n", "Tests", "average", "max", "min");

	for(std::size_t i = 0; i < this->Data.size(); ++i)
	{
		fprintf(File, "%s;%d;%d;%d\n",
			Data[i].String.c_str(),
			static_cast<int>(Data[i].Convergent),
			static_cast<int>(Data[i].Max), static_cast<int>(Data[i].Min));
	}
	fclose(File);
}

void csv::print()
{
	fprintf(stdout, "\n");
	for(std::size_t i = 0; i < this->Data.size(); ++i)
	{
		fprintf(stdout, "%s, %2.5f, %2.5f, %2.5f\n",
			Data[i].String.c_str(),
			Data[i].Convergent / 1000.0,
			Data[i].Min / 1000.0, Data[i].Max / 1000.0);
	}
}

