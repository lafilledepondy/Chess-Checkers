#include "piece.hpp"

Piece::Piece(bool isBlack, std::string name) : _isBlack(isBlack), _name(name) {}

bool Piece::getIsBlack() const {
    return _isBlack;
}

std::string Piece::toString() const {
    std::ostringstream oss;
    std::string str = _name.substr(0, 2);    

    if (this->_isBlack) {
        str[0] = std::toupper(str[0]);
        oss << str.substr(0, 2);
    } else {
        str[0] = std::tolower(str[0]);
        oss << str.substr(0, 2);
    }
    return oss.str();    
}

std::vector<Position> Piece::getValidMoves(const Position& start, const Plateau* plateau) const {
    std::vector<Position> possibleMoves_vec;

    for (int x=0; x< plateau->getWidth(); x++) {
        for (int y=0; y< plateau->getWidth(); y++) {
            Position end(x, y); 
            bool isCapture = (plateau->getPiece(end) != nullptr);

            if (!(end == start) && isValidMove(start, end, isCapture, const_cast<Plateau*>(plateau))) {
                possibleMoves_vec.push_back(end);
            }
        }
    }
    return possibleMoves_vec;
}