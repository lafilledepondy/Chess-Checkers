#include "cavalier.hpp"

Cavalier::Cavalier(bool isBlack) : Piece(isBlack, "cavalier") {}

bool Cavalier::isValidMove(const Position &start_pos, const Position &end_pos, bool isCapture, Plateau* board) const {
    const int dx = abs(end_pos.getX() - start_pos.getX());
    const int dy = abs(end_pos.getY() - start_pos.getY());
    const Piece* target = board->getPiece(end_pos);

    // no move
    if (dx == 0 && dy == 0) return false;

    // 2 by 1 or 1 by 2
    if (!((dx == 2 && dy == 1) || (dx == 1 && dy == 2))) return false;

    // capture
    if (isCapture) {
        return target != nullptr && target->getIsBlack() != getIsBlack();
    } else {
        return target == nullptr;
    }
}