//
// Created by Sagiv Marzini on 06/08/2026.
//

#include "MoveGenerator.h"

std::vector<Move> MoveGenerator::generateMoves(const Board& board, Board::Color color) {
	std::vector<Move> moves;
	const Bitboard empty = board.getUnoccupied();
	const Bitboard enemies = board.getColorPieces(Board::getEnemyColor(color));
	const Bitboard men = board.get(color, Board::Man);
	const Bitboard kings = board.get(color, Board::King);

	switch (color) {
		case Board::White:
			addQuietMovesInDirection(moves, men, Board::Mask::MaskRightUp, Board::RightUpOffset, empty);
			addQuietMovesInDirection(moves, men, Board::Mask::MaskLeftUp, Board::LeftUpOffset, empty);
			addCapturesInDirection(moves, men, Board::Mask::MaskRightUp, Board::RightUpOffset, empty, enemies);
			addCapturesInDirection(moves, men, Board::Mask::MaskLeftUp, Board::LeftUpOffset, empty, enemies);
			break;
		case Board::Black:
			addQuietMovesInDirection(moves, men, Board::Mask::MaskRightDown, Board::RightDownOffset, empty);
			addQuietMovesInDirection(moves, men, Board::Mask::MaskLeftDown, Board::LeftDownOffset, empty);
			addCapturesInDirection(moves, men, Board::Mask::MaskRightDown, Board::RightDownOffset, empty, enemies);
			addCapturesInDirection(moves, men, Board::Mask::MaskLeftDown, Board::LeftDownOffset, empty, enemies);
			break;
	}

	addKingQuietMoves(moves, kings, empty);
	addKingCaptures(moves, kings, empty, enemies);

	return moves;
}

Move MoveGenerator::createMove(int src, int dest, MoveFlags flags) {
	return (flags << 12) | (dest << 6) | src;
}

void MoveGenerator::addLandingsAsMoves(std::vector<Move>& moves, Bitboard landings, int offset,
                                       MoveFlags flags) {
	while (landings) {
		int dest = std::countr_zero(landings);
		int src = dest - offset;

		moves.push_back(createMove(src, dest, flags));
		landings &= landings - 1;
	}
}

void MoveGenerator::addQuietMovesInDirection(std::vector<Move>& moves, Bitboard sources,
                                             Board::Mask directionMask,
                                             int offset, Bitboard empty) {
	Bitboard landings = shift(sources & directionMask, offset) & empty;

	addLandingsAsMoves(moves, landings, offset, None);
}

void MoveGenerator::addCapturesInDirection(std::vector<Move>& moves, Bitboard sources,
                                           Board::Mask directionMask, int offset, Bitboard empty,
                                           Bitboard enemies) {
	// Check for enemies in the first jump, and nothing at the second jump
	Bitboard landings = shift(sources & directionMask, offset) & enemies;
	landings = shift(landings & directionMask, offset) & empty;

	addLandingsAsMoves(moves, landings, offset * 2, Capture);
}

void MoveGenerator::addKingQuietMoves(std::vector<Move>& moves, Bitboard kings, Bitboard empty) {
	addKingSlideMovesInDirection(moves, kings, empty, Board::MaskRightUp, Board::RightUpOffset);
	addKingSlideMovesInDirection(moves, kings, empty, Board::MaskRightDown, Board::RightDownOffset);
	addKingSlideMovesInDirection(moves, kings, empty, Board::MaskLeftUp, Board::LeftUpOffset);
	addKingSlideMovesInDirection(moves, kings, empty, Board::MaskLeftDown, Board::LeftDownOffset);
}

void MoveGenerator::addKingCaptures(std::vector<Move>& moves, Bitboard kings, Bitboard empty,
                                    Bitboard enemies) {
	addKingCapturesInDirection(moves, kings, empty, enemies, Board::MaskRightUp, Board::RightUpOffset);
	addKingCapturesInDirection(moves, kings, empty, enemies, Board::MaskRightDown, Board::RightDownOffset);
	addKingCapturesInDirection(moves, kings, empty, enemies, Board::MaskLeftUp, Board::LeftUpOffset);
	addKingCapturesInDirection(moves, kings, empty, enemies, Board::MaskLeftDown, Board::LeftDownOffset);
}

void MoveGenerator::addKingSlideMovesInDirection(std::vector<Move>& moves, Bitboard kings, Bitboard empty,
                                                 Board::Mask directionMask, int offset) {
	kings &= directionMask;

	for (uint8_t step = 1; kings; ++step) {
		Bitboard landings = shift(kings, offset) & empty;
		addLandingsAsMoves(moves, landings, offset * step, None);
		kings = landings & directionMask;
	}
}

void MoveGenerator::addKingCapturesInDirection(std::vector<Move>& moves, Bitboard kings, Bitboard empty,
                                               Bitboard enemies, Board::Mask directionMask, int offset) {
	kings &= directionMask;

	for (uint8_t step = 1; kings; ++step) {
		Bitboard captures = shift(kings, offset) & enemies;
		captures = shift(captures & directionMask, offset) & empty;
		addLandingsAsMoves(moves, captures, offset * step + offset, Capture);

		// At the end, walk one to check for another capture
		kings = shift(kings & directionMask, offset) & empty & directionMask;
	}
}

Bitboard MoveGenerator::shift(const Bitboard& b, const int offset) {
	return offset >= 0 ? (b << offset) : (b >> -offset);
}
