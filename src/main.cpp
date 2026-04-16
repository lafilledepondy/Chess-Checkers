// ==============================================================================
// main.cpp
// ==============================================================================

#include <iostream>
#include <limits>
#include <chrono>
#include <cmath>
#include <sstream>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>

#include "checkerboard.hpp"
#include "damier.hpp"
#include "pawn.hpp"
#include "pion.hpp"

namespace {
    constexpr const char* ANSI_RESET = "\x1b[0m";
    constexpr const char* ANSI_LIGHT_SQUARE = "\x1b[48;5;180m";
    constexpr const char* ANSI_DARK_SQUARE = "\x1b[48;5;94m";
    constexpr const char* ANSI_HINT_SQUARE = "\x1b[48;5;34m";
    constexpr const char* ANSI_WHITE_PIECE = "\x1b[38;5;231m";
    constexpr const char* ANSI_BLACK_PIECE = "\x1b[38;5;16m";

    std::string baseSquareBackground(int row, int col) {
        return ((row + col) % 2 == 0) ? ANSI_DARK_SQUARE : ANSI_LIGHT_SQUARE;
    }

    bool containsPosition(const std::vector<Position>& positions, const Position& target) {
        for (const Position& pos : positions) {
            if (pos == target) {
                return true;
            }
        }
        return false;
    }

    template <typename BoardType>
    std::vector<Position> collectHintMoves(const BoardType& board, const Position& start_pos, bool turnBlack) {
        std::vector<Position> moves;
        Piece* selected = board.getPiece(start_pos);

        if (selected == nullptr) {
            return moves;
        }

        for (int y = 1; y <= board.getHeight(); ++y) {
            for (int x = 1; x <= board.getWidth(); ++x) {
                Position end_pos(x, y);
                if (end_pos == start_pos) {
                    continue;
                }

                Piece* target = board.getPiece(end_pos);
                if (target != nullptr && target->getIsBlack() == selected->getIsBlack()) {
                    continue;
                }

                const bool isCapture = (target != nullptr);
                if (selected->isValidMove(start_pos, end_pos, isCapture, const_cast<BoardType*>(&board))) {
                    moves.push_back(end_pos);
                }
            }
        }

        (void)turnBlack;
        return moves;
    }

    template <typename BoardType>
    bool isCaptureHintSquare(const BoardType& board, const Position& start_pos, const Position& end_pos) {
        Piece* selected = board.getPiece(start_pos);
        if (selected == nullptr) {
            return false;
        }

        Piece* target = board.getPiece(end_pos);
        if (target != nullptr && target->getIsBlack() != selected->getIsBlack()) {
            return true;
        }

        // en passant pos_end empty
        if (dynamic_cast<const Pawn*>(selected) != nullptr) {
            const int dx = std::abs(end_pos.getX() - start_pos.getX());
            const int dy = std::abs(end_pos.getY() - start_pos.getY());
            if (dx == 1 && dy == 1 && board.canEnPassantCapture(start_pos, end_pos)) {
                return true;
            }
        }

        // checkers pawn jump capture end_pos
        if (dynamic_cast<const Pion*>(selected) != nullptr) {
            const int dx = std::abs(end_pos.getX() - start_pos.getX());
            const int dy = std::abs(end_pos.getY() - start_pos.getY());
            if (dx == 2 && dy == 2) {
                return true;
            }
        }

        return false;
    }

    template <typename BoardType>
    std::string renderHintBoard(const BoardType& board,
                                const Position& start_pos,
                                const std::vector<Position>& hintMoves) {
        std::ostringstream oss;
        oss << "\n";

        for (int row = 0; row <= board.getHeight(); ++row) {
            for (int col = 0; col <= board.getWidth(); ++col) {
                if (row == 0 && col == 0) {
                    oss << "   ";
                    continue;
                }
                if (row > 0 && row <10&& col == 0 ) {
                    oss << std::to_string(row) << "  ";
                    continue;
                } else if (row==10 && col==0) {
                    oss << std::to_string(row) << " ";
                    continue;
                }
                if (row == 0 && col > 0) {
                    oss << " " << char('A' + col - 1) << " ";
                    continue;
                }

                const Position current(col, row);
                const bool isHint = containsPosition(hintMoves, current);
                const std::string background = isHint ? ANSI_HINT_SQUARE : baseSquareBackground(row, col);

                if (isHint) {
                    const char marker = isCaptureHintSquare(board, start_pos, current) ? 'x' : '.';
                    oss << background << " " << marker << " " << ANSI_RESET;
                    continue;
                }

                Piece* piece = board.getPiece(current);
                if (piece != nullptr) {
                    const char* pieceColor = piece->getIsBlack() ? ANSI_BLACK_PIECE : ANSI_WHITE_PIECE;
                    oss << background << pieceColor << " " << piece->toUnicodeString() << " " << ANSI_RESET;
                } else {
                    oss << background << "   " << ANSI_RESET;
                }
            }
            oss << "\n";
        }

        return oss.str();
    }
}

template <typename BoardType> // used so that the pseudoMain is common for Chess and jeuDD

void replaySavedGame(const std::string& filePath) {
    BoardType replayBoard;
    replayBoard.initialConditions();

    // 
    const std::vector<typename BoardType::ReplayMove> moves = replayBoard.readMovesFromFile(filePath);
    if (moves.empty()) {
        std::cout << "No replayable move found in: " << filePath << "\n";
        return;
    }

    std::size_t currentMove = 0;
    std::string command;

    std::cout << "\nReplay controls: n=next, p=previous, a=autoplay, q=quit\n";
    while (true) {
        std::cout << replayBoard.toUnicodeString() << "\n";
        std::cout << "Replay move " << currentMove << "/" << moves.size() << " > ";
        std::getline(std::cin, command);

        if (command == "q") {break;} // quit

        if (command == "n") {
            // replay ended && n
            if (currentMove >= moves.size()) {
                std::cout << "End of replay reached.\n";
                continue;
            }
            // play next move
            const typename BoardType::ReplayMove& move = moves[currentMove];
            try {
                replayBoard.play(move.from, move.to, move.turnBlack);
                ++currentMove;
            }
            // error handling for invalid move in the replay file
            catch (const std::exception& e) {
                std::cout << "Replay stopped on invalid move: " << e.what() << "\n";
                break;
            }
            continue;
        }

        if (command == "p") { // previous move
            if (currentMove == 0 || !replayBoard.undoLastMove()) {std::cout << "Already at replay start.\n";} 
            else {--currentMove;}
            continue;
        }

        if (command == "a") { // autoplay
            while (currentMove < moves.size()) {
                const typename BoardType::ReplayMove& move = moves[currentMove];
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

            // replay ended
            if (currentMove >= moves.size()) {std::cout << "Replay completed.\n";}
            continue;
        }

        std::cout << "Unknown replay command. Use n/p/a/q.\n"; // n=next p=previous a=autoplay q=quit
    }
}

template <typename BoardType>
void pseudoMain(BoardType& cb) {
    cb.initialConditions();

    std::string input; 
    bool turnBlack = false; // white starts

    while (true) {
        std::cout << "             Scoreboard\n";
        std::cout << "White score : " << cb.getScore(false) ; 
        std::cout << "     Black score : " << cb.getScore(true) << "\n";
        std::cout << cb.toUnicodeString() << std::endl;
        std::cout << (turnBlack ? "Black" : "White") // options to chose for x player
              << " move (B7 B5 | hint <square> | undo | save <file> | replay <file> | quit):\n> ";
        std::getline(std::cin, input);

        if (input == "quit") {break;} // quit => exit the game loop

        if (input.rfind("save ", 0) == 0 && input.size() > 5) { // save <file>
            const std::string filePath = input.substr(5);
            cb.saveToFile(filePath); // call the save function of the checkerboard
            std::cout << "~~~Game saved to: " << filePath << "\n\n";
            continue;
        }

        if (input.rfind("replay ", 0) == 0 && input.size() > 7) { // replay <file>
            const std::string filePath = input.substr(7);
            replaySavedGame<BoardType>(filePath); // call the replay function (defined above)
            continue;
        }

        if (input == "undo") { // undo => call the undo function of the checkerboard
            if (cb.undoLastMove()) {
                turnBlack = !turnBlack;
                std::cout << "~~~Last move undone\n\n";
            } else {
                std::cout << "~~~No move to undo\n\n";
            }
            continue;
        }

        if (input.rfind("hint ", 0) == 0 && input.size() > 5) { // hint <square>
            std::istringstream hintIss(input.substr(5));
            std::string startStr;
            hintIss >> startStr;

            if (startStr.empty()) {
                std::cout << "Invalid format. Use: hint B7\n";
                continue;
            }

            try {
                Position start(startStr);
                Piece* selected = cb.getPiece(start);

                if (selected == nullptr) {
                    std::cout << "No piece on " << startStr << "\n";
                    continue;
                }

                if (selected->getIsBlack() != turnBlack) {
                    std::cout << "Selected piece is not yours this turn.\n";
                    continue;
                }

                const std::vector<Position> hintMoves = collectHintMoves(cb, start, turnBlack);
                std::cout << "Show possible moves for " << startStr << ":\n";
                std::cout << renderHintBoard(cb, start, hintMoves) << "\n";
            }
            catch (const std::exception& e) {
                std::cout << "Invalid hint position: " << e.what() << "\n";
            }

            continue;
        }

        std::istringstream iss(input);
        std::string startStr, endStr;
        // error handling for the input format 
        if (!(iss >> startStr >> endStr)) {
            std::cout << "Invalid format. Use: `D6 D4`\n";
            continue;
        }

        try { // move => call the play function of the checkerboard
            Position start(startStr);
            Position end(endStr);

            cb.play(start, end, turnBlack);
            turnBlack = !turnBlack; // switch player

            if constexpr (std::is_same_v<BoardType, Checkerboard>) {
                if (cb.isCheckmate(turnBlack)) {
                    std::cout << cb.toUnicodeString() << std::endl;
                    std::cout << "CHECKMATE! "
                              << (turnBlack ? "White" : "Black")
                              << " wins.\n";
                    break;
                }

                if (cb.isStalemate(turnBlack)) {
                    std::cout << cb.toUnicodeString() << std::endl;
                    std::cout << "STALEMATE! The game is a draw.\n";
                    break;
                }
            }

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
    std::cout << "|                   CHESS & JEU DE DAMES                       |" << std::endl;
    std::cout << "+==============================================================+" << std::endl;

    std::string gameInput;
    while (true) {
        std::cout << "Choose a game (chess | dames | quit):\n> ";
        if (!(std::cin >> gameInput)) {
            break; 
        }
        if (gameInput == "chess") {
            std::cout << "=============== CHESS ===============" << std::endl;
            Checkerboard cb;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            pseudoMain(cb);
            break;
        }
        if (gameInput == "dames") {
            std::cout << "=========== JEU DE DAMES ============" << std::endl;
            Damier damier;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            pseudoMain(damier);
            break;
        }
        if (gameInput == "quit") {
            break;
        }
        std::cout << "Unknown choice. Type: chess, dames, or quit.\n";
    }

    // Checkerboard cb;
    // cb.initialConditions();

    // cb.play(Position("A2"), Position("A4"), true);
    
    // std::cout << cb.toString() << std::endl;
    // std::cout << cb.toUnicodeString() << std::endl;

    // Damier damier;
    // damier.initialConditions();

    // std::cout << damier.toString() << std::endl;
    // std::cout << damier.toUnicodeString() << std::endl;    

    return 0;
}