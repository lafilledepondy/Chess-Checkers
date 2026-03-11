#include "pawn.hpp"

Pawn::Pawn(bool isBlack):Piece(isBlack, "pawn") {}

bool Pawn::isValidMove(const Position &start_pos, const Position &end_pos, 
    bool isCapture, Plateau* board) const {
    // TODO
    return true;
}