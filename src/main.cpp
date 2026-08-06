//
// Created by Sagiv Marzini on 06/08/2026.
//

#include <thread>

#include "Board.h"
#include "Game.h"
#include "InvalidMove.h"

int main() {
	Game game;
	std::string moveStr;

	while (true) {
		game.printBoard();
		std::cout << "\nEnter move (format: <from><to>, e.g. a3b4):\n> ";
		std::cin >> moveStr;
		std::transform(moveStr.begin(), moveStr.end(), moveStr.begin(), [](unsigned char c) {
			return std::tolower(c);
		});

		try {
			game.makeMove(moveStr);
		} catch (const InvalidMove& e) {
			std::cerr << "Invalid move: " << e.what() << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}

	return 0;
}
