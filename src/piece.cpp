#include "piece.hpp"

Piece::Piece(bool isBlack, std::string name) {
    this->_isBlack = isBlack;
    this->_name = name;
}

std::string Piece::toString() const {
    std::ostringstream oss;
    if (this->_isBlack) {
        std::string str = this->_name;
        str[0] = toupper(str[0]);
        oss << str.substr(0, 2);;
    } else {
        oss << this->_name.substr(0, 2);;
    }
    return oss.str();    
}