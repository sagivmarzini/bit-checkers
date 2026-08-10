//
// Created by Sagiv Marzini on 06/08/2026.
//

#include "AiPlayer.h"

#include <random>

#include "Game.h"
#include "MoveGenerator.h"

AiPlayer::AiPlayer()
	: IPlayer(Board::Black) {
}

Move AiPlayer::getMove(const Board& board) {
	const auto possibleMoves = MoveGenerator::generateMoves(board, Board::Black);

	int lowestRow = Board::ROW;
	Move farthestMove;
	for (const auto& move: possibleMoves) {
		const auto [src, dest, flags] = Game::decodeMove(move);
		const int currentRow = dest / Board::ROW;
		if (currentRow < lowestRow && dest < src) {
			lowestRow = currentRow;
			farthestMove = move;
		}
	}
	return farthestMove;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, possibleMoves.size() - 1);

	int random_index = distr(gen);

	return possibleMoves[random_index];
}
