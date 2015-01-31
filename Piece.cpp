#include "Piece.hxx"

#include <algorithm>

char const pieceChars[] = " XPNBRQK xpnbrqk";

Piece pieceFromChar( char c )
{
	auto pos = std::find(std::begin(pieceChars), std::end(pieceChars), c) - std::begin(pieceChars);

	return {PieceType(pos%8), Color(pos/8)};
}

char toChar( Piece p )
{
	return pieceChars[p.type + p.color*8];
}
