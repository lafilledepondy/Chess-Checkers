#include "damier.hpp"

Damier::Damier():Plateau(10, 10) {}

void Damier::initialConditions() {

}

void Damier::play(const Position &start_pos, const Position &end_pos, bool turnBlack){

}

bool Damier::undoLastMove() {

}

bool Damier::canUndo() const {
    return _movesHistory.canUndo();
}

std::string Damier::toString() const {

}

std::string Damier::toUnicodeString() const {

}

std::vector<Damier::ReplayMove> Damier::readMovesFromFile(const std::string& filename) const {

}

void Damier::loadFromFile(const std::string& filename) {

}

void Damier::saveToFile(const std::string& filename) const {
    // replay move by move 
    // for (const ReplayMove& move : readMovesFromFile(filename)) {
    //     play(move.from, move.to, move.turnBlack);
    // }
}
