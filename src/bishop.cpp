#include "bishop.hpp"

Bishop::Bishop(bool isBlack):Piece(isBlack, "bishop") {}

bool Bishop::isValidMove(const Position &start_pos, const Position &end_pos, bool isCapture, Plateau* board) const {
    int dx = abs(end_pos.getX() - start_pos.getX());
    int dy = abs(end_pos.getY() - start_pos.getY());
    Piece* target = board->getPiece(end_pos);

    // no move
    if (dx == 0 && dy == 0) return false;

    // straight diagonal
    if (dx != dy) return false;

    // path checking
    int stepX = (end_pos.getX() > start_pos.getX()) ? 1 : -1; // 1: move right -1: move left
    int stepY = (end_pos.getY() > start_pos.getY()) ? 1 : -1;

    int x = start_pos.getX() + stepX;
    int y = start_pos.getY() + stepY;

    while (x != end_pos.getX() && y != end_pos.getY()) {
        if (board->getPiece(Position(x, y)) != nullptr) return false;
        x += stepX;
        y += stepY;
    }

    // capture
    if (isCapture) {
        return target != nullptr && target->getIsBlack() != getIsBlack();
    } else {
        return target == nullptr;
    }
}