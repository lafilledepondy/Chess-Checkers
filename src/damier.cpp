#include "damier.hpp"

#include <cctype>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>

Damier::Damier():Plateau(10, 10) {}

void Damier::promotePawnIfNeeded(const Position &end_pos, MoveRecord &record) {
    Piece* piece = getPiece(end_pos);
    Pion* pawn = dynamic_cast<Pion*>(piece);
    if (pawn == nullptr) {
        return;
    }
    // last row
    const bool reachesLastRow = (pawn->getIsBlack() && end_pos.getY() == getHeight()) ||
                                (!pawn->getIsBlack() && end_pos.getY() == 1);
    if (!reachesLastRow) {
        return;
    }

    record.wasPromotion = true;
    record.promotionOldPiece = piece;
    addPiece(new Dame(pawn->getIsBlack()), end_pos);
}

void Damier::play(const Position &start_pos, const Position &end_pos, bool turnBlack){
    Piece* movedPieceBefore = getPiece(start_pos);
    Piece* capturedPieceBefore = getPiece(end_pos);
    Position capturedPositionBefore = end_pos;

    bool isJumpCapture = false;
    Position jumpCapturedPos = end_pos;

    if (movedPieceBefore != nullptr) { // handdle jump capture
        const int dx = end_pos.getX() - start_pos.getX();
        const int dy = end_pos.getY() - start_pos.getY();

        if (std::abs(dx) == 2 && std::abs(dy) == 2) {
            jumpCapturedPos = Position(start_pos.getX() + dx / 2, start_pos.getY() + dy / 2);
            Piece* jumpedPiece = getPiece(jumpCapturedPos);
            // advicer piece
            if (jumpedPiece != nullptr && jumpedPiece->getIsBlack() != movedPieceBefore->getIsBlack()) {
                isJumpCapture = true;
                capturedPieceBefore = jumpedPiece;
                capturedPositionBefore = jumpCapturedPos;
            }
        }
    }

    // execute the move
    Plateau::play(start_pos, end_pos, turnBlack);

    if (isJumpCapture) {
        addPiece(nullptr, jumpCapturedPos);
    }

    // struct to record
    MoveRecord record {
        turnBlack,
        start_pos,
        end_pos,
        movedPieceBefore,
        capturedPieceBefore,
        capturedPositionBefore,
        false, // no first move concept
        false,
        nullptr,
        {}
    };

    promotePawnIfNeeded(end_pos, record);

    // to save the move in history 
    _movesHistory.push(record);
}

bool Damier::canUndo() const {
    return _movesHistory.canUndo();
}
bool Damier::undoLastMove() {
    // mo move to undo
    if (!_movesHistory.canUndo()) return false;

    MoveRecord move = _movesHistory.pop();
    Piece* movedPiece = getPiece(move.to);
    // handling in case error: piece is missing BUT not expected
    if (movedPiece == nullptr) return false;

    movePiece(move.to, move.from); // back to original position ; undid move

    addPiece(move.capturedPiece, move.capturedPosition); // restored the piece on checkboard

    // handle is promotion
    if (move.wasPromotion && move.promotionOldPiece != nullptr) {
        addPiece(move.promotionOldPiece, move.from);
    }

    return true;
}

void Damier::initialConditions() {
    // BLACK
    for (int y = 1; y <= 4; ++y) {
        for (int x = 1; x <= 10; ++x) {
            if ((x + y) % 2 == 0) {
                addPiece(new Pion(true), Position(x, y));
            }
        }
    }
    // WHITE
    for (int y = 7; y <= 10; ++y) {
        for (int x = 1; x <= 10; ++x) {
            if ((x + y) % 2 == 0) {
                addPiece(new Pion(false), Position(x, y));
            }
        }
    }
}

std::string Damier::toString() const {
    std::ostringstream oss;
    oss << "\n" ;
    for (int x = 0; x <= getWidth(); x++) {
        for (int y = 0; y <= getHeight(); y++) {
            if (x==0 && y==0) { // [0][0]
                oss << " "; 
            }
            else if (x!=0 && y==0) { // [*][0]
                oss << std::to_string(x) << " ";
            }
            else if (x==0 && y!=0) { /// [0][*]
                oss << " " <<char('A' + y - 1) << " ";
            }
            else if (plateau_vec[x][y] != nullptr) { // [*][*] with piece
                oss << plateau_vec[x][y]->toString() << " ";
            }
            else {
                oss << " . ";
            }
        }
        oss << "\n" ;
    }
    // oss << "  A  B  C  D  E  F  G  H";
    return oss.str();
}

std::string Damier::toUnicodeString() const {
    std::ostringstream oss;
    oss << "\n";
    for (int x = 0; x <= getWidth(); x++) {
        for (int y = 0; y <= getHeight(); y++) {
            if (x == 0 && y == 0) {
                oss << "  ";
            }
            else if (x != 0 && y == 0) {
                oss << std::to_string(x) << " ";
            }
            else if (x == 0 && y != 0) {
                oss << " " <<char('A' + y - 1) << " ";
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
    // oss << "   A  B  C  D  E  F  G  H";
    return oss.str();
}

void Damier::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    // file open error handling
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
        if (dynamic_cast<const Pion*>(piece) != nullptr) return "Pion";
        if (dynamic_cast<const Dame*>(piece) != nullptr) return "Dame";
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

std::vector<Damier::ReplayMove> Damier::readMovesFromFile(const std::string& filename) const {
    // same as saveToFile but in reverse order and with error handling for malformed lines
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
            // ignore malformed lines
            continue;
        }
    }

    return moves;
}

void Damier::loadFromFile(const std::string& filename) {
    // replay move by move 
    for (const ReplayMove& move : readMovesFromFile(filename)) {
        play(move.from, move.to, move.turnBlack);
    }
}
