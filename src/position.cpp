// ==============================================================================
// POSITION : position in the checkerboard. Contain x and y.
// ==============================================================================

#include "position.hpp"

Position::Position(int x, int y) {
    this->_x = x;
    this->_y = y;
}

Position::Position(const Position &p, int dx, int dy) {
    this->_x = p._x + dx;
    this->_y = p._y + dy;
}

Position::Position(std::string str) {

    this->_x = str[0] - 'A' + 1;
    this->_y = str[1] - '0';
}

int Position::getX() const {
    return this->_x;
}

int Position::getY() const {
    return this->_y;
}

std::string Position::toString() const {
    std::ostringstream oss;
    oss << "[";
    oss << char(this->_x + 'A' - 1);
    oss << std::to_string(this->_y);
    oss << "]";
    return oss.str();
}