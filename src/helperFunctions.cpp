#include "helperFunctions.hpp"
#include "checkerboard.hpp"

void HelperFunctions::chessMain() {
    Checkerboard cb;
    cb.initialConditions();

    std::string input;
    bool turnBlack = false; // white starts

    while (true) {
        std::cout << cb.toUnicodeString() << std::endl;
        std::cout << (turnBlack ? "Black" : "White") << " move: \n> ";
        std::getline(std::cin, input);

        if (input == "quit") {
            break;
        }
        if (input == "undo") {
            if (cb.undoLastMove()) {
                turnBlack = !turnBlack;
                std::cout << "~~~Last move undone\n\n";
            } else {
                std::cout << "~~~No move to undo\n\n";
            }
            continue;
        }

        std::istringstream iss(input);
        std::string startStr, endStr;

        if (!(iss >> startStr >> endStr)) {
            std::cout << "Invalid format. Use: `D6 D4`\n";
            continue;
        }

        try {
            Position start(startStr);
            Position end(endStr);

            cb.play(start, end, turnBlack);
            turnBlack = !turnBlack; // switch player
            // spacing from the previous player
            std::cout << "\n";
            std::cout << "\n";
        }
        catch (const std::exception& e) {
            std::cout << "Invalid move: " << e.what() << std::endl;
        }
    }
}