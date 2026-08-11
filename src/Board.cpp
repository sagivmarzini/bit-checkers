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
				setBit(_pieces[Black][Man], row * ROW + col);
			}
		}
	}

	// Init the white men on the bottom
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < ROW; col++) {
			if ((row + col) % 2 == 0) {
				setBit(_pieces[White][Man], row * ROW + col);
			}
		}
	}
}

Bitboard Board::getOccupied() const {
	return _pieces[White][Man] | _pieces[White][King] | _pieces[Black][Man] | _pieces[Black][King];
}

Bitboard Board::getUnoccupied() const {
	return ~getOccupied();
}

const Bitboard& Board::getPieceBitboard(Color color, PieceType piece) const {
	return _pieces[color][piece];
}

void Board::applyMove(Color player, Move move) {
	_history.push_back(_pieces);

	const auto [src, dest, flags] = Game::decodeMove(move);
	const PieceType piece = pieceTypeAt(src);

	unsetBit(_pieces[player][piece], src);
	setBit(_pieces[player][piece], dest);

	if (flags & Capture) {
		const auto capturedPos = getCapturedPosition(src, dest);
		const PieceType capturedPiece = pieceTypeAt(capturedPos);
		unsetBit(_pieces[getEnemyColor(player)][capturedPiece], capturedPos);
	}

	bool onTopRow = dest / ROW == ROW - 1;
	bool onBottomRow = dest / ROW == 0;
	if (pieceTypeAt(dest) == Man && ((player == White && onTopRow) ||
	                                 (player == Black && onBottomRow))) {
		unsetBit(_pieces[player][piece], dest);
		setBit(_pieces[player][King], dest);
	}
}

void Board::undoMove() {
	_pieces = _history.back();
	_history.pop_back();
}

Bitboard Board::getColorPieces(Color color) const {
	return _pieces[color][Man] | _pieces[color][King];
}

int Board::countPiece(Color color, PieceType piece) const {
	return std::popcount(_pieces[color][piece]);
}

void Board::printBitboard(const Bitboard& board) {
	for (int row = 7; row >= 0; row--) {
		std::cout << "  +---+---+---+---+---+---+---+---+\n";
		std::cout << row + 1 << " ";
		for (int col = 0; col < 8; col++) {
			bool exists = board & (1ULL << square(row, col));
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
	_lastCapturePos = move.flags & Capture ? getCapturedPosition(move.src, move.dest) : -1;
}

int Board::getCapturedPosition(int src, int dest) {
	int diagonalOffset = calculateDiagonalOffset(src, dest); // one of the 4 fixed offsets
	return dest - diagonalOffset;
}

Board::PieceKind Board::pieceAt(int square) const {
	const uint64_t mask = 1ULL << square;
	if (_pieces[Black][Man] & mask) return PieceKind::BlackMan;
	if (_pieces[Black][King] & mask) return PieceKind::BlackKing;
	if (_pieces[White][Man] & mask) return PieceKind::WhiteMan;
	if (_pieces[White][King] & mask) return PieceKind::WhiteKing;
	return PieceKind::None;
}

Board::PieceType Board::pieceTypeAt(int square) const {
	switch (pieceAt(square)) {
		case PieceKind::BlackMan:
		case PieceKind::WhiteMan:
			return Man;
		case PieceKind::BlackKing:
		case PieceKind::WhiteKing:
			return King;
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

Board::DiagonalMove Board::calculateDiagonalOffset(int src, int dest) {
	const int srcFile = src % 8, srcRank = src / 8;
	const int destFile = dest % 8, destRank = dest / 8;

	const int fileStep = destFile - srcFile;
	const int rankStep = destRank - srcRank;

	if (fileStep == 0 || rankStep == 0 || std::abs(fileStep) != std::abs(rankStep))
		throw std::runtime_error("Source and destination are not on a diagonal");

	if (fileStep > 0 && rankStep > 0) return RightUpOffset;
	if (fileStep < 0 && rankStep > 0) return LeftUpOffset;
	if (fileStep > 0 && rankStep < 0) return RightDownOffset;
	return LeftDownOffset; // fileStep < 0 && rankStep < 0
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
