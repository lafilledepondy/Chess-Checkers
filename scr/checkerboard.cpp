#include "checkerboard.hpp"


Checkerboard::Checkerboard():Plateau(8, 8) {}
       
void Checkerboard::initialesConditions() {
    // TODO 

    addPiece(new Pawn(true), Position(0, 0));
}

std::string Checkerboard::toString() const {
    // TODO 

    std::ostringstream oss;
    oss << "TODO";
    return oss.str();
}