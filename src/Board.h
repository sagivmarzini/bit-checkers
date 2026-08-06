//
// Created by Sagiv Marzini on 06/08/2026.
//

#ifndef BIT_CHECKERS_BOARD_H
#define BIT_CHECKERS_BOARD_H
#include <cstdint>
#include <iostream>


class Board {
public:
	Board();

	friend std::ostream& operator<<(std::ostream& os, const Board& board);

	enum Color {
		WHITE,
		BLACK,
		COLORS_SIZE
	};

private:
	enum PieceType {
		MAN,
		KING,
		PIECES_SIZE
	};

	static constexpr int ROW = 8;

	uint64_t _pieces[COLORS_SIZE][PIECES_SIZE]; // Bitmaps for each piece type and color

	static constexpr int square(int row, int col);

	static constexpr void setBit(uint64_t& bitboard, int square);
};

constexpr int Board::square(int row, int col) {
	return row * ROW + col;
}

constexpr void Board::setBit(uint64_t& bitboard, int square) {
	bitboard |= (1ULL << square);
}


#endif //BIT_CHECKERS_BOARD_H
