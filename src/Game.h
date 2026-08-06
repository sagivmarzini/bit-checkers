//
// Created by Sagiv Marzini on 06/08/2026.
//

#ifndef BIT_CHECKERS_GAME_H
#define BIT_CHECKERS_GAME_H
#include "Board.h"


class Game {
public:
	Game();

	void printBoard() const;

	// Parses a move in coordinate notation: <from><to> (e.g. a3b4)
	bool makeMove(const std::string& move);

private:
	Board _board;
	Board::Color _nextToPlay = Board::Color::WHITE;
};


#endif //BIT_CHECKERS_GAME_H
