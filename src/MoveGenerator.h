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

	// Forms a 16-bit move representation <flag (4 bits)><dest (6 bits)><src (6 bits)>
	static Move createMove(int src, int dest, MoveFlag flag);

private:
	enum Mask : uint64_t {
		RightUp = 0x7F7F7F7F7F7F7F, // Zeros the right and top columns
		LeftUp = 0xFEFEFEFEFEFEFE,  // left and top
		RightDown = 0x7F7F7F7F7F7F7F00,
		LeftDown = 0xFEFEFEFEFEFEFE00
	};

	enum DiagonalMove {
		RightUpOffset = 9,
		LeftUpOffset = 7,
		RightDownOffset = -7,
		LeftDownOffset = -9
	};

	static void addLandingsAsMoves(std::vector<unsigned short>& moves, Bitboard landings, int offset, MoveFlag flag);

	static void addQuietMovesInDirection(std::vector<unsigned short>& moves, const Bitboard& sources,
	                                     const Mask& directionMask,
	                                     int offset, const Bitboard& empty);


	static void addCapturesInDirection(std::vector<unsigned short>& moves, const Bitboard& sources,
	                                   const Mask& directionMask,
	                                   int offset, const Bitboard& empty, const Bitboard& enemies);

	static Bitboard shift(const Bitboard& b, int offset);
};


#endif //BIT_CHECKERS_MOVEGENERATOR_H
