//
// Created by Sagiv Marzini on 06/08/2026.
//

#include "MoveGenerator.h"

std::vector<Move> MoveGenerator::generateMoves(const Board& board, Board::Color color) {
	std::vector<Move> moves;
	const Bitboard empty = board.getUnoccupied();
	const Bitboard men = board.get(color, Board::MAN);

	switch (color) {
		case Board::WHITE:
			addQuietMovesInDirection(moves, men, Mask::RightUp, RightUpOffset, empty);
			addQuietMovesInDirection(moves, men, Mask::LeftUp, LeftUpOffset, empty);
			break;
		case Board::BLACK:
			addQuietMovesInDirection(moves, men, Mask::RightDown, RightDownOffset, empty);
			addQuietMovesInDirection(moves, men, Mask::LeftDown, LeftDownOffset, empty);
			break;
	}

	return moves;
}

Move MoveGenerator::createMove(int src, int dest, MoveFlag flag) {
	return (flag << 12) | (dest << 6) | src;
}

void MoveGenerator::addQuietMovesInDirection(std::vector<unsigned short>& moves, const Bitboard& sources,
                                             const Mask& directionMask,
                                             const DiagonalMove& offset, const Bitboard& empty) {
	Bitboard landings = shift(sources & directionMask, offset) & empty;

	while (landings) {
		int dest = std::countr_zero(landings);
		int src = dest - offset;
		moves.push_back(createMove(src, dest, QUIET));
		landings &= landings - 1;
	}
}

Bitboard MoveGenerator::shift(const Bitboard& b, const DiagonalMove& offset) {
	return offset >= 0 ? (b << offset) : (b >> -offset);
}
