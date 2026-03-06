// ==============================================================================
// POSITION : position in the checkerboard. Contain x and y.
// ==============================================================================

#include "position.hpp"

Position::Position(int x, int y) {
    this->_x = x;
    this->_y = y;
}

Position::Position(Position p, int dx, int dy) {
    this->_x = p._x + dx;
    this->_y = p._y + dy;
}

Position::Position(std::string str) {
    if (str[0] == 'A') this->_x = 1;
    else if (str[0] == 'B') this->_x = 2;
    else if (str[0] == 'C') this->_x = 3;
    else if (str[0] == 'D') this->_x = 4;
    else if (str[0] == 'E') this->_x = 5;
    else if (str[0] == 'F') this->_x = 6;
    else if (str[0] == 'G') this->_x = 7; 
    else if (str[0] == 'H') this->_x = 8;   

    this->_y = str[1] - '0';
}

std::string Position::toString() const {
    std::ostringstream oss;
    oss << "[";
    if (this->_x == 1) oss << "A";
    else if (this->_x == 2) oss << "B";
    else if (this->_x == 3) oss << "C";
    else if (this->_x == 4) oss << "D";
    else if (this->_x == 5) oss << "E";
    else if (this->_x == 6) oss << "F";
    else if (this->_x == 7) oss << "G";
    else if (this->_x == 8) oss << "H";

    oss << std::to_string(this->_y);
    oss << "]";
    return oss.str();
}