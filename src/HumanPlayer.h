//
// Created by Sagiv Marzini on 06/08/2026.
//

#ifndef BIT_CHECKERS_HUMANPLAYER_H
#define BIT_CHECKERS_HUMANPLAYER_H
#include "IPlayer.h"


class Renderer;

class HumanPlayer : public IPlayer {
public:
	HumanPlayer(Renderer& renderer);


	Move getMove(Board& board, const std::vector<Move>& possibleMoves) const override;

private:
	Renderer& _renderer;

	static Move getMoveInputCli();
};


#endif //BIT_CHECKERS_HUMANPLAYER_H
