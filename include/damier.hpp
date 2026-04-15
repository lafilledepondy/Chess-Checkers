#pragma once

#include <string>
#include <vector>

#include "pion.hpp"
#include "dame.hpp"
#include "movesPile.hpp"

class Damier: public Plateau {
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
        Damier();

        // ----------------------------------------------------------------------------
        // methods
        // ----------------------------------------------------------------------------         
        void initialConditions();
        void play(const Position &start_pos, const Position &end_pos, bool turnBlack);
        bool undoLastMove();
        bool canUndo() const;
        std::string toString() const;
        std::string toUnicodeString() const;
        std::vector<ReplayMove> readMovesFromFile(const std::string& filename) const;
        void loadFromFile(const std::string& filename);
        void saveToFile(const std::string& filename) const;
};