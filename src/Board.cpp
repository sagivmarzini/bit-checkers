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
	_pieces[WHITE][KING] |= 1ULL << 25;
	_pieces[WHITE][KING] |= 1ULL << 31;
	_pieces[BLACK][KING] |= 1ULL << 32;

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
	const PieceType srcPiece = pieceTypeAt(src);


	switch (flag) {
		case QUIET:
			setBit(_pieces[player][srcPiece], dest);
			unsetBit(_pieces[player][srcPiece], src);
			break;
		case CAPTURE: {
			setBit(_pieces[player][srcPiece], dest);
			unsetBit(_pieces[player][srcPiece], src);
			const auto capturedPos = getCapturedPosition(src, dest);
			const PieceType capturedPiece = pieceTypeAt(capturedPos);
			unsetBit(_pieces[getEnemyColor(player)][capturedPiece], capturedPos);
			break;
		}
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

Board::PieceKind Board::pieceAt(int square) const {
	const uint64_t mask = 1ULL << square;
	if (_pieces[BLACK][MAN] & mask) return PieceKind::BlackMan;
	if (_pieces[BLACK][KING] & mask) return PieceKind::BlackKing;
	if (_pieces[WHITE][MAN] & mask) return PieceKind::WhiteMan;
	if (_pieces[WHITE][KING] & mask) return PieceKind::WhiteKing;
	return PieceKind::None;
}

Board::PieceType Board::pieceTypeAt(int square) {
	switch (pieceAt(square)) {
		case PieceKind::BlackMan:
		case PieceKind::WhiteMan:
			return MAN;
		case PieceKind::BlackKing:
		case PieceKind::WhiteKing:
			return KING;
	}

	throw std::invalid_argument("No piece at given square.");
}

const char *Board::glyphFor(Board::PieceKind kind, bool isLastDest) {
	switch (kind) {
		case Board::PieceKind::BlackMan: return isLastDest ? "◉" : "●";
		case Board::PieceKind::BlackKing: return isLastDest ? "◈" : "◆";
		case Board::PieceKind::WhiteMan: return isLastDest ? "◎" : "○";
		case Board::PieceKind::WhiteKing: return isLastDest ? "◇" : "◇"; // placeholder
		case Board::PieceKind::None: return nullptr;                     // caller handles empty-square markers
	}
	return nullptr; // unreachable, silences -Wreturn-type on some compilers
}

const char *Board::emptySquareGlyph(const Board& board, int square) {
	if (board._lastSrcPos == square) return ".";
	if (board._lastCapturePos == square) return "x";
	return " ";
}


std::ostream& operator<<(std::ostream& os, const Board& board) {
	constexpr const char *divider = "  +---+---+---+---+---+---+---+---+\n";

	for (int row = 7; row >= 0; --row) {
		os << divider << (row + 1) << ' ';

		for (int col = 0; col < 8; ++col) {
			const int sq = Board::square(row, col);
			const auto kind = board.pieceAt(sq);
			const bool isLastDest = board._lastDestPos == sq;

			const char *glyph = (kind == Board::PieceKind::None)
				                    ? Board::emptySquareGlyph(board, sq)
				                    : Board::glyphFor(kind, isLastDest);

			os << "| " << glyph << ' ';
		}
		os << "|\n";
	}

	os << divider << "    A   B   C   D   E   F   G   H\n";
	return os;
}
