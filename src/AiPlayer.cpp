//
// Created by Sagiv Marzini on 06/08/2026.
//

#include "AiPlayer.h"

#include <random>

#include "Game.h"

AiPlayer::AiPlayer()
	: IPlayer(Board::Black) {
}

AiPlayer::AiPlayer(Board::Color color)
	: IPlayer(color) {
}

Move AiPlayer::getMove(const Board& board, const std::vector<Move>& possibleMoves) {
	int lowestRow = Board::ROW;
	Move farthestMove = possibleMoves[0];
	for (const auto& move: possibleMoves) {
		const auto [src, dest, flags] = Game::decodeMove(move);
		const int currentRow = dest / Board::ROW;
		if (currentRow < lowestRow && dest < src) {
			lowestRow = currentRow;
			farthestMove = move;
		}
	}
	return farthestMove;
}
