// ==============================================================================
// POSITION : position in the checkerboard. Contain x and y.
// ==============================================================================

#include "position.hpp"

#include <cctype>
#include <stdexcept>

Position::Position(int x, int y) {
    this->_x = x;
    this->_y = y;
}

Position::Position(const Position &p, int dx, int dy) {
    this->_x = p._x + dx;
    this->_y = p._y + dy;
}

Position::Position(std::string str) {
    std::size_t i = 0;
    int x = 0;

    while (i < str.size() && isalpha(str[i])) {
        char c = toupper(str[i]);
        x = x * 26 + (c - 'A' + 1);
        i++;
    }

    int y = 0;
    while (i < str.size() && isdigit(str[i])) {
        y = y * 10 + (str[i] - '0');
        i++;
    }

    this->_x = x;
    this->_y = y;
}

int Position::getX() const {
    return this->_x;
}

int Position::getY() const {
    return this->_y;
}

std::string Position::toString() const {
    std::ostringstream oss;
    int x = this->_x;
    std::string column;

    while (x > 0) {
        x -= 1;
        column.insert(column.begin(), static_cast<char>('A' + (x % 26)));
        x /= 26;
    }

    oss << "[";
    oss << column;
    oss << std::to_string(this->_y);
    oss << "]";
    return oss.str();
}