//
// Created by Sagiv Marzini on 06/08/2026.
//

#include <thread>

#include "Board.h"
#include "Game.h"
#include "HumanPlayer.h"
#include "InvalidMove.h"

int main() {
	Game game;

	while (true) {
		game.playTurn();
	}

	return 0;
}
