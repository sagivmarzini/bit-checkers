//
// Created by Sagiv Marzini on 06/08/2026.
//

#include "AiPlayer.h"

#include <random>

#include "MoveGenerator.h"

AiPlayer::AiPlayer()
	: IPlayer(Board::BLACK) {
}

Move AiPlayer::getMove(const Board& board) {
	const auto possibleMoves = MoveGenerator::generateMoves(board, Board::BLACK);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, possibleMoves.size() - 1);

	int random_index = distr(gen);

	return possibleMoves[random_index];
}
