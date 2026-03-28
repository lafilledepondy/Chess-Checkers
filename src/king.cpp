#include "king.hpp"
#include "rook.hpp"

King::King(bool isBlack):Piece(isBlack, "king") {}

bool King::isValidMove(const Position &start_pos, const Position &end_pos, bool isCapture, Plateau* board) const {
    const int deltaX = end_pos.getX() - start_pos.getX();
    const int deltaY = end_pos.getY() - start_pos.getY();
    int dx = abs(deltaX);
    int dy = abs(deltaY);
    const Piece* target = board->getPiece(end_pos);

    // no move
    if (dx == 0 && dy == 0) return false;

    // castling only if king & rook never moved
    if (!isCapture && firstMove && dy == 0 && dx == 2) {
        const int direction = (deltaX > 0) ? 1 : -1;
        const int rookX = (direction > 0) ? board->getWidth() : 1;
        const Position rookPos(rookX, start_pos.getY());

        Piece* rookPiece = board->getPiece(rookPos);
        Rook* rook = dynamic_cast<Rook*>(rookPiece);
        // rook is present && rook is myColor && rook didn't move
        if (rook == nullptr || rook->getIsBlack() != getIsBlack() || !rook->firstMove) {
            return false;
        }

        // check path
        for (int x = start_pos.getX() + direction; x != rookX; x += direction) {
            if (board->getPiece(Position(x, start_pos.getY())) != nullptr) {
                return false;
            }
        }

        return true;
    }

    // king moves only 1 square
    if (dx > 1 || dy > 1) return false;

    if (isCapture) {
        // capture
        return target != nullptr && target->getIsBlack() != getIsBlack();
    } else {
        // move
        return target == nullptr;
    }
}
