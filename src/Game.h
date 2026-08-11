//
// Created by Sagiv Marzini on 06/08/2026.
//

#ifndef BIT_CHECKERS_GAME_H
#define BIT_CHECKERS_GAME_H
#include "Board.h"


class IPlayer;

class Game {
public:
	Game(const IPlayer *player1, const IPlayer *player2);

	void playTurn();


	static void validateMoveString(const std::string& move);

	bool validateAndApplyMove(const Move& move, const std::vector<Move>& possibleMoves);

	static constexpr Move encodeMove(UnpackedMove move);

	static constexpr UnpackedMove decodeMove(Move move);

	static Move moveStringToBinary(const std::string& move);

	[[nodiscard]] bool isGameOver() const;

	[[nodiscard]] Board::Color getWinner() const;

	[[nodiscard]] const Board& getBoard();

private:
	Board _board;
	const IPlayer *_player1;
	const IPlayer *_player2;
	const IPlayer *_currentPlayer;
	bool _gameOver = false;
	Board::Color _winner;

	static std::string binaryMoveToString(const Move& move);
};


#endif //BIT_CHECKERS_GAME_H
