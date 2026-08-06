//
// Created by Sagiv Marzini on 06/08/2026.
//

#ifndef BIT_CHECKERS_BOARD_H
#define BIT_CHECKERS_BOARD_H
#include <iostream>


using Bitboard = uint64_t;
using Move = uint16_t;

enum MoveFlag : uint8_t {
	QUIET,
	CAPTURE,
	PROMOTION,
	MULTIJUMP
};

struct UnpackedMove {
	uint8_t src;
	uint8_t dest;
	MoveFlag flag;
};

class Board {
public:
	enum Color {
		WHITE,
		BLACK,
		COLORS_SIZE
	};

	enum PieceType {
		MAN,
		KING,
		PIECES_SIZE
	};

	static constexpr int ROW = 8;

	Board();

	[[nodiscard]] Bitboard getUnoccupied() const;

	[[nodiscard]] const Bitboard& get(Color color, PieceType piece) const;

	void applyMove(Color player, Move move);

	static void printBitboard(const Bitboard& board);

	friend std::ostream& operator<<(std::ostream& os, const Board& board);

private:
	Bitboard _pieces[COLORS_SIZE][PIECES_SIZE]; // Bitmaps for each piece type and color

	static constexpr int square(int row, int col);

	static constexpr void setBit(Bitboard& board, int square);

	static constexpr void unsetBit(Bitboard& board, int square);
};

constexpr int Board::square(int row, int col) {
	return row * ROW + col;
}

constexpr void Board::setBit(Bitboard& board, int square) {
	board |= (1ULL << square);
}

constexpr void Board::unsetBit(Bitboard& board, int square) {
	board &= ~(1ULL << square);
}


#endif //BIT_CHECKERS_BOARD_H
