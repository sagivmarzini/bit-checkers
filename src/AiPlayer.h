//
// Created by Sagiv Marzini on 06/08/2026.
//

#ifndef BIT_CHECKERS_AIPLAYER_H
#define BIT_CHECKERS_AIPLAYER_H
#include "IPlayer.h"


class AiPlayer : public IPlayer {
public:
	AiPlayer();

	explicit AiPlayer(Board::Color color);

	Move getMove(const Board& board, const std::vector<Move>& possibleMoves) override;
};


#endif //BIT_CHECKERS_AIPLAYER_H
