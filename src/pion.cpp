#include "pion.hpp"

#include <cmath>

Pion::Pion(bool isBlack):Piece(isBlack, "pion") {}

bool Pion::isValidMove(const Position &start_pos, const Position &end_pos, bool isCapture, Plateau* board) const {
	const int direction = getIsBlack() ? 1 : -1;
	const int dx = end_pos.getX() - start_pos.getX();
	const int dy = end_pos.getY() - start_pos.getY();
	const int absDx = std::abs(dx);
	const int absDy = std::abs(dy);

	// end_pos empty 
	if (board->getPiece(end_pos) != nullptr) {
		return false;
	}

	// one diagonal step forward
	if (absDx == 1 && dy == direction) {
		return true;
	}

	// capture jump one opponent piece diagonally to an empty square
	if (absDx == 2 && absDy == 2) {
		const Position middle(start_pos.getX() + dx / 2, start_pos.getY() + dy / 2);
		const Piece* middlePiece = board->getPiece(middle);
		return middlePiece != nullptr && middlePiece->getIsBlack() != getIsBlack();
	}

	return false;
}