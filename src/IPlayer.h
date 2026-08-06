//
// Created by Sagiv Marzini on 06/08/2026.
//

#ifndef BIT_CHECKERS_IPLAYER_H
#define BIT_CHECKERS_IPLAYER_H
#include "Board.h"


class IPlayer {
public:
	explicit IPlayer(Board::Color color) : color(color) {
	}

	virtual ~IPlayer() = default;

	virtual Move getMove(const Board& board) = 0;

protected:
	Board::Color color;
};

#endif //BIT_CHECKERS_IPLAYER_H
