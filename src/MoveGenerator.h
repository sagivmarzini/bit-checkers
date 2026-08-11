//
// Created by Sagiv Marzini on 06/08/2026.
//

#ifndef BIT_CHECKERS_MOVEGENERATOR_H
#define BIT_CHECKERS_MOVEGENERATOR_H
#include <vector>

#include "Board.h"


class MoveGenerator {
public:
	static std::vector<Move> generateMoves(const Board& board, Board::Color color);

private:
	static void addLandingsAsMoves(std::vector<unsigned short>& moves, Bitboard landings, int offset, MoveFlags flags);

	static void addQuietMovesInDirection(std::vector<unsigned short>& moves, Bitboard sources,
	                                     Board::Mask directionMask,
	                                     int offset, Bitboard empty);


	static void addCapturesInDirection(std::vector<unsigned short>& moves, Bitboard sources, Board::Mask directionMask,
	                                   int offset, Bitboard empty, Bitboard enemies);

	static void addKingQuietMoves(std::vector<Move>& moves, Bitboard kings, Bitboard empty);

	static void addKingCaptures(std::vector<unsigned short>& moves, Bitboard kings, Bitboard empty, Bitboard enemies);

	static void addKingSlideMovesInDirection(std::vector<Move>& moves, Bitboard kings, Bitboard empty,
	                                         Board::Mask directionMask,
	                                         int offset);

	static void addKingCapturesInDirection(std::vector<Move>& moves, Bitboard kings, Bitboard empty, Bitboard enemies,
	                                       Board::Mask directionMask,
	                                       int offset);


	static Bitboard shift(const Bitboard& b, int offset);
};


#endif //BIT_CHECKERS_MOVEGENERATOR_H
