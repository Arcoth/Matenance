#ifndef PSEUDOLEGALMOVEGEN_HXX_INCLUDED
#define PSEUDOLEGALMOVEGEN_HXX_INCLUDED

#include "Limits.hxx"
#include "Basic.hxx"

#include <boost/container/static_vector.hpp>

#include <iostream>
#include <vector>

namespace MoveGen
{

	/// static_vector doesn't support list-initialization yet - only the development version.
	const /*boost::container::static_vector*/std::vector<OneDimPos> moveTable[]
	{
		{}, {},
		{},
		{ 19, 21, 8, 12, -19, -21, -8, -12 }, //knight
		{ 9, 11, -9, -11},{10, 1, -10, -1 }, // bishop, rook
		{ 9, 11, -9, -11 , 10, 1, -10, -1 }, // queen
		{ 9, 11, -9, -11 , 10, 1, -10, -1 }, // king
	};

namespace PseudoLegal
{
	template <typename T,
	          typename=decltype(*std::declval<T>()++)>
	auto writeTo(T it)
	{
		return [=] (auto&& v) mutable { *it++ = std::forward<decltype(v)>(v); };
	}

	template <typename Container,
	          typename=decltype(std::declval<Container>().emplace_back())>
	auto writeTo(Container& c)
	{
		return writeTo(std::back_inserter(c));
	}

	template <bool onlyTakes,
		    typename Range,
		    typename Func>
	void foreachLinearMove( Range&& moves,
	                        Board const& b,
	                        OneDimPos pos,
	                        Color col,
	                        Func func )
	{
		for (auto j : moves)
		{
			OneDimPos p = pos;
			for(;;)
			{
				p += j;

				if (b(p).type == sentinel)
					break;

				if (b(p).type != nothing)
				{
					if (b(p).color != col)
						func(p);
					break;
				}

				if (not onlyTakes)
					func(p);
			}
		}
	}

	template <bool onlyTakes,
		    typename Range,
		    typename OutputIterator>
	void generateLinearMoves( Range&& moves,
					  Board const& b,
					  OneDimPos pos,
					  Color col,
					  OutputIterator out )
	{
		foreachLinearMove<onlyTakes>(moves, b, pos, col, writeTo(out));
	}


	template <bool onlyTakes,
		    typename Range,
		    typename Func>
	void foreachJumpMove( Range&& moves,
	                      Board const& b,
	                      OneDimPos pos,
	                      Color col,
	                      Func func )
	{
		for (auto j : moves)
		{
			OneDimPos p = pos + j;

			if (b(p).type == sentinel || isFigureAndHasColor(b(p), col))
				continue;

			if (not onlyTakes)
				func(p);
		}
	}

	template <bool onlyTakes,
		    typename Range,
		    typename OutputIterator>
	void generateJumpMoves( Range&& moves,
					Board const& b,
					OneDimPos pos,
					Color col,
					OutputIterator out )
	{
		foreachJumpMove<onlyTakes>(moves, b, pos, col, writeTo(out));
	}

	#define DEFINE_LIN_MOV_GEN(attacks, fname, fname2, name )                 \
	\
	template <typename Func>                                                  \
	void foreach##fname##fname2                                               \
					   ( Board const& b,                              \
					     OneDimPos p,                                 \
					     Color c,                                     \
					     Func f )                                     \
	{  return foreachLinearMove<attacks>   (moveTable[name], b, p, c, f);  }  \
	\
	template <typename OutputIterator>                                        \
	void generate##fname##fname2##s                                           \
					   ( Board const& b,                              \
					     OneDimPos p,                                 \
					     Color c,                                     \
					     OutputIterator o )                           \
	{  return generateLinearMoves<attacks> (moveTable[name], b, p, c, o);  }

	#define DEFINE_JMP_MOV_GEN( attacks, fname, fname2, name )                \
	                                                                          \
	template <typename Func>                                                  \
	void foreach##fname##fname2                                               \
					   ( Board const& b,                              \
					     OneDimPos p,                                 \
					     Color c,                                     \
					     Func f )                                     \
	{  return generateJumpMoves <attacks>(moveTable[name], b, p, c, f);  }    \
	                                                                          \
	template <typename OutputIterator>                                        \
	void generate##fname##fname2##s                                           \
					   ( Board const& b,                              \
					     OneDimPos p,                                 \
					     Color c,                                     \
					     OutputIterator o )                           \
	{  return generateJumpMoves <attacks>(moveTable[name], b, p, c, o);  }

	DEFINE_LIN_MOV_GEN(0, Bishop, Move, bishop)
	DEFINE_LIN_MOV_GEN(0, Rook, Move, rook)

	DEFINE_JMP_MOV_GEN(0, Knight, Move, knight)
	DEFINE_JMP_MOV_GEN(0, King, Move, king)

	DEFINE_LIN_MOV_GEN(1, Bishop, Attack, bishop)
	DEFINE_LIN_MOV_GEN(1, Rook, Attack, rook)

	DEFINE_JMP_MOV_GEN(1, Knight, Attack, knight)
	DEFINE_JMP_MOV_GEN(1, King, Attack, king)

	template <typename Callable>
	void foreachQueenMove( Board const& b, OneDimPos p, Color c, Callable o )
	{
		foreachBishopMove(b, p, c, o);
		foreachRookMove  (b, p, c, o);
	}

	template <typename Callable>
	void foreachQueenAttacks( Board const& b, OneDimPos p, Color c, Callable o )
	{
		foreachBishopAttack(b, p, c, o);
		foreachRookAttack  (b, p, c, o);
	}

	namespace detail
	{
		inline int pathSign(Color c)
		{
			return 1 - 2*c; // 1 for white(0), -1 for black (1)
		}

		template <typename Callable>
		void foreachPawnAttack( Board const& b, OneDimPos p, Color c, Callable callable, int path_sign )
		{
			int path1 = path_sign * 11, path2 = path_sign * 9;

			for (pos : {p + path1, p + path2})
				if (isFigureAndHasColor(b(pos), invert(c))  // If there is a fiendish piece on the take-square
				 || b.enPassant() == pos)                   // ... or the take square is the en-passant square
					callable(pos);
		}
	}

	template <typename Callable>
	void foreachPawnAttack( Board const& b, OneDimPos p, Color c, Callable callable )
	{
		return detail::foreachPawnAttack(b, p, c, callable, detail::pathSign(c));
	}

	template <typename Callable>
	void foreachPawnMove( Board const& b, OneDimPos p, Color c, Callable callback )
	{
		auto sign = detail::pathSign(c);
		detail::foreachPawnAttack(b, p, c, callback, sign);

		auto forward_one = sign * 10;

		if (b(p+forward_one).type == nothing)
		{
			callback(p+forward_one);

			auto forward_two = 2*forward_one;

			if (rank_of(p) == 1+5*c && b(p+forward_two).type == nothing)
				callback(p+forward_two);
		}
	}

	template <typename Callable>
	void foreachKingsCastlingMove( Board const& b, Color c, Callable callback )
	{
		bool longcastle  = b.castlingRights()[c].long_,
		     shortcastle = b.castlingRights()[c].short_;

		OneDimPos king       = to120(fromPosChars('e', '1')) + 70*c,
			    long_rook  = to120(fromPosChars('a', '1')) + 70*c,
			    short_rook = to120(fromPosChars('h', '1')) + 70*c;

		// Go through the first field_count fields next to the rook and check whether any piece is there
		auto add_if_no_obstacle = [&] (int field_count, int rook_pos, int direction)
		{
			for (int pos = 1; pos != field_count; ++pos)
				if (b(rook_pos+direction*pos).type != nothing)
					return;

			callback(king + direction*2);
		};

		if (longcastle)
			add_if_no_obstacle(3, long_rook, -1);
		if (shortcastle)
			add_if_no_obstacle(2, short_rook, 1);
	}

	/*template <typename Callable>
	void foreachStraightAttackerOn( Board const& b, OneDimPos p, Color c, Callable out )
	{
		boost::container::static_vector<OneDimPos, PieceTraits::maxMoves[rook]> attackers;

		generateRookAttacks(b, p, invert(c), std::back_inserter(attackers));

		for (auto a : attackers)
			if (PieceTraits::straightMovers[b(a).type])
	}*/

}


}

#endif // PSEUDOLEGALMOVEGEN_HXX_INCLUDED
