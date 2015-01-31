#ifndef BOARD_HXX_INCLUDED
#define BOARD_HXX_INCLUDED

#include "Piece.hxx"
#include "Position.hxx"

#include <array>

struct IndividualCastlingRights
{
	bool short_, long_;
};

using CastlingRights = std::array<IndividualCastlingRights, 2>;

inline void set( std::array<IndividualCastlingRights, 2>& arr,
          IndividualCastlingRights w,
          IndividualCastlingRights b )
{
	arr[white] = w;
	arr[black] = b;
}

class Board
{
	Piece mArray[120];

	CastlingRights mCastlingRights;

	OneDimPos mEnPassantPos /*: 8*/;

	Color toMove /*: 1*/;

	char const* parseCastlingRights(char const*);
	char const* parseMoveColor     (char const*);
	char const* parseEnPassantField(char const*);

public:

	CastlingRights castlingRights() const {return mCastlingRights;}

	Color colorToMove() const {return toMove;}
	OneDimPos enPassant() const {return mEnPassantPos;}

	Piece operator()( OneDimPos p ) const
	{
		return mArray[p];
	}

	Board( char const* FEN ) {initializeFromFEN(FEN);}

	void initializeFromFEN(char const*);
};

void print( std::ostream&, Board const& );

#endif // BOARD_HXX_INCLUDED
