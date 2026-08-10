//
// Created by Sagiv Marzini on 06/08/2026.
//

#include "Game.h"

constexpr const char *WELCOME = R"( ______ __ __        ______ __                __
|   __ \__|  |_     |      |  |--.-----.----.|  |--.-----.----.-----.
|   __ <  |   _|    |   ---|     |  -__|  __||    <|  -__|   _|__ --|
|______/__|____|    |______|__|__|_____|____||__|__|_____|__| |_____|
                                                                     )";
constexpr const char *GAME_OVER = R"(▄▖         ▄▖
▌ ▀▌▛▛▌█▌  ▌▌▌▌█▌▛▘
▙▌█▌▌▌▌▙▖  ▙▌▚▘▙▖▌
                   )";

int main() {
	Game game;

	std::cout << WELCOME << '\n';
	while (!game.isGameOver()) {
		game.playTurn();
	}

	std::cout << GAME_OVER << '\n' << (game.getWinner() == Board::White ? "White" : "Black") << " won!" << '\n';

	return 0;
}
