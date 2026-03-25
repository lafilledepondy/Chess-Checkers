// ==============================================================================
// PLATEAU : 
// ==============================================================================

#include <cmath>

#include "plateau.hpp"
#include "exception.hpp"
#include "pawn.hpp"
#include "rook.hpp"
#include "king.hpp"

Plateau::Plateau(int height, int width) {
    plateau_vec.resize(height + 1);

    for (int i_height=0; i_height<=height; i_height++) {
        plateau_vec[i_height].resize(width + 1, nullptr);
    }
}

void Plateau::movePiece(const Position &start_pos, const Position &end_pos) {
    Piece *piece_start = getPiece(start_pos); 

    plateau_vec[end_pos.getY()][end_pos.getX()] = piece_start;
    plateau_vec[start_pos.getY()][start_pos.getX()] = nullptr;
}

void Plateau::addPiece(Piece * pi, const Position &pos){
    plateau_vec[pos.getY()][pos.getX()] = pi;
}

int Plateau::getHeight() const {
    return plateau_vec.size() - 1;
}

int Plateau::getWidth() const {
    if (plateau_vec.empty()) {
        return 0;
    }
    return plateau_vec[0].size() - 1;
}

bool Plateau::isInside(const Position &pos) const {
    return pos.getX() > 0 && pos.getX() <= getWidth() &&
            pos.getY() > 0 && pos.getY() <= getHeight() ;
}

Piece * Plateau::getPiece(const Position &pos) const {
    if (!isInside(pos)) {
        throw InvalidMoveException(1, "Outside of the board.", 3);
    }
    return plateau_vec[pos.getY()][pos.getX()];
}

void Plateau::play(const Position &start_pos, const Position &end_pos, bool turnBlack) {
    Piece *piece_start = getPiece(start_pos); 
    Piece *piece_end = getPiece(end_pos); 

    if (piece_start == nullptr) {
        throw InvalidMoveException(2, "`start_pos` is empty.", 3);
    }
    if (piece_start->getIsBlack() != turnBlack) {
        throw InvalidMoveException(3, "`start_pos` is another color.", 3);
    }
    if (piece_end != nullptr && piece_start->getIsBlack() == piece_end->getIsBlack()) {
        throw InvalidMoveException(4, "`end_pos` has my color.", 3);
    }
    bool isCapture = (piece_end != nullptr);   
    if (!piece_start->isValidMove(start_pos, end_pos, isCapture, this)) {
        throw InvalidMoveException(5, "invalid move by the piece.", 2);
    }

    // Detect castling before moving pieces so we can move rook too
    bool isCastling = false;
    Position rookStart(1, 1);
    Position rookEnd(1, 1);
    Rook* castlingRook = nullptr;

    King* movingKing = dynamic_cast<King*>(piece_start);
    if (movingKing != nullptr && start_pos.getY() == end_pos.getY() && std::abs(end_pos.getX() - start_pos.getX()) == 2) {
        const int direction = (end_pos.getX() > start_pos.getX()) ? 1 : -1;
        rookStart = Position(direction > 0 ? getWidth() : 1, start_pos.getY());
        rookEnd = Position(start_pos.getX() + direction, start_pos.getY());

        Piece* rookPiece = getPiece(rookStart);
        castlingRook = dynamic_cast<Rook*>(rookPiece);
        if (castlingRook != nullptr && castlingRook->getIsBlack() == piece_start->getIsBlack()) {
            isCastling = true;
        }
    }

    movePiece(start_pos, end_pos);

    if (isCastling) {
        movePiece(rookStart, rookEnd);
        castlingRook->firstMove = false;
    }

    if (Pawn* pawn = dynamic_cast<Pawn*>(piece_start)) {
        pawn->firstMove = false;
    }
    if (Rook* rook = dynamic_cast<Rook*>(piece_start)) {
        rook->firstMove = false;
    }
    if (King* king = dynamic_cast<King*>(piece_start)) {
        king->firstMove = false;
    }
}
