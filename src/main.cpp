// ==============================================================================
// main.cpp
// ==============================================================================

#include <iostream>
#include <chrono>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "checkerboard.hpp"

void replaySavedGame(const std::string& filePath) {
    Checkerboard replayBoard;
    replayBoard.initialConditions();

    const std::vector<Checkerboard::ReplayMove> moves = replayBoard.readMovesFromFile(filePath);
    if (moves.empty()) {
        std::cout << "No replayable move found in: " << filePath << "\n";
        return;
    }

    std::size_t currentMove = 0;
    std::string command;

    std::cout << "\nReplay controls: n/?=next, p=previous, a=autoplay, q=quit\n";
    while (true) {
        std::cout << replayBoard.toUnicodeString() << "\n";
        std::cout << "Replay move " << currentMove << "/" << moves.size() << " > ";
        std::getline(std::cin, command);

        if (command == "q") {
            break;
        }

        if (command == "n" || command == "?") {
            if (currentMove >= moves.size()) {
                std::cout << "End of replay reached.\n";
                continue;
            }

            const Checkerboard::ReplayMove& move = moves[currentMove];
            try {
                replayBoard.play(move.from, move.to, move.turnBlack);
                ++currentMove;
            }
            catch (const std::exception& e) {
                std::cout << "Replay stopped on invalid move: " << e.what() << "\n";
                break;
            }
            continue;
        }

        if (command == "p") {
            if (currentMove == 0 || !replayBoard.undoLastMove()) {
                std::cout << "Already at replay start.\n";
            } else {
                --currentMove;
            }
            continue;
        }

        if (command == "a") {
            while (currentMove < moves.size()) {
                const Checkerboard::ReplayMove& move = moves[currentMove];
                try {
                    replayBoard.play(move.from, move.to, move.turnBlack);
                    ++currentMove;
                }
                catch (const std::exception& e) {
                    std::cout << "Replay stopped on invalid move: " << e.what() << "\n";
                    break;
                }

                std::cout << replayBoard.toUnicodeString() << "\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(1200));
            }

            if (currentMove >= moves.size()) {
                std::cout << "Replay completed.\n";
            }
            continue;
        }

        std::cout << "Unknown replay command. Use n/?/p/a/q.\n";
    }
}

void chessMain() {
    Checkerboard cb;
    cb.initialConditions();

    std::string input;
    bool turnBlack = false; // white starts

    while (true) {
        std::cout << cb.toUnicodeString() << std::endl;
        std::cout << (turnBlack ? "Black" : "White")
                  << " move (D2 D4 | undo | save <file> | replay <file> | quit):\n> ";
        std::getline(std::cin, input);

        if (input == "quit") {
            break;
        }

        if (input.rfind("save ", 0) == 0 && input.size() > 5) {
            const std::string filePath = input.substr(5);
            cb.saveToFile(filePath);
            std::cout << "~~~Game saved to: " << filePath << "\n\n";
            continue;
        }

        if (input.rfind("replay ", 0) == 0 && input.size() > 7) {
            const std::string filePath = input.substr(7);
            replaySavedGame(filePath);
            continue;
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

int main() {

    for(int i = 0; i < 100; ++i) {
        std::cout << "\n";
    }    
    std::cout << "+==============================================================+" << std::endl;
    std::cout << "|                       CHESS & CHECKERS                       |" << std::endl;
    std::cout << "+==============================================================+" << std::endl;

    chessMain();

    // Checkerboard cb;

    // cb.initialConditions();
    // cb.play(Position("A2"), Position("A4"), true);
    
    // std::cout << cb.toString() << std::endl;
    // std::cout << cb.toUnicodeString() << std::endl;

    return 0;
}
