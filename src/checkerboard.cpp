#include "checkerboard.hpp"

/*
x <>
y ^
        0 1  2  3  4  5  6  7  8           
    0     A  B  C  D  E  F  G  H
    1   8 Ro Ho Bi Qu Ki Bi Ho Ro       <> BLACK
    2   7 Pa Pa Pa Pa Pa Pa Pa Pa
    3   6 . . . . . . . . . . . .
    4   5 . . . . . . . . . . . .
    5   4 . . . . . . . . . . . .
    6   3 . . . . . . . . . . . .
    7   2 pa pa pa pa pa pa pa pa       <> WHITE
    8   1 ro ho bi qu ki bi ho ro
*/

Checkerboard::Checkerboard():Plateau(8, 8) {}
       
void Checkerboard::initialConditions() { 
    // TODO 

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
    // // ---black
    // addPiece(new Rook(true), Position());
    // addPiece(new Horse(true), Position());    
    // addPiece(new Bishop(true), Position());    
    // addPiece(new Queen(true), Position());    
    // addPiece(new King(true), Position());    
    // addPiece(new Bishop(true), Position());    
    // addPiece(new Horse(true), Position());    
    // addPiece(new Rook(true), Position()); 
    // // ---white
    // addPiece(new Rook(false), Position());
    // addPiece(new Horse(false), Position());    
    // addPiece(new BishopPawn(false), Position());    
    // addPiece(new Queen(false), Position());    
    // addPiece(new King(false), Position());    
    // addPiece(new Bishop(false), Position());    
    // addPiece(new Horse(false), Position());    
    // addPiece(new Rook(false), Position());     
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
                oss << std::to_string(x) << " ";
            }
            else if (x==0 && y!=0) { /// [0][*]
                oss << " " <<char('A' + y - 1) << " ";
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
    return oss.str();
}