//
// Created by Sagiv Marzini on 11/08/2026.
//

#include "Renderer.h"

Renderer::Renderer(unsigned int width, unsigned int height, const std::string& windowTitle)
	: _window(sf::VideoMode({width, height}), windowTitle) {
}

void Renderer::processEvents() {
	while (const std::optional event = _window.pollEvent()) {
		if (event->is<sf::Event::Closed>())
			_window.close();
	}

	_cellSize = static_cast<float>(std::min(_window.getSize().x, _window.getSize().y)) / Board::ROW;
}

void Renderer::draw(const Board& board) {
	_window.clear(sf::Color::Black);

	drawCheckersBoard({});
	drawPieces(board);

	_window.display();
}

bool Renderer::isOpen() const {
	return _window.isOpen();
}

UnpackedMove Renderer::getMoveInput(const std::vector<Move>& possibleMoves) {
	while (true) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(_window);
		const auto hoveredPos = pixelToSquare(mousePos);

		drawCheckersBoard(hoveredPos);
	}
}

void Renderer::drawCheckersBoard(std::optional<int> hoveredSquare) {
	for (int row = 0; row < Board::ROW; ++row) {
		for (int col = 0; col < Board::ROW; ++col) {
			int hoveredBonus = hoveredSquare.has_value() && hoveredSquare.value() / 8 == row ? 100 : 0;

			sf::RectangleShape square;

			square.setSize({_cellSize, _cellSize});

			square.setPosition({
				col * _cellSize,
				row * _cellSize
			});

			if ((row + col) % 2 == 0)
				square.setFillColor(sf::Color(240 + hoveredBonus, 217, 181));
			else
				square.setFillColor(sf::Color(181, 136, 99));

			_window.draw(square);
		}
	}
}

void Renderer::drawPieces(const Board& board) {
	for (int row = 0; row < Board::ROW; ++row) {
		for (int col = 0; col < Board::ROW; ++col) {
			const float radius = _cellSize * 0.4f;

			sf::CircleShape man(radius);
			man.setOrigin({radius, radius});
			const int screenRow = Board::ROW - 1 - row;
			man.setPosition({
				col * _cellSize + _cellSize / 2.f,
				screenRow * _cellSize + _cellSize / 2.f
			});

			sf::CircleShape king(radius, 3);
			king.setOrigin({radius, radius});
			king.setPosition({
				col * _cellSize + _cellSize / 2.f,
				screenRow * _cellSize + _cellSize / 2.f
			});


			switch (board.pieceAt(row * 8 + col)) {
				case Board::PieceKind::None:
					continue;
				case Board::PieceKind::BlackMan:
					man.setFillColor(BLACK);
					_window.draw(man);
					break;
				case Board::PieceKind::BlackKing:
					king.setFillColor(BLACK);
					_window.draw(king);
					break;
				case Board::PieceKind::WhiteMan:
					man.setFillColor(WHITE);
					_window.draw(man);
					break;
				case Board::PieceKind::WhiteKing:
					king.setFillColor(WHITE);
					_window.draw(king);
					break;
			}
		}
	}
}

std::optional<int> Renderer::pixelToSquare(sf::Vector2i pixel) const {
	int col = pixel.x / static_cast<int>(_cellSize);
	int row = pixel.y / static_cast<int>(_cellSize);
	if (col < 0 || col >= Board::ROW || row < 0 || row >= Board::ROW)
		return std::nullopt;

	// Flipping rows for display
	// Here too so it maps back to board coords correctly
	int boardRow = Board::ROW - 1 - row;
	return boardRow * Board::ROW + col;
}
