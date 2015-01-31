#include "Board.hxx"
#include "PseudoLegalMoveGen.hxx"

#include <iostream>
#include <iterator>


int main()
{
	using namespace std;

	//Board b("rnbqkbnr/pppppppp/3R4/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	Board b("8/8/8/8/8/8/8/4K2R w K - 0 1");

	print(std::cout, b);

	using namespace MoveGen::PseudoLegal;

	foreachKingsCastlingMove( b, white, [] (auto p) {cout << from120(p) << endl;} );

/*


	std::vector<OneDimPos> vec;
	generateRookMoves(b, to120(fromPosChars('d', '6')), white, std::back_inserter(vec));

	std::cout << "\nRook: ";
	for (p : vec)
		std::cout << std::showbase << from120(p) << ", ";

	vec.clear();
	foreachPawnMove(b, to120(fromPosChars('e', '7')), black, writeTo(vec));

	std::cout << "\nPawn: ";
	for (p : vec)
		std::cout << std::showbase << from120(p) << ", ";*/
}
