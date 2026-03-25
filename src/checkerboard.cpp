#include "checkerboard.hpp"

#include <cctype>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>

namespace {
bool getFirstMoveFlag(const Piece* piece) {
    if (const Pawn* pawn = dynamic_cast<const Pawn*>(piece)) return pawn->firstMove;
    if (const Rook* rook = dynamic_cast<const Rook*>(piece)) return rook->firstMove;
    if (const King* king = dynamic_cast<const King*>(piece)) return king->firstMove;
    return false;
}

void setFirstMoveFlag(Piece* piece, bool value) {
    if (Pawn* pawn = dynamic_cast<Pawn*>(piece)) pawn->firstMove = value;
    if (Rook* rook = dynamic_cast<Rook*>(piece)) rook->firstMove = value;
    if (King* king = dynamic_cast<King*>(piece)) king->firstMove = value;
}
}

/*
x <>
y ^
        0 1  2  3  4  5  6  7  8           
    0     
    1   8 Ro Ho Bi Qu Ki Bi Ho Ro       <> BLACK
    2   7 Pa Pa Pa Pa Pa Pa Pa Pa
    3   6 . . . . . . . . . . . .
    4   5 . . . . . . . . . . . .
    5   4 . . . . . . . . . . . .
    6   3 . . . . . . . . . . . .
    7   2 pa pa pa pa pa pa pa pa       <> WHITE
    8   1 ro ho bi qu ki bi ho ro
           A  B  C  D  E  F  G  H
*/

Checkerboard::Checkerboard():Plateau(8, 8) {}

void Checkerboard::play(const Position &start_pos, const Position &end_pos, bool turnBlack) {
    Piece* movedPieceBefore = getPiece(start_pos);
    Piece* capturedPieceBefore = getPiece(end_pos);
    const bool movedPieceWasFirstMove = getFirstMoveFlag(movedPieceBefore);

    Plateau::play(start_pos, end_pos, turnBlack);

    MoveRecord record {
        turnBlack,
        start_pos,
        end_pos,
        movedPieceBefore,
        capturedPieceBefore,
        movedPieceWasFirstMove,
        false,
        nullptr
    };

    _movesHistory.push(record);
}

bool Checkerboard::canUndo() const {
    return _movesHistory.canUndo();
}

bool Checkerboard::undoLastMove() {
    if (!_movesHistory.canUndo()) return false;

    MoveRecord move = _movesHistory.pop();
    Piece* movedPiece = getPiece(move.to);
    if (movedPiece == nullptr) return false;

    movePiece(move.to, move.from);

    if (King* king = dynamic_cast<King*>(getPiece(move.from))) {
        if (move.from.getY() == move.to.getY() && std::abs(move.to.getX() - move.from.getX()) == 2) {
            const int direction = (move.to.getX() > move.from.getX()) ? 1 : -1;
            const Position rookFrom(move.from.getX() + direction, move.from.getY());
            const Position rookTo(direction > 0 ? getWidth() : 1, move.from.getY());

            Piece* rookPiece = getPiece(rookFrom);
            movePiece(rookFrom, rookTo);
            setFirstMoveFlag(rookPiece, true);
        }
        setFirstMoveFlag(king, move.movedPieceWasFirstMove);
    } else {
        setFirstMoveFlag(getPiece(move.from), move.movedPieceWasFirstMove);
    }

    addPiece(move.capturedPiece, move.to);

    if (move.wasPromotion && move.promotionOldPiece != nullptr) {
        addPiece(move.promotionOldPiece, move.from);
    }

    return true;
}
       
void Checkerboard::initialConditions() { 
    // PAWN    
    // ---black
    addPiece(new Pawn(true), Position(1, 2));
    addPiece(new Pawn(true), Position(2, 2));    
    addPiece(new Pawn(true), Position(3, 2));    
    addPiece(new Pawn(true), Position(4, 2));    
    addPiece(new Pawn(true), Position(5, 2));    
    addPiece(new Pawn(true), Position(6, 2));    
    addPiece(new Pawn(true), Position(7, 2));    
    addPiece(new Pawn(true), Position(8, 2));   
    // ---white
    addPiece(new Pawn(false), Position(1, 7));
    addPiece(new Pawn(false), Position(2, 7));    
    addPiece(new Pawn(false), Position(3, 7));    
    addPiece(new Pawn(false), Position(4, 7));    
    addPiece(new Pawn(false), Position(5, 7));    
    addPiece(new Pawn(false), Position(6, 7));    
    addPiece(new Pawn(false), Position(7, 7));    
    addPiece(new Pawn(false), Position(8, 7));    

    // OTHER PIECES   
    // ---black
    addPiece(new Rook(true), Position(1, 1));
    addPiece(new Cavalier(true), Position(2, 1));    
    addPiece(new Bishop(true), Position(3, 1));    
    addPiece(new Queen(true), Position(4, 1));    
    addPiece(new King(true), Position(5, 1));    
    addPiece(new Bishop(true), Position(6, 1));    
    addPiece(new Cavalier(true), Position(7, 1));    
    addPiece(new Rook(true), Position(8, 1)); 
    // // ---white
    addPiece(new Rook(false), Position(1, 8));
    addPiece(new Cavalier(false), Position(2, 8));    
    addPiece(new Bishop(false), Position(3, 8));    
    addPiece(new Queen(false), Position(4, 8));    
    addPiece(new King(false), Position(5, 8));    
    addPiece(new Bishop(false), Position(6, 8));    
    addPiece(new Cavalier(false), Position(7, 8));    
    addPiece(new Rook(false), Position(8, 8));     
}

std::string Checkerboard::toString() const {
    std::ostringstream oss;
    oss << "\n" ;
    for (int x = 0; x <= getWidth(); x++) {
        for (int y = 0; y <= getHeight(); y++) {
            if (x==0 && y==0) {
                oss << " ";
            }
            else if (x!=0 && y==0) { // [*][0]
                oss << std::to_string(-(x-9)) << " ";
            }
            else if (x==0 && y!=0) { /// [0][*]
                oss << "  " ;
            }
            else if (plateau_vec[x][y] != nullptr) {
                oss << plateau_vec[x][y]->toString() << " ";
            }
            else {
                oss << " . ";
            }
        }
        oss << "\n" ;
    }
    oss << "  A  B  C  D  E  F  G  H";
    return oss.str();
}

std::string Checkerboard::toUnicodeString() const {
    std::ostringstream oss;
    oss << "\n";
    for (int x = 0; x <= getWidth(); x++) {
        for (int y = 0; y <= getHeight(); y++) {
            if (x == 0 && y == 0) {
                oss << "  ";
            }
            else if (x != 0 && y == 0) {
                oss << std::to_string(-(x-9)) << " ";
            }
            else if (x == 0 && y != 0) {
                oss << "  " ;
            }
            else if (plateau_vec[x][y] != nullptr) {
                oss << " " << plateau_vec[x][y]->toUnicodeString() << " ";
            }
            else {
                oss << " . ";
            }
        }
        oss << "\n";
    }
    oss << "   A  B  C  D  E  F  G  H";
    return oss.str();
}

void Checkerboard::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return;
    }

    MovesPile history = _movesHistory;
    std::vector<MoveRecord> temp;

    // stack --> vector (reverse order)
    while (!history.empty()) {
        temp.push_back(history.top());
        history.pop();
    }

    // lambda function strPosition converts Position to E2, D4
    auto strPosition = [](const Position& pos) {
        std::string s = pos.toString();
        if (s.size() >= 2 && s.front() == '[' && s.back() == ']') {
            s = s.substr(1, s.size() - 2);
        }
        return s;
    };

    // lambda function pieceName casting
    auto pieceName = [](const Piece* piece) -> std::string {
        if (dynamic_cast<const Pawn*>(piece) != nullptr) return "PAWN";
        if (dynamic_cast<const Rook*>(piece) != nullptr) return "ROOK";
        if (dynamic_cast<const Cavalier*>(piece) != nullptr) return "CAVALIER";
        if (dynamic_cast<const Bishop*>(piece) != nullptr) return "BISHOP";
        if (dynamic_cast<const Queen*>(piece) != nullptr) return "QUEEN";
        if (dynamic_cast<const King*>(piece) != nullptr) return "KING";
        return "UNKNOWN";
    };

    // write like WHITE,PAWN,E2,E4
    for (auto it = temp.rbegin(); it != temp.rend(); ++it) {
        file << (it->turnBlack ? "BLACK" : "WHITE") << ","
            << pieceName(it->movedPiece) << ","
            << strPosition(it->from) << ","
            << strPosition(it->to) << "\n";
    }
}

std::vector<Checkerboard::ReplayMove> Checkerboard::readMovesFromFile(const std::string& filename) const {
    std::vector<ReplayMove> moves;
    std::ifstream file(filename);
    if (!file.is_open()) {
        return moves;
    }

    std::string line;

    // trim removes trailing whitespace 
    // add carriage return characters
    auto trim = [](std::string& value) {
        while (!value.empty() && (value.back() == '\r' || value.back() == ' ' || value.back() == '\t')) {
            value.pop_back();
        }
    };

    auto upper = [](std::string value) {
        for (char& c : value) {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        }
        return value;
    };

    while (std::getline(file, line)) {
        trim(line);
        if (line.empty()) {
            continue;
        }

        std::istringstream iss(line);
        std::vector<std::string> fields;
        std::string field;

        while (std::getline(iss, field, ',')) {
            trim(field);
            fields.push_back(field);
        }

        std::string turn;
        std::string from;
        std::string to;

        if (fields.size() == 3) {
            turn = fields[0];
            from = fields[1];
            to = fields[2];
        } else if (fields.size() == 4) {
            turn = fields[0];
            from = fields[2];
            to = fields[3];
        } else {
            continue;
        }

        if (turn.empty() || from.empty() || to.empty()) {
            continue;
        }

        try {
            moves.push_back({
                upper(turn) == "BLACK",
                Position(from),
                Position(to)
            });
        }
        catch (const std::exception&) {
            // Ignore malformed lines and continue parsing the rest of the replay file.
            continue;
        }
    }

    return moves;
}

void Checkerboard::loadFromFile(const std::string& filename) {
    for (const ReplayMove& move : readMovesFromFile(filename)) {
        play(move.from, move.to, move.turnBlack);
    }
}
