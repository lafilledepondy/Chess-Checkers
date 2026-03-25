// ==============================================================================
// main.cpp
// ==============================================================================

#include <iostream>
#include <sstream>
#include <string>

#include "checkerboard.hpp"
#include "helperFunctions.hpp"


int main() {

    for(int i = 0; i < 100; ++i) {
        std::cout << "\n";
    }    
    std::cout << "+==============================================================+" << std::endl;
    std::cout << "|                       CHESS & CHECKERS                       |" << std::endl;
    std::cout << "+==============================================================+" << std::endl;

    HelperFunctions::chessMain();

    // Checkerboard cb;

    // cb.initialConditions();
    // cb.play(Position("A2"), Position("A4"), true);
    
    // std::cout << cb.toString() << std::endl;
    // std::cout << cb.toUnicodeString() << std::endl;

    return 0;
}
