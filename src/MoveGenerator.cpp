//
// Created by Sagiv Marzini on 06/08/2026.
//

#include "MoveGenerator.h"

std::vector<Move> MoveGenerator::generateMoves(const Board& board, Board::Color color) {
	std::vector<Move> moves;
	const Bitboard empty = board.getUnoccupied();
	const Bitboard enemies = board.getColorPieces(Board::getEnemyColor(color));
	const Bitboard men = board.get(color, Board::MAN);

	switch (color) {
		case Board::WHITE:
			addQuietMovesInDirection(moves, men, Mask::RightUp, RightUpOffset, empty);
			addQuietMovesInDirection(moves, men, Mask::LeftUp, LeftUpOffset, empty);
			addCapturesInDirection(moves, men, Mask::RightUp, RightUpOffset, empty, enemies);
			addCapturesInDirection(moves, men, Mask::LeftUp, LeftUpOffset, empty, enemies);
			break;
		case Board::BLACK:
			addQuietMovesInDirection(moves, men, Mask::RightDown, RightDownOffset, empty);
			addQuietMovesInDirection(moves, men, Mask::LeftDown, LeftDownOffset, empty);
			addCapturesInDirection(moves, men, Mask::RightDown, RightDownOffset, empty, enemies);
			addCapturesInDirection(moves, men, Mask::LeftDown, LeftDownOffset, empty, enemies);
			break;
	}

	return moves;
}

Move MoveGenerator::createMove(int src, int dest, MoveFlag flag) {
	return (flag << 12) | (dest << 6) | src;
}

void MoveGenerator::addLandingsAsMoves(std::vector<unsigned short>& moves, Bitboard landings, int offset,
                                       MoveFlag flag) {
	while (landings) {
		int dest = std::countr_zero(landings);
		int src = dest - offset;
		moves.push_back(createMove(src, dest, flag));
		landings &= landings - 1;
	}
}

void MoveGenerator::addQuietMovesInDirection(std::vector<unsigned short>& moves, const Bitboard& sources,
                                             const Mask& directionMask,
                                             const int offset, const Bitboard& empty) {
	Bitboard landings = shift(sources & directionMask, offset) & empty;

	addLandingsAsMoves(moves, landings, offset, QUIET);
}

void MoveGenerator::addCapturesInDirection(std::vector<unsigned short>& moves, const Bitboard& sources,
                                           const Mask& directionMask, const int offset, const Bitboard& empty,
                                           const Bitboard& enemies) {
	// Check for enemies in the first jump, and nothing at the second jump
	Bitboard landings = shift(sources & directionMask, offset) & enemies;
	landings = shift(landings & directionMask, offset) & empty;

	addLandingsAsMoves(moves, landings, offset * 2, CAPTURE);
}

Bitboard MoveGenerator::shift(const Bitboard& b, const int offset) {
	return offset >= 0 ? (b << offset) : (b >> -offset);
}
