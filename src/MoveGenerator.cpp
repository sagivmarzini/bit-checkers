//
// Created by Sagiv Marzini on 06/08/2026.
//

#include "MoveGenerator.h"

std::vector<Move> MoveGenerator::generateMoves(const Board& board, Board::Color color) {
	std::vector<Move> moves;
	const Bitboard empty = board.getUnoccupied();

	auto& blackMen = board.get(color, Board::MAN);
	// Apply RightUp mask since right and top columns can't go diagonally right-up
	// Move 9 bits left (1 row up + 1 column right) and AND (&) with the empty cells to find landings
	Bitboard rightUpLandings = ((blackMen & Mask::RightUp) << RightUpOffset) & empty;
	Bitboard leftUpLandings = ((blackMen & Mask::LeftUp) << LeftUpOffset) & empty;

	while (rightUpLandings) {
		int dest = std::countr_zero(rightUpLandings);
		int src = dest - RightUpOffset;
		moves.push_back(createMove(src, dest, QUIET));
		rightUpLandings &= rightUpLandings - 1;
	}
	while (leftUpLandings) {
		int dest = std::countr_zero(leftUpLandings);
		int src = dest - LeftUpOffset;
		moves.push_back(createMove(src, dest, QUIET));
		leftUpLandings &= leftUpLandings - 1;
	}

	return moves;
}

Move MoveGenerator::createMove(int src, int dest, MoveFlag flag) {
	return (flag << 12) | (dest << 6) | src;
}
