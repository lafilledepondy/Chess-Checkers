#include "pawn.hpp"

Pawn::Pawn(bool isBlack):Piece(isBlack, "pawn") {}

bool Pawn::isValidMove(const Position &start_pos, const Position &end_pos, bool isCapture, Plateau* board) const {
    const int direction = getIsBlack() ? 1 : -1; // black: move down ; white: move up
    const int dx = end_pos.getX() - start_pos.getX();
    const int dy = end_pos.getY() - start_pos.getY();
    Piece* target = board->getPiece(end_pos);
    
    // forward
    if (dx == 0) {
        // cannot move forward into a piece
        if (target != nullptr) return false;

        // 1 step forward
        if (dy == direction) return true;

        // 2 steps forward first move only
        if (firstMove && dy == 2 * direction) {
            Position mid(start_pos.getX(), start_pos.getY() + direction);

            // pos+1 must be nullptr empty
            if (board->getPiece(mid) == nullptr) {
                return true;
            }
        }

        return false;
    }

    // capture
    if (std::abs(dx) == 1 && dy == direction) {

        // normal capture
        if (target != nullptr && target->getIsBlack() != getIsBlack()) {
            return true;
        }

        // TODO: en passant is it coded?
        // en passant requires last move tracking in Plateau
        // if (target == nullptr) {
        //     Position side(end_pos.getX(), start_pos.getY());
        //     Piece* sidePawn = board->getPiece(side);

        //     if (sidePawn != nullptr &&
        //         sidePawn->getName() == "pawn" &&
        //         sidePawn->getIsBlack() != getIsBlack() &&
        //         board->wasLastMoveDoubleStep(side)) {

        //         return true;
        //     }
        // }
    }


    return false;
}