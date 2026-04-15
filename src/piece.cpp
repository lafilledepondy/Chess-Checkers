#include "piece.hpp"

Piece::Piece(bool isBlack, std::string name) : _isBlack(isBlack), _name(name) {}

bool Piece::getIsBlack() const {
    return _isBlack;
}

std::string Piece::toString() const {
    std::ostringstream oss;
    std::string str = _name.substr(0, 2); // first 2 letters   

    if (this->_isBlack) {
        str[0] = std::toupper(str[0]);
        oss << str;//.substr(0, 2); 
    } else {
        str[0] = std::tolower(str[0]);
        oss << str;//.substr(0, 2); 
    }
    return oss.str();    
}

std::string Piece::toUnicodeString() const {
    if (_name == "pawn") {return _isBlack ? "♟" : "♙";}
    if (_name == "rook") {return _isBlack ? "♜" : "♖";}
    if (_name == "cavalier") {return _isBlack ? "♞" : "♘";}
    if (_name == "bishop") {return _isBlack ? "♝" : "♗";}
    if (_name == "queen") {return _isBlack ? "♛" : "♕";}
    if (_name == "king") {return _isBlack ? "♚" : "♔";}
    if (_name == "pion") {return _isBlack ? "●" : "○";}
    if (_name == "dame") {return _isBlack ? "⛃" : "⛁";}    
    // unknown piece
    return "?";
}

std::vector<Position> Piece::getValidMoves(const Position& start, const Plateau* plateau) const {
    std::vector<Position> possibleMoves_vec;
    // 2D table 
    for (int x = 1; x <= plateau->getWidth(); x++) {
        for (int y = 1; y <= plateau->getHeight(); y++) {
            Position end(x, y); 
            bool isCapture = (plateau->getPiece(end) != nullptr);
            // check is valid move and not the same position
            if (!(end == start) && isValidMove(start, end, isCapture, const_cast<Plateau*>(plateau))) {
                possibleMoves_vec.push_back(end);
            }
        }
    }
    // return a 1D<Position> vector containing all possible moves for this piece from the start position
    return possibleMoves_vec;
}