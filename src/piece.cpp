#include "piece.hpp"

#include <limits>

Piece::Piece(bool isBlack, std::string name) : _isBlack(isBlack), _name(name) {}

bool Piece::getIsBlack() const {
    return _isBlack;
}

int Piece::getScoreValue() const {
    if (_name == "pawn" || _name == "pion") {
        return 1;
    }
    if (_name == "cavalier" || _name == "bishop" || _name == "dame") {
        return 3;
    }
    if (_name == "rook") {
        return 5;
    }
    if (_name == "queen") {
        return 9;
    }
    if (_name == "king") {
        return std::numeric_limits<int>::max();
    }
    return 0;
}

bool Piece::isPriceless() const {
    return _name == "king";
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
    // since there is ANSI_BLACK_PIECE and ANSI_WHITE_PIECE definied in the checkerboard 
    // that is why we don't to define the color of the pieces here
    if (_name == "pawn") {return _isBlack ? "♟" : "♟";} //♙
    if (_name == "rook") {return _isBlack ? "♜" : "♜";} //♖
    if (_name == "cavalier") {return _isBlack ? "♞" : "♞";} //♘
    if (_name == "bishop") {return _isBlack ? "♝" : "♝";} //♗
    if (_name == "queen") {return _isBlack ? "♛" : "♛";} //♕
    if (_name == "king") {return _isBlack ? "♚" : "♚";} //♔
    if (_name == "pion") {return _isBlack ? "●" : "●";} //○
    if (_name == "dame") {return _isBlack ? "⛃" : "⛃";}    //⛁
    // unknown piece
    return "?";
}