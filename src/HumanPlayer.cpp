//
// Created by Sagiv Marzini on 06/08/2026.
//

#include "HumanPlayer.h"

#include "InvalidMove.h"
#include "Renderer.h"
#include "Board.h"


HumanPlayer::HumanPlayer(Renderer& renderer)
	: IPlayer(Board::White), _renderer(renderer) {
}

Move HumanPlayer::getMove(Board& board, const std::vector<Move>& possibleMoves) const {
	return Game::encodeMove(_renderer.getMoveInput(possibleMoves));
}

Move HumanPlayer::getMoveInputCli() {
	std::string moveStr;

	while (true) {
		std::cout << "\nEnter move (format: <from><to>, e.g. a3b4):\n> ";
		std::cin >> moveStr;
		std::transform(moveStr.begin(), moveStr.end(), moveStr.begin(), [](unsigned char c) {
			return std::tolower(c);
		});

		try {
			Game::validateMoveString(moveStr);
			return Game::moveStringToBinary(moveStr);
		} catch (const InvalidMove& e) {
			std::cerr << "Invalid move: " << e.what() << "\n";
		}
	}
}
