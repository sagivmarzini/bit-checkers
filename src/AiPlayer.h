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

	Move getMove(Board& board, const std::vector<Move>& possibleMoves) override;

private:
	static constexpr int CENTER_SCORE[Board::ROW * Board::ROW] = {
		-78, -50, -34, -26, -26, -34, -50, -78,
		-50, -26, -10, -2, -2, -10, -26, -50,
		-34, -10, 6, 14, 14, 6, -10, -34,
		-26, -2, 14, 18, 18, 14, -2, -26,
		-26, -2, 14, 18, 18, 14, -2, -26,
		-34, -10, 6, 14, 14, 6, -10, -34,
		-50, -26, -10, -2, -2, -10, -26, -50,
		-78, -50, -34, -26, -26, -34, -50, -78
	};
	static constexpr int WIN_SCORE = std::numeric_limits<int>::max();

	static int negamax(Board& board, int depth, int color);

	static int evaluate(const Board& board, Board::Color color);

	static int advancementScore(const Board& board, Board::Color color);

	static int centerScore(const Board& board, Board::Color color);
};


#endif //BIT_CHECKERS_AIPLAYER_H
