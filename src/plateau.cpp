// ==============================================================================
// PLATEAU : 
// ==============================================================================

#include "plateau.hpp"
#include "exception.hpp"

Plateau::Plateau(int height, int width) {
    plateau_vec.resize(height);

    for (int i_height=0; i_height<height; i_height++) {
        plateau_vec[i_height].resize(width, nullptr);
    }
}

void Plateau::movePiece(const Position &start_pos, const Position &end_pos) {
    Piece *piece_start = getPiece(start_pos); 

    plateau_vec[end_pos.getY() - 1][end_pos.getX() - 1] = piece_start;
    plateau_vec[start_pos.getY() - 1][start_pos.getX() - 1] = nullptr;
}

void Plateau::addPiece(Piece * pi, const Position &pos){
    plateau_vec[pos.getY() - 1][pos.getX() - 1] = pi;
}

int Plateau::getHeight() const {
    return plateau_vec.size();
}

int Plateau::getWidth() const {
    if (plateau_vec.empty()) {
        return 0;
    }
    return plateau_vec[0].size();
}

bool Plateau::isInside(const Position &pos) const {
    return pos.getX() > 0 && pos.getX() <= getWidth() &&
            pos.getY() > 0 && pos.getY() <= getHeight() ;
}

Piece * Plateau::getPiece(const Position &pos) const {
    if (!isInside(pos)) {
        throw InvalidMoveException(1, "Outside of the board.", 3);
    }
    return plateau_vec[pos.getY() - 1][pos.getX() - 1];    
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

    movePiece(start_pos, end_pos);
}
