//
// Created by Sagiv Marzini on 06/08/2026.
//

#ifndef BIT_CHECKERS_GAME_H
#define BIT_CHECKERS_GAME_H
#include "AiPlayer.h"
#include "Board.h"
#include "HumanPlayer.h"


class IPlayer;

class Game {
public:
	Game();

	void playTurn();

	static void validateMoveString(const std::string& move);

	bool makeMove(const Move& move);

	static constexpr UnpackedMove decodeMove(Move move);

	static Move moveStringToBinary(const std::string& move);

private:
	Board _board;
	HumanPlayer _human;
	AiPlayer _computer;
	IPlayer *_currentPlayer;

	static std::string binaryMoveToString(const Move& move);
};


#endif //BIT_CHECKERS_GAME_H
