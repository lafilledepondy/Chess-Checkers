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

TEST(PieceTest, IsValidMoveStubAlwaysTrue) {
    TestPiece piece(true, "pawn");
    TestPlateau board(8, 8);
    Position start(1, 1);
    Position end(8, 8);

    EXPECT_TRUE(piece.isValidMove(start, end, false, &board));
}

TEST(PieceTest, UnicodeSymbolsBlack) {
    EXPECT_EQ(TestPiece(true, "pawn").toUnicodeString(), "♟");
    EXPECT_EQ(TestPiece(true, "rook").toUnicodeString(), "♜");
    EXPECT_EQ(TestPiece(true, "cavalier").toUnicodeString(), "♞");
    EXPECT_EQ(TestPiece(true, "bishop").toUnicodeString(), "♝");
    EXPECT_EQ(TestPiece(true, "queen").toUnicodeString(), "♛");
    EXPECT_EQ(TestPiece(true, "king").toUnicodeString(), "♚");
}

TEST(PieceTest, UnicodeSymbolsWhite) {
    EXPECT_EQ(TestPiece(false, "pawn").toUnicodeString(), "♙");
    EXPECT_EQ(TestPiece(false, "rook").toUnicodeString(), "♖");
    EXPECT_EQ(TestPiece(false, "cavalier").toUnicodeString(), "♘");
    EXPECT_EQ(TestPiece(false, "bishop").toUnicodeString(), "♗");
    EXPECT_EQ(TestPiece(false, "queen").toUnicodeString(), "♕");
    EXPECT_EQ(TestPiece(false, "king").toUnicodeString(), "♔");
}