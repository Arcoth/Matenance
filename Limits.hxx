#ifndef LIMITS_HXX_INCLUDED
#define LIMITS_HXX_INCLUDED

#include "Piece.hxx"

namespace PieceTraits
{
	constexpr OneDimPos maxMoves[]
	{
		0, 0, // none, sentinel
		4,
		8,
		13,
		14,
		27,
		8
	};

	const OneDimPos maxMove = maxMoves[queen];

	#define DEFTABLE(name, ...) const bool name[] {0, 0, __VA_ARGS__};

	DEFTABLE( verticalMovers   , 1, 0, 0, 1, 1, 1 )
	DEFTABLE( verticalAttackers, 0, 0, 0, 1, 1, 1 )
	DEFTABLE( horizontalMovers     , 0, 0, 0, 1, 1, 1 )
	DEFTABLE( horizontalAttackers  , 0, 0, 0, 1, 1, 1 )

	auto& straightMovers = verticalMovers;
	auto& straightAttackers = verticalAttackers;

	DEFTABLE( diagonalFrontAttackers, 1, 0, 1, 0, 1, 1 )
	DEFTABLE( diagonalRearAttackers , 0, 0, 1, 0, 1, 1 )
	auto& diagonalAttackers = diagonalFrontAttackers;
	auto& diagonalMovers = diagonalAttackers;
}

#endif // LIMITS_HXX_INCLUDED
