//
// Created by Sagiv Marzini on 06/08/2026.
//

#ifndef BIT_CHECKERS_BOARD_H
#define BIT_CHECKERS_BOARD_H
#include <iostream>
#include <vector>


using Bitboard = uint64_t;
using Move = uint16_t;

enum MoveFlags : uint8_t {
	None = 0b0000,
	Capture = 0b0001,
	// Promotion = 0b0010,
	Multijump = 0b0100,
};

struct UnpackedMove {
	uint8_t src;
	uint8_t dest;
	MoveFlags flags;
};

class Board {
public:
	enum Color {
		White,
		Black,
		COLORS_SIZE
	};

	enum PieceType {
		Man,
		King,
		PIECES_SIZE
	};

	enum class PieceKind { None, BlackMan, BlackKing, WhiteMan, WhiteKing };

	enum DiagonalMove {
		RightUpOffset = 9,
		LeftUpOffset = 7,
		RightDownOffset = -7,
		LeftDownOffset = -9
	};

	enum Mask : uint64_t {
		MaskRightUp = 0x7F7F7F7F7F7F7F, // Zeros the right and top columns
		MaskLeftUp = 0xFEFEFEFEFEFEFE,  // left and top
		MaskRightDown = 0x7F7F7F7F7F7F7F00,
		MaskLeftDown = 0xFEFEFEFEFEFEFE00
	};

	static constexpr int ROW = 8;

	Board();


	[[nodiscard]] Bitboard getOccupied() const;

	[[nodiscard]] Bitboard getUnoccupied() const;

	[[nodiscard]] const Bitboard& getPieceBitboard(Color color, PieceType piece) const;

	[[nodiscard]] Bitboard getColorPieces(Color color) const;

	[[nodiscard]] int countPiece(Color color, PieceType piece) const;

	void applyMove(Color player, Move move);

	void undoMove();


	static void printBitboard(const Bitboard& board);

	void setLastMove(Move lastMove);


	static constexpr Color getEnemyColor(Color color);

	[[nodiscard]] PieceKind pieceAt(int square) const;

	PieceType pieceTypeAt(int square) const;

	friend std::ostream& operator<<(std::ostream& os, const Board& board);

private:
	std::array<std::array<Bitboard, PIECES_SIZE>, COLORS_SIZE> _pieces;
	std::vector<std::array<std::array<Bitboard, PIECES_SIZE>, COLORS_SIZE> > _history;
	uint8_t _lastSrcPos = -1; // For displaying the last move
	uint8_t _lastDestPos = -1;
	uint8_t _lastCapturePos = -1;


	static constexpr int square(int row, int col);

	static constexpr void setBit(Bitboard& board, int square);

	static constexpr void unsetBit(Bitboard& board, int square);


	static const char *glyphFor(Board::PieceKind kind, bool isLastDest);

	static const char *emptySquareGlyph(const Board& board, int square);

	static DiagonalMove calculateDiagonalOffset(int src, int dest);

	static int getCapturedPosition(int src, int dest);
};

constexpr Board::Color Board::getEnemyColor(const Color color) {
	return color == White ? Black : White;
}

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
