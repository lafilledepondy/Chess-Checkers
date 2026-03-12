#include <gtest/gtest.h>

#include "piece.hpp"
#include "plateau.hpp"

class TestPlateau : public Plateau {
public:
    TestPlateau(int h, int w) : Plateau(h, w) {}

    void initialConditions() override {}
};

class TestPiece : public Piece {
public:
    TestPiece(bool color, const std::string& name)
        : Piece(color, name) {}

    bool isValidMove(const Position&, const Position&, bool, Plateau*) const override {
        return true;
    }
};

class TestPieceExceptOne : public Piece {
public:
    TestPieceExceptOne(bool color, const std::string& name)
        : Piece(color, name) {}

    bool isValidMove(const Position& start,
                     const Position& end,
                     bool,
                     Plateau*) const override {

        if (end.getX() == 3 && end.getY() == 3)
            return false;

        return true;
    }
};

TEST(PieceTest, BlackPiece) {
    TestPiece piece_1(true, "pawn");
    TestPiece piece_2(false, "pawn");

    EXPECT_TRUE(piece_1.getIsBlack());
    EXPECT_FALSE(piece_2.getIsBlack());
}

TEST(PieceTest, ToStringBlack) {
    TestPiece piece(true, "pawn");
    TestPiece piece1(true, "Pawn");

    EXPECT_EQ(piece.toString(), "Pa");
    EXPECT_EQ(piece1.toString(), "Pa");    
}

TEST(PieceTest, ToStringWhite) {
    TestPiece piece(false, "pawn");
    TestPiece piece1(false, "Pawn");

    EXPECT_EQ(piece.toString(), "pa");
    EXPECT_EQ(piece1.toString(), "pa");
}

TEST(PieceTest, GetValidMovesAll) {
    TestPiece piece(true, "pawn");
    TestPlateau board(8,8);

    Position start(0,0);

    auto moves = piece.getValidMoves(start, &board);

    EXPECT_EQ(moves.size(), 63);
}

// TEST(PieceTest, GetValidMovesAllExceptOne) {
//     TestPieceExceptOne piece(true, "pawn");
//     TestPlateau board(8,8);

//     Position start(0,0);

//     auto moves = piece.getValidMoves(start, &board);

//     EXPECT_EQ(moves.size(), 63);
// }