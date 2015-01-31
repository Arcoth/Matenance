#include <stdexcept>

#define AUTO_RETURN(...) -> decltype(__VA_ARGS__) {return (__VA_ARGS__);}

inline char const* skipSpaces(char const* s)
{
	for(;;)
	{
		if (*s == 0)
			throw std::runtime_error("Unexpected end!");
		if (*s != ' ')
			return s;
		++s;
	}

	__builtin_unreachable();
}
