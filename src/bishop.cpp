#include "bishop.hpp"

Bishop::Bishop(bool isBlack):Piece(isBlack, "bishop") {}

bool Bishop::isValidMove(const Position &start_pos, const Position &end_pos, bool isCapture, Plateau* board) const {
    const int dx = abs(end_pos.getX() - start_pos.getX());
    const int dy = abs(end_pos.getY() - start_pos.getY());
    Piece* target = board->getPiece(end_pos);

    // no move
    if (dx == 0 && dy == 0) return false;

    // must be diagonal
    if (dx != dy) return false;

    // check path
    const int stepX = (end_pos.getX() > start_pos.getX()) ? 1 : -1;
    const int stepY = (end_pos.getY() > start_pos.getY()) ? 1 : -1;
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