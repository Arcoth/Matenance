#ifndef PIECE_HXX_INCLUDED
#define PIECE_HXX_INCLUDED

#include <ostream>
#include <istream>

// -- PieceType ---------------------------

enum PieceType
{
	nothing,
	sentinel,
	pawn,
	knight,
	bishop,
	rook,
	queen,
	king
};

inline bool isFigure( PieceType p )
{
	return p != nothing && p != sentinel;
}

// -- Color ---------------------------

enum Color
{
	white,
	black
};

inline Color invert(Color c)
{
	return Color(!c);
}

// -- Piece ---------------------------

struct Piece
{
	PieceType type /*: 3*/;
	Color color /*: 1*/;

	Piece(PieceType type = sentinel, Color color = black) :
		type(type),
		color(color) {}
};

inline bool isFigureAndHasColor( Piece const& p, Color c )
{
	return isFigure(p.type) && p.color == c;
}

// -- I/O ---------------------------

Piece pieceFromChar( char );
char toChar( Piece );

inline std::ostream& operator<<( std::ostream& os, Piece p )
{
	return os << toChar(p);
}

inline std::istream& operator>>( std::istream& is, Piece& p )
{
	char c;
	if (is.get(c))
		p = pieceFromChar(c);
	return is;
}

#endif // PIECE_HXX_INCLUDED
