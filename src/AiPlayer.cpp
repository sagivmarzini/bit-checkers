//
// Created by Sagiv Marzini on 06/08/2026.
//

#include "AiPlayer.h"

#include <cassert>

#include "Game.h"
#include "MoveGenerator.h"

AiPlayer::AiPlayer()
	: IPlayer(Board::Black) {
}

AiPlayer::AiPlayer(Board::Color color)
	: IPlayer(color) {
}

Move AiPlayer::getMove(Board& board, const std::vector<Move>& possibleMoves) {
	int bestScore = std::numeric_limits<int>::min();
	Move bestMove = 0;

	for (const auto& move: possibleMoves) {
		board.applyMove(_color, move);

		const int score = negamax(board, 5, -1);

		board.undoMove();

		if (score > bestScore) {
			bestScore = score;
			bestMove = move;
		}
	}

	return bestMove;
}

int AiPlayer::negamax(Board& board, int depth, int color) {
	/*
	function negamax(node, depth, color) is
	if depth = 0 or node is a terminal node then
		return color × the heuristic value of node
	value := −∞
	for each child of node do
		value := max(value, −negamax(child, depth − 1, −color))
	return value
	 */
	const Board::Color playingColor = color == 1 ? Board::Black : Board::White;
	if (depth == 0) return color * evaluate(board, playingColor);

	const auto possibleMoves = MoveGenerator::generateMoves(board, playingColor);

	if (possibleMoves.empty()) return color * evaluate(board, playingColor);

	int value = std::numeric_limits<int>::min();
	for (const auto& move: possibleMoves) {
		auto before = board.getOccupied(); // or a hash
		board.applyMove(playingColor, move);
		value = std::max(value, -negamax(board, depth - 1, -color));
		board.undoMove();
		assert(board.getOccupied() == before && "undoMove did not restore board state");
	}

	return value;
}

int AiPlayer::evaluate(const Board& board, Board::Color color) {
	const Board::Color enemy = Board::getEnemyColor(color);

	const auto myMoves = MoveGenerator::generateMoves(board, color);
	if (myMoves.empty()) return -WIN_SCORE;

	const auto enemyMoves = MoveGenerator::generateMoves(board, enemy);
	if (enemyMoves.empty()) return WIN_SCORE;

	const int material = (board.countPiece(color, Board::Man) - board.countPiece(enemy, Board::Man))
	                     + (board.countPiece(color, Board::King) - board.countPiece(enemy, Board::King)) * 3;

	const int advancement = advancementScore(board, color) - advancementScore(board, enemy);
	const int center = centerScore(board, color) - centerScore(board, enemy);
	const int mobility = static_cast<int>(myMoves.size()) - static_cast<int>(enemyMoves.size());

	return material * 100 + advancement * 3 + center * 2 + mobility * 2;
}

int AiPlayer::advancementScore(const Board& board, Board::Color color) {
	int score = 0;
	Bitboard men = board.getPieceBitboard(color, Board::Man);
	const int startRow = color == Board::White ? 0 : 7;
	while (men) {
		score += std::abs(std::countr_zero(men) / Board::ROW - startRow);
		men &= men - 1;
	}
	return score;
}

int AiPlayer::centerScore(const Board& board, Board::Color color) {
	int score = 0;
	Bitboard kings = board.getPieceBitboard(color, Board::King);
	while (kings) {
		score += CENTER_SCORE[std::countr_zero(kings)];
		kings &= kings - 1;
	}
	return score;
}
