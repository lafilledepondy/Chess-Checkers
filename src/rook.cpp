#include "rook.hpp"

Rook::Rook(bool isBlack):Piece(isBlack, "rook") {}

bool Rook::isValidMove(const Position &start_pos, const Position &end_pos, bool isCapture, Plateau* board) const {
    const int dx = abs(end_pos.getX() - start_pos.getX());
    const int dy = abs(end_pos.getY() - start_pos.getY());
    const Piece* target = board->getPiece(end_pos);

    // no move
    if (dx == 0 && dy == 0) return false;
    // not straight line
    if (!(dx == 0 || dy == 0)) return false;

    // check path
    const int stepX = dx == 0 ? 0 : (end_pos.getX() > start_pos.getX() ? 1 : -1); // 0: no move ; 1: move right -1: move left
    const int stepY = dy == 0 ? 0 : (end_pos.getY() > start_pos.getY() ? 1 : -1); 

    int x = start_pos.getX() + stepX;
    int y = start_pos.getY() + stepY;

    while (x != end_pos.getX() || y != end_pos.getY()) {
        if (board->getPiece(Position(x, y)) != nullptr) return false;
        x += stepX;
        y += stepY;
    }

    // capture?
    if (isCapture) {
        // capured
        return target != nullptr && target->getIsBlack() != getIsBlack();
    } else {
        // empty square
        return target == nullptr;
    }
}