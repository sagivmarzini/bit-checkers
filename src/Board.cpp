//
// Created by Sagiv Marzini on 06/08/2026.
//

#include "Board.h"

#include "Game.h"

Board::Board()
	: _pieces() {
	// Init the black men on the top 3 rows
	for (int row = 5; row < 8; row++) {
		for (int col = 0; col < ROW; col++) {
			if ((row + col) % 2 == 0) {
				setBit(_pieces[BLACK][MAN], row * ROW + col);
			}
		}
	}

	// Init the white men on the bottom
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < ROW; col++) {
			if ((row + col) % 2 == 0) {
				setBit(_pieces[WHITE][MAN], row * ROW + col);
			}
		}
	}
}

Bitboard Board::getUnoccupied() const {
	const Bitboard occupied = _pieces[WHITE][MAN] | _pieces[WHITE][KING] | _pieces[BLACK][MAN] | _pieces[BLACK][KING];
	return ~occupied;
}

const Bitboard& Board::get(Color color, PieceType piece) const {
	return _pieces[color][piece];
}

void Board::applyMove(Color player, Move move) {
	const auto [src, dest, flag] = Game::decodeMove(move);

	switch (flag) {
		case QUIET:
			setBit(_pieces[player][MAN], dest);
			unsetBit(_pieces[player][MAN], src);
			break;
		case CAPTURE:
			break;
		case PROMOTION:
			break;
		case MULTIJUMP:
			break;
	}
}

void Board::printBitboard(const Bitboard& board) {
	for (int row = 7; row >= 0; row--) {
		std::cout << "  +---+---+---+---+---+---+---+---+\n";
		std::cout << row + 1 << " ";
		for (int col = 0; col < 8; col++) {
			bool exists = board & (1ULL << Board::square(row, col));
			std::cout << "| " << (exists ? "1 " : "  ");
		}
		std::cout << "|\n";
	}
	std::cout << "  +---+---+---+---+---+---+---+---+\n";
	std::cout << "    A   B   C   D   E   F   G   H\n";
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
	for (int row = 7; row >= 0; row--) {
		os << "  +---+---+---+---+---+---+---+---+\n";
		os << row + 1 << " ";
		for (int col = 0; col < 8; col++) {
			bool isBlackMan = board._pieces[Board::BLACK][Board::MAN] & (1ULL << Board::square(row, col));
			bool isWhiteMan = board._pieces[Board::WHITE][Board::MAN] & (1ULL << Board::square(row, col));
			os << "| " << (isBlackMan ? "● " : isWhiteMan ? "○ " : "  ");
		}
		os << "|\n";
	}
	os << "  +---+---+---+---+---+---+---+---+\n";
	os << "    A   B   C   D   E   F   G   H\n";

	return os;
}
