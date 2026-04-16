#pragma once

#include <string>
#include <vector>

#include "plateau.hpp"
#include "pawn.hpp"
#include "rook.hpp"
#include "cavalier.hpp"
#include "queen.hpp"
#include "bishop.hpp"
#include "king.hpp"
#include "movesPile.hpp"

class Checkerboard: public Plateau {
    private:
        MovesPile _movesHistory;

    public:
        struct ReplayMove {
            bool turnBlack;
            Position from;
            Position to;
        };

        // ----------------------------------------------------------------------------
        // constructors
        // ----------------------------------------------------------------------------    
        Checkerboard();

        // ----------------------------------------------------------------------------
        // methods
        // ----------------------------------------------------------------------------         
        void initialConditions();
        void play(const Position &start_pos, const Position &end_pos, bool turnBlack);
        bool canEnPassantCapture(const Position &start_pos, const Position &end_pos) const override;
        Position getEnPassantCapturedPosition(const Position &start_pos, const Position &end_pos) const override;
        bool undoLastMove();
        bool canUndo() const;
        std::string toString() const;
        std::string toUnicodeString() const;
        std::vector<ReplayMove> readMovesFromFile(const std::string& filename) const;
        void loadFromFile(const std::string& filename);
        void saveToFile(const std::string& filename) const;

        // state helpers
        bool isInCheck(bool kingBlack) const;
        bool isCheckmate(bool kingBlack) const;
        bool isStalemate(bool playerBlack) const;
};