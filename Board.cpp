#include "Basic.hxx"
#include "Board.hxx"

#include <iostream>

char const* Board::parseCastlingRights(char const* FEN)
{
      set(mCastlingRights, {}, {} ); // zero-fill the castling rights
	for (;;)
	{
		char c = *FEN++;
		if (c == ' ') break;

		     if (c == 'K') mCastlingRights[white].short_ = true;
		else if (c == 'Q') mCastlingRights[white].long_  = true;
		else if (c == 'k') mCastlingRights[black].short_ = true;
		else if (c == 'q') mCastlingRights[black].long_  = true;

		else
			throw std::invalid_argument("Invalid castling right!");
	}

	return FEN;
}

char const* Board::parseMoveColor     (char const* FEN)
{
	switch (*FEN++)
	{
	case 'w':
		toMove = white;
		break;
	case 'b':
		toMove = black;
		break;
	default:
		throw std::invalid_argument("Invalid color for first move!");
	}

	return FEN;
}

char const* Board::parseEnPassantField(char const* FEN)
{
	char f = *FEN++;

	if (f == '-')
	{
		mEnPassantPos = 0;
		return FEN;
	}

	if (f)
	if (char r = *FEN++)
	{
		mEnPassantPos = to120(fromPosChars(f,r));
		return FEN;
	}

	throw std::invalid_argument("Invalid en-passant field!");
}

void Board::initializeFromFEN( char const* FEN )
{
	for (OneDimPos pos = to120(0, 7);; pos -= 10)
	{
		//std::cout << from120(pos) << ' ' << FEN << '\n';
		for (auto inner_p = pos; inner_p != pos + 8; ++inner_p)
		{
			char const c = *FEN++;
			if (std::isdigit(c))
			{
				for (int i = 0; i != c - '0'; ++i)
					mArray[inner_p++] = Piece(nothing);

				if (inner_p > pos + 8)
					throw std::invalid_argument("Invalid digit in FEN!");

				--inner_p;
			}
			else
				mArray[inner_p] = pieceFromChar(c);
		}

		if (pos == to120(0, 0))
			break;

		//std::cout << from120(pos) << ' ' << FEN << '\n';
		if (*FEN++ != '/')
			throw std::invalid_argument("Missing forward slash in FEN!");
	}

	FEN = parseMoveColor(skipSpaces(FEN));

	FEN = parseCastlingRights(skipSpaces(FEN));

	FEN = parseEnPassantField(skipSpaces(FEN));

}

void print( std::ostream& os , Board const& b )
{
	auto horiz_line = [&]
	{
		os << ' ';
		for (unsigned i = 0; i != 8; ++i)
			os << "   -";
		os << '\n';
	};

	horiz_line();

	unsigned rank = 8;
	for (auto pos = to120(0, 7); pos != to120(0, -1); pos -= 18)
	{
		os << rank-- << ' ';
		for (auto end = pos+8; pos != end; ++pos)
			os << "| " << toChar(b(pos)) << ' ';
		os << "|\n";
		horiz_line();
	}
	os << ' ';
	for (char c = 'a'; c <= 'h'; ++c)
		os << "   " << c;
	os << '\n';


	os << (b.colorToMove() == black? "Black" : "White") << " to move, ";
	if (auto pos = b.enPassant())
		os << std::showbase << from120(pos) << " en passant";
	else
		os << "no en passant field";
}
