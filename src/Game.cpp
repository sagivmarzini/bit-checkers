//
// Created by Sagiv Marzini on 06/08/2026.
//

#include "Game.h"

#include "InvalidMove.h"

Game::Game()
	: _board() {
}

void Game::printBoard() const {
	std::cout << _board;
}

bool Game::makeMove(const std::string& move) {
	if (move.length() != 4) throw InvalidMove("Invalid move format (format: <from><to>, e.g. a3b4)");
	if (move[0] < 'a' || move[0] > 'h' || move[2] < 'a' || move[2] > 'h')
		throw InvalidMove("Must use files 'a' to 'h'");
	if (move[1] < '1' || move[1] > '8' || move[3] < '1' || move[3] > '8')
		throw InvalidMove("Must use ranks 1 to 8");
	if (move.substr(0, 2) == move.substr(2, 2))
		throw InvalidMove("Source and destination squares must be different");
}
