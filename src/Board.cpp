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

Bitboard Board::getOccupied() const {
	return _pieces[WHITE][MAN] | _pieces[WHITE][KING] | _pieces[BLACK][MAN] | _pieces[BLACK][KING];
}

Bitboard Board::getUnoccupied() const {
	return ~getOccupied();
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
			setBit(_pieces[player][MAN], dest);
			unsetBit(_pieces[player][MAN], src);
			unsetBit(_pieces[getEnemyColor(player)][MAN], getCapturedPosition(src, dest));
			break;
		case PROMOTION:
			break;
		case MULTIJUMP:
			break;
	}
}

Bitboard Board::getColorPieces(Color color) const {
	return _pieces[color][MAN] | _pieces[color][KING];
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

void Board::setLastMove(Move lastMove) {
	const auto move = Game::decodeMove(lastMove);
	_lastSrcPos = move.src;
	_lastDestPos = move.dest;
	_lastCapturePos = move.flag == CAPTURE ? getCapturedPosition(move.src, move.dest) : -1;
}

int Board::getCapturedPosition(uint8_t src, uint8_t dest) {
	// For now just a man piece capturing
	return (src + dest) / 2;
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
	for (int row = 7; row >= 0; row--) {
		os << "  +---+---+---+---+---+---+---+---+\n";
		os << row + 1 << " ";
		for (int col = 0; col < 8; col++) {
			bool isBlackMan = board._pieces[Board::BLACK][Board::MAN] & (1ULL << Board::square(row, col));
			bool isWhiteMan = board._pieces[Board::WHITE][Board::MAN] & (1ULL << Board::square(row, col));
			bool isLastSrc = board._lastSrcPos == row * Board::ROW + col;
			bool isLastDest = board._lastDestPos == row * Board::ROW + col;
			bool isLastCapture = board._lastCapturePos == row * Board::ROW + col;
			os << "| " << (isBlackMan
				               ? isLastDest
					                 ? "◉"
					                 : "●"
				               : isWhiteMan
					                 ? isLastDest
						                   ? "◎"
						                   : "○"
					                 : isLastSrc
						                   ? "."
						                   : isLastCapture
							                     ? "x"
							                     : " ") << ' ';
		}
		os << "|\n";
	}
	os << "  +---+---+---+---+---+---+---+---+\n";
	os << "    A   B   C   D   E   F   G   H\n";

	return os;
}
