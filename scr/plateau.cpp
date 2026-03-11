// ==============================================================================
// PLATEAU : 
// ==============================================================================

#include "plateau.hpp"

Plateau::Plateau(int height, int width) {
    plateau_vec.resize(height);

    for (int i_height=0; i_height < height; i_height++) {
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
    // TODO

    // if (plateau_vec[pos.getX()] == nullptr && plateau_vec[pos.getY()] == nullptr) {
    //     return false; 
    // }

    return true; 
}

Piece * Plateau::getPiece(const Position &pos) {
    // TODO
}

void Plateau::turn(const Position &start_pos, const Position &end_pos, bool turnBlack) {
    // TODO
}

void Plateau::addPiece(Piece * pi, const Position &pos){
    // TODO
}

void Plateau::movePiece(const Position &start_pos, const Position &end_pos) {
    // TODO
}
