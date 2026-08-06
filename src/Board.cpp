//
// Created by Sagiv Marzini on 06/08/2026.
//

#include "Board.h"

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

std::ostream& operator<<(std::ostream& os, const Board& board) {
	for (int row = 7; row >= 0; row--) {
		os << "+---+---+---+---+---+---+---+---+\n";
		for (int col = 0; col < 8; col++) {
			bool isBlackMan = board._pieces[Board::BLACK][Board::MAN] & (1ULL << Board::square(row, col));
			bool isWhiteMan = board._pieces[Board::WHITE][Board::MAN] & (1ULL << Board::square(row, col));
			os << "| " << (isBlackMan ? "● " : isWhiteMan ? "○ " : "  ");
		}
		os << "|\n";
	}
	os << "+---+---+---+---+---+---+---+---+\n";

	return os;
}
