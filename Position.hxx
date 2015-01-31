#ifndef POSITION_HXX_INCLUDED
#define POSITION_HXX_INCLUDED

#include <ostream>

using OneDimPos = int;

struct Position
{
	int x : 8;
	int y : 8;
};

inline std::ostream& operator<<(std::ostream& os, Position p)
{
	if (os.flags() & std::ios::showbase)
		return os << char(p.x + 'a') << char(p.y + '1');

	return os << '(' << p.x << ',' << p.y << ')';
}

inline int file_of(OneDimPos p)
{
	return p%10 - 1;
}

inline int rank_of(OneDimPos p)
{
	return p/10 - 2;
}

inline Position from120(OneDimPos p)
{
	return {file_of(p), rank_of(p)};
}

inline OneDimPos to120(int x, int y)
{
	return x+1 + (y+2)*10;
}

inline OneDimPos to120(Position p)
{
	return to120(p.x, p.y);
}

inline Position fromPosChars( char file, char rank )
{
	return {file - 'a', rank - '1'};
}



#endif // POSITION_HXX_INCLUDED
