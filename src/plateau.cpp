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

int Plateau::getHeight() const {
    return plateau_vec.size();
}

int Plateau::getWidth() const {
    return plateau_vec[0].size();
}

bool Plateau::isInside(const Position &pos) const {
    return pos.getX() > 0 && pos.getX() <= getWidth() &&
            pos.getY() > 0 && pos.getY() <= getHeight() ;
}

Piece * Plateau::getPiece(const Position &pos) {
    if (!isInside(pos)) {
        throw Erreur(1, "Outside the grid of board", 3);
    }
    return plateau_vec[pos.getY() - 1][pos.getX() - 1];    
}

void Plateau::play(const Position &start_pos, const Position &end_pos, bool turnBlack) {
    // TODO
}

void Plateau::addPiece(Piece * pi, const Position &pos){
    // TODO
}

void Plateau::movePiece(const Position &start_pos, const Position &end_pos) {
    // TODO
}
