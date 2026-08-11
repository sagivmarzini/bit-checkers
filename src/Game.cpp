//
// Created by Sagiv Marzini on 06/08/2026.
//

#include "Game.h"

#include "InvalidMove.h"
#include "MoveGenerator.h"

Game::Game()
	: _currentPlayer(&_human), _winner() {
}

void Game::playTurn() {
	const auto color = _currentPlayer->getColor();

	std::cout << _board << '\n';

	const auto possibleMoves =
			MoveGenerator::generateMoves(_board, color);

	if (possibleMoves.empty()) {
		_gameOver = true;
		_winner = Board::getEnemyColor(color);
		return;
	}

	Move move = _currentPlayer->getMove(_board, possibleMoves);

	while (!validateAndApplyMove(move, possibleMoves)) {
		std::cout << "Illegal move. Try again.\n";
		move = _currentPlayer->getMove(_board, possibleMoves);
	}
}

bool Game::validateAndApplyMove(
	const Move& move,
	const std::vector<Move>& possibleMoves
) {
	for (const auto possibleMove: possibleMoves) {
		if ((possibleMove & 0xFFF) == (move & 0xFFF)) {
			_board.applyMove(_currentPlayer->getColor(), possibleMove);
			_board.setLastMove(possibleMove);

			_currentPlayer = (_currentPlayer == &_human)
				                 ? static_cast<IPlayer*>(&_computer)
				                 : static_cast<IPlayer*>(&_human);

			return true;
		}
	}

	return false;
}

constexpr UnpackedMove Game::decodeMove(const Move move) {
	return {
		static_cast<uint8_t>(move & 0b0000000000111111),
		static_cast<uint8_t>((move & 0b0000111111000000) >> 6),
		static_cast<MoveFlags>((move & 0b1111000000000000) >> 12)
	};
}

void Game::validateMoveString(const std::string& move) {
	if (move.length() != 4) throw InvalidMove("Invalid move format (format: <from><to>, e.g. a3b4)");
	if (move[0] < 'a' || move[0] > 'h' || move[2] < 'a' || move[2] > 'h')
		throw InvalidMove("Must use files 'a' to 'h'");
	if (move[1] < '1' || move[1] > '8' || move[3] < '1' || move[3] > '8')
		throw InvalidMove("Must use ranks 1 to 8");
	if (move.substr(0, 2) == move.substr(2, 2))
		throw InvalidMove("Source and destination squares must be different");
}

std::string Game::binaryMoveToString(const Move& move) {
	const auto [src, dest, flag] = decodeMove(move);

	std::string moveSrc = std::string(1, static_cast<char>(src % 8 + 'a')) + std::to_string(src / 8 + 1);
	std::string moveDest = std::string(1, static_cast<char>(dest % 8 + 'a')) + std::to_string(dest / 8 + 1);

	return moveSrc + moveDest;
}

Move Game::moveStringToBinary(const std::string& move) {
	const int src = move[0] - 'a' + ((move[1] - '1') * Board::ROW);
	const int dest = move[2] - 'a' + ((move[3] - '1') * Board::ROW);

	return MoveGenerator::createMove(src, dest, MoveFlags::None);
}

bool Game::isGameOver() const {
	return _gameOver;
}

Board::Color Game::getWinner() const {
	if (!_gameOver) throw std::runtime_error("No winners yet. Game is not over.");

	return _winner;
}

const Board& Game::getBoard() {
	return _board;
}
