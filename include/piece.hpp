#pragma once

#include "position.hpp"
#include "plateau.hpp"

class Plateau;

class Piece {
    private:
        // ----------------------------------------------------------------------------
        // private attribute
        // ----------------------------------------------------------------------------
        const std::string _name; 
        const bool _isBlack;     

    public:
        // ----------------------------------------------------------------------------
        // constructors
        // ----------------------------------------------------------------------------
        Piece(bool isBlack, std::string name);

        // ----------------------------------------------------------------------------
        // getters
        // ----------------------------------------------------------------------------     
        bool getIsBlack() const;

        // ----------------------------------------------------------------------------
        // abstract methods
        // ----------------------------------------------------------------------------
        virtual bool isValidMove(const Position &start_pos, const Position &end_pos,
                                bool isCapture, Plateau *board) const = 0;

        // ----------------------------------------------------------------------------
        // methods
        // ----------------------------------------------------------------------------
        std::string toString() const;
        std::vector<Position> getValidMoves(const Position& start, const Plateau* plateau) const;
};