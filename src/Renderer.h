//
// Created by Sagiv Marzini on 11/08/2026.
//

#ifndef BIT_CHECKERS_RENDERER_H
#define BIT_CHECKERS_RENDERER_H

#include <SFML/Graphics.hpp>

#include "Board.h"
#include "Game.h"


class Renderer {
public:
	Renderer(unsigned int width, unsigned int height, const std::string& windowTitle);

	void processEvents();


	void draw(const Board& board);

	bool isOpen() const;

	// UnpackedMove getMove();

private:
	sf::RenderWindow _window;
	float _cellSize = 1;
	sf::Color DARK_SQUARE = sf::Color(0x3b3728);
	sf::Color LIGHT_SQUARE = sf::Color(0xd9d0ad);
	sf::Color BLACK = sf::Color::Black;
	sf::Color WHITE = sf::Color::White;

	void drawCheckersBoard();

	void drawPieces(const Board& board);
};


#endif //BIT_CHECKERS_RENDERER_H
