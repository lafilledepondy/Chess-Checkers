#include "checkerboard.hpp"

#include <cctype>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>

#include "exception.hpp"

// to avoid code duplication for firstMove flag in Pawn;Rook; King
namespace {
    Position findKingPosition(const Checkerboard& board, bool kingBlack) {
        // run through all squares
        for (int y = 1; y <= board.getHeight(); ++y) {
            for (int x = 1; x <= board.getWidth(); ++x) {
                const Position pos(x, y);
                const Piece* piece = board.getPiece(pos);
                const King* king = dynamic_cast<const King*>(piece);
                // return king color correct pos
                if (king != nullptr && king->getIsBlack() == kingBlack) {
                    return pos;
                }
            }
        }
        return Position(0, 0); //else invalid pos
    }

    bool pieceAttacksSquare(const Piece* attacker, const Position& from, const Position& target, Plateau* board) {
        // attacker !empty
        if (attacker == nullptr) {
            return false;
        }

        const int dx = target.getX() - from.getX();
        const int dy = target.getY() - from.getY();
        // pawn attacks
        if (dynamic_cast<const Pawn*>(attacker) != nullptr) {
            const int direction = attacker->getIsBlack() ? 1 : -1;
            return std::abs(dx) == 1 && dy == direction;
        }
        // king attacks
        if (dynamic_cast<const King*>(attacker) != nullptr) {
            return std::abs(dx) <= 1 && std::abs(dy) <= 1 && !(dx == 0 && dy == 0);
        }
        // other pieces
        Piece* targetPiece = board->getPiece(target);
        return attacker->isValidMove(from, target, targetPiece != nullptr, board);
    }

    bool isSquareUnderAttack(const Checkerboard& board, const Position& square, bool attackerBlack) {
        Checkerboard* mutableBoard = const_cast<Checkerboard*>(&board);
        // run through all squares to find attackers
        for (int y = 1; y <= board.getHeight(); ++y) {
            for (int x = 1; x <= board.getWidth(); ++x) {
                const Position from(x, y);
                const Piece* attacker = board.getPiece(from);
                // only consider pieces of the attacking color
                if (attacker == nullptr || attacker->getIsBlack() != attackerBlack) {
                    continue;
                }
                // check if this piece attacks the target square
                if (pieceAttacksSquare(attacker, from, square, mutableBoard)) {
                    return true;
                }
            }
        }

        return false;
    }

    bool hasAnyLegalMove(const Checkerboard& board, bool playerBlack) {
        Checkerboard* mutableBoard = const_cast<Checkerboard*>(&board);
        // run through all squares to find player's pieces
        for (int y = 1; y <= board.getHeight(); ++y) {
            for (int x = 1; x <= board.getWidth(); ++x) {
                const Position from(x, y);
                Piece* movingPiece = mutableBoard->getPiece(from);
                // only consider pieces of the player's color
                if (movingPiece == nullptr || movingPiece->getIsBlack() != playerBlack) {
                    continue;
                }
                // try all possible moves for this piece
                for (int toY = 1; toY <= board.getHeight(); ++toY) {
                    for (int toX = 1; toX <= board.getWidth(); ++toX) {
                        const Position to(toX, toY);
                        if (to == from) {
                            continue;
                        }
                        // skip moves that capture player's own pieces
                        Piece* target = mutableBoard->getPiece(to);
                        if (target != nullptr && target->getIsBlack() == playerBlack) {
                            continue;
                        }
                        // castling possible if not under check
                        const int deltaX = to.getX() - from.getX();
                        const int deltaY = to.getY() - from.getY();
                        if (dynamic_cast<King*>(movingPiece) != nullptr && deltaY == 0 && std::abs(deltaX) == 2) {
                            // king moves 2 squares horizontally => castling attempt
                            if (board.isInCheck(playerBlack)) {
                                continue;
                            }
                            const int direction = (deltaX > 0) ? 1 : -1;
                            const Position intermediate(from.getX() + direction, from.getY());
                            // else illegal move
                            if (isSquareUnderAttack(board, intermediate, !playerBlack)) {
                                continue;
                            }
                        }

                        try {
                            mutableBoard->play(from, to, playerBlack);
                        }
                        catch (const std::exception&) {
                            continue;
                        }

                        const bool leavesKingInCheck = mutableBoard->isInCheck(playerBlack);
                        mutableBoard->undoLastMove();
                        // checkmate
                        if (!leavesKingInCheck) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

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

    constexpr const char* ANSI_RESET = "\x1b[0m";
    // constexpr const char* ANSI_LAST_MOVE = "\x1b[48;5;220m"; // yellow
    constexpr const char* ANSI_LIGHT_SQUARE = "\x1b[48;5;180m"; //wood color
    constexpr const char* ANSI_DARK_SQUARE = "\x1b[48;5;94m"; //bright wood color
    constexpr const char* ANSI_WHITE_PIECE = "\x1b[38;5;231m"; // white
    constexpr const char* ANSI_BLACK_PIECE = "\x1b[38;5;16m";  // black

    std::string squareBackground(int x, int y) {
        return ((x + y) % 2 == 0) ? ANSI_DARK_SQUARE : ANSI_LIGHT_SQUARE;
    }
}

/*
x <>
y ^
        0 1  2  3  4  5  6  7  8           
    0     A  B  C  D  E  F  G  H 
    1   1 Ro Ho Bi Qu Ki Bi Ho Ro       <> BLACK
    2   2 Pa Pa Pa Pa Pa Pa Pa Pa
    3   3 . . . . . . . . . . . .
    4   4 . . . . . . . . . . . .
    5   5 . . . . . . . . . . . .
    6   6 . . . . . . . . . . . .
    7   7 pa pa pa pa pa pa pa pa       <> WHITE
    8   8 ro ho bi qu ki bi ho ro
*/

Checkerboard::Checkerboard():Plateau(8, 8) {}

bool Checkerboard::isInCheck(bool kingBlack) const {
    const Position kingPos = findKingPosition(*this, kingBlack);
    // king missing -> false
    if (!isInside(kingPos)) {
        return false;
    }
    return isSquareUnderAttack(*this, kingPos, !kingBlack);
}

bool Checkerboard::isCheckmate(bool kingBlack) const {
    return isInCheck(kingBlack) && !hasAnyLegalMove(*this, kingBlack);
}

bool Checkerboard::isStalemate(bool playerBlack) const {
    return !isInCheck(playerBlack) && !hasAnyLegalMove(*this, playerBlack);
}

bool Checkerboard::canEnPassantCapture(const Position &start_pos, const Position &end_pos) const {
    // not possible if no move has been made yet
    if (_movesHistory.empty()) {
        return false;
    }
    // assert is start_pos is a pawn
    const Piece* attacker = getPiece(start_pos);
    if (attacker == nullptr || dynamic_cast<const Pawn*>(attacker) == nullptr) {
        return false;
    }
    // assert last move was a 2 square pawn advance
    const MoveRecord& lastMove = _movesHistory.top();
    if (dynamic_cast<const Pawn*>(lastMove.movedPiece) == nullptr) {
        return false;
    }
    // assert last move was a 2 square advance in the same column
    if (std::abs(lastMove.to.getY() - lastMove.from.getY()) != 2) {
        return false;
    }
    // assert last move ended adjacent to the attacker
    const Position side(end_pos.getX(), start_pos.getY());
    if (lastMove.to != side) {
        return false;
    }
    // assert attacker is on correct rank for en passant
    return lastMove.turnBlack != attacker->getIsBlack();
}

Position Checkerboard::getEnPassantCapturedPosition(const Position &start_pos, const Position &end_pos) const {
    // if fails then invalid position(0, 0)
    if (!canEnPassantCapture(start_pos, end_pos)) {
        return Position(0, 0);
    }
    return Position(end_pos.getX(), start_pos.getY());
}

void Checkerboard::play(const Position &start_pos, const Position &end_pos, bool turnBlack) {
    Piece* movedPieceBefore = getPiece(start_pos);
    Piece* capturedPieceBefore = getPiece(end_pos);
    Position capturedPositionBefore = end_pos;

    // Castling cannot start from check and cannot pass through an attacked square.
    if (King* movingKing = dynamic_cast<King*>(movedPieceBefore)) {
        const int deltaX = end_pos.getX() - start_pos.getX();
        const int deltaY = end_pos.getY() - start_pos.getY();
        if (movingKing->firstMove && deltaY == 0 && std::abs(deltaX) == 2) {
            if (isInCheck(turnBlack)) {
                throw InvalidMoveException(6, "castling is illegal while king is in check.", 2);
            }

            const int direction = (deltaX > 0) ? 1 : -1;
            const Position intermediate(start_pos.getX() + direction, start_pos.getY());
            if (isSquareUnderAttack(*this, intermediate, !turnBlack)) {
                throw InvalidMoveException(7, "castling through an attacked square is illegal.", 2);
            }
        }
    }

    // en passant capture check
    if (capturedPieceBefore == nullptr &&
        dynamic_cast<Pawn*>(movedPieceBefore) != nullptr &&
        canEnPassantCapture(start_pos, end_pos)) {

        capturedPositionBefore = getEnPassantCapturedPosition(start_pos, end_pos);
        // en passant capture handling
        if (isInside(capturedPositionBefore)) {
            capturedPieceBefore = getPiece(capturedPositionBefore);
        }
    }

    // execute the move
    const bool movedPieceWasFirstMove = getFirstMoveFlag(movedPieceBefore);
    addCaptureScore(turnBlack, capturedPieceBefore);
    Plateau::play(start_pos, end_pos, turnBlack);

    // struct to record
    MoveRecord record {
        turnBlack,
        start_pos,
        end_pos,
        movedPieceBefore,
        capturedPieceBefore,
        capturedPositionBefore,
        movedPieceWasFirstMove,
        false,
        nullptr,
        {}
    };
    // to save the move in history 
    _movesHistory.push(record);

    // Any move that leaves own king in check is illegal and must be rolled back.
    if (isInCheck(turnBlack)) {
        undoLastMove();
        throw InvalidMoveException(8, "move leaves own king in check.", 2);
    }
}

bool Checkerboard::canUndo() const {
    return _movesHistory.canUndo();
}

bool Checkerboard::undoLastMove() {
    // mo move to undo
    if (!_movesHistory.canUndo()) return false;

    MoveRecord move = _movesHistory.pop();
    Piece* movedPiece = getPiece(move.to);
    // handling in case error: piece is missing BUT not expected
    if (movedPiece == nullptr) return false;

    movePiece(move.to, move.from); // back to original position ; undid move
    // if castling happened 
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
    } else { // pawn or rook
        setFirstMoveFlag(getPiece(move.from), move.movedPieceWasFirstMove);
    }
    // restored the piece on checkboard
    addPiece(move.capturedPiece, move.capturedPosition);
    undoCaptureScore(move.turnBlack, move.capturedPiece);

    // handle is promotion
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

std::string Checkerboard::toUnicodeString() const {
    // same as toString but with Unicode characters
    std::ostringstream oss;
    oss << "\n";
    for (int x = 0; x <= getWidth(); x++) {
        for (int y = 0; y <= getHeight(); y++) {
            if (x == 0 && y == 0) {
                oss << "   ";
            }
            else if (x != 0 && y == 0) {
                oss << std::to_string(x) << "  ";
            }
            else if (x == 0 && y != 0) {
                oss << " " <<char('A' + y - 1) << " ";
            }
            else if (plateau_vec[x][y] != nullptr) {
                const Piece* piece = plateau_vec[x][y];
                const char* pieceColor = piece->getIsBlack() ? ANSI_BLACK_PIECE : ANSI_WHITE_PIECE;
                oss << squareBackground(x, y)
                    << pieceColor
                    << " " << piece->toUnicodeString() << " "
                    << ANSI_RESET;
            }
            else {
                oss << squareBackground(x, y) << "   " << ANSI_RESET;
            }
        }
        oss << "\n";
    }
    // oss << "   A  B  C  D  E  F  G  H";
    return oss.str();
}

void Checkerboard::saveToFile(const std::string& filename) const {
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

void Checkerboard::loadFromFile(const std::string& filename) {
    // replay move by move 
    for (const ReplayMove& move : readMovesFromFile(filename)) {
        play(move.from, move.to, move.turnBlack);
    }
}