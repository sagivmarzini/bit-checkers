//
// Created by Sagiv Marzini on 06/08/2026.
//

#ifndef BIT_CHECKERS_HUMANPLAYER_H
#define BIT_CHECKERS_HUMANPLAYER_H
#include "IPlayer.h"


class HumanPlayer : public IPlayer {
public:
	HumanPlayer();

	Move getMove(const Board& board, const std::vector<Move>& possibleMoves) override;
};


#endif //BIT_CHECKERS_HUMANPLAYER_H
