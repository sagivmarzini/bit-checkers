//
// Created by Sagiv Marzini on 06/08/2026.
//

#include "AiPlayer.h"
#include <cassert>
#include <algorithm>
#include "Game.h"
#include "MoveGenerator.h"

AiPlayer::AiPlayer()
	: IPlayer(Board::Black) {
}

AiPlayer::AiPlayer(Board::Color color)
	: IPlayer(color) {
}

Move AiPlayer::getMove(Board& board, const std::vector<Move>& possibleMoves) {
	int bestScore = -WIN_SCORE * 2; // Safely below any possible evaluation
	Move bestMove = possibleMoves.front();

	const Board::Color enemyColor = Board::getEnemyColor(_color);

	// We start searching at DEPTH - 1 because applying the move here counts as depth 1
	const int searchDepth = 7;

	for (const auto& move: possibleMoves) {
		board.applyMove(_color, move);

		// Negate the result because negamax returns the score from the enemy's perspective
		const int score = -negamax(board, searchDepth - 1, -WIN_SCORE * 2, WIN_SCORE * 2, enemyColor);

		board.undoMove();

		if (score > bestScore) {
			bestScore = score;
			bestMove = move;
		}
	}

	return bestMove;
}

int AiPlayer::negamax(Board& board, int depth, int alpha, int beta, Board::Color playingColor) {
	if (depth == 0) {
		return evaluate(board, playingColor);
	}

	const auto possibleMoves = MoveGenerator::generateMoves(board, playingColor);

	if (possibleMoves.empty()) {
		// We add 'depth' so the AI prefers winning faster (higher depth remaining)
		// and losing slower (lower depth remaining).
		return -WIN_SCORE - depth;
	}

	int value = -WIN_SCORE * 2;
	const Board::Color nextColor = Board::getEnemyColor(playingColor);

	for (const auto& move: possibleMoves) {
		auto before = board.getOccupied();

		board.applyMove(playingColor, move);

		// Standard Negamax Alpha-Beta recurrence
		const int score = -negamax(board, depth - 1, -beta, -alpha, nextColor);

		board.undoMove();
		assert(board.getOccupied() == before && "undoMove did not restore board state");

		value = std::max(value, score);
		alpha = std::max(alpha, value);

		// Alpha-Beta Pruning: If we found a move that is too good, the opponent
		// will avoid this branch entirely. We can stop searching.
		if (alpha >= beta) {
			break;
		}
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
	                     + (board.countPiece(color, Board::King) - board.countPiece(enemy, Board::King)) * 10;

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
