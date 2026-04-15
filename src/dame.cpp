#include "dame.hpp"

#include <cmath>

Dame::Dame(bool isBlack):Piece(isBlack, "dame") {}

bool Dame::isValidMove(const Position &start_pos, const Position &end_pos, bool isCapture, Plateau* board) const {
	const int dx = end_pos.getX() - start_pos.getX();
	const int dy = end_pos.getY() - start_pos.getY();
	const int absDx = std::abs(dx);
	const int absDy = std::abs(dy);

    // deplacement non nul
	if (dx == 0 && dy == 0) {
		return false;
	}
    // move diagonal
	if (absDx != absDy) {
		return false;
	}

	const int stepX = (dx > 0) ? 1 : -1;
	const int stepY = (dy > 0) ? 1 : -1;
	int x = start_pos.getX() + stepX;
	int y = start_pos.getY() + stepY;
    // chemin empty
	while (x != end_pos.getX() && y != end_pos.getY()) {
		if (board->getPiece(Position(x, y)) != nullptr) {
			return false;
		}
		x += stepX;
		y += stepY;
	}

	return true;
}