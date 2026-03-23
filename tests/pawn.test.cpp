#include <gtest/gtest.h>

#include "exception.hpp"
#include "checkerboard.hpp"

// Helper to setup a mid-game board
Checkerboard setupBoard() {
    Checkerboard cb;
    cb.initialConditions();

    cb.play(Position("B7"), Position("B5"), false); // p1
    cb.play(Position("B2"), Position("B3"), true);  // P2
    cb.play(Position("C2"), Position("C4"), true); // P3
    /*
          A  B  C  D  E  F  G  H
        1 Ro Ho Bi Qu Ki Bi Ho Ro
        2 Pa  .  .  Pa Pa Pa Pa Pa
        3  .  P2 .  .  .  .  .  .
        4  .  .  P3 .  .  .  .  .
        5  .  p1 .  .  .  .  .  .
        6  .  .  .  .  .  .  .  .
        7 pa  . pa pa pa pa pa pa
        8 ro ho bi qu ki bi ho ro
    */   
    return cb;
}
 
TEST(PawnTest, DoubleStepAfterMoveThrows) {
    Checkerboard cb = setupBoard();

    EXPECT_THROW(
        cb.play(Position("B5"), Position("B3"), false),
        InvalidMoveException
    );
} 

TEST(PawnTest, ValidForwardMove) {
    Checkerboard cb = setupBoard();

    EXPECT_NO_THROW(
        cb.play(Position("B3"), Position("B4"), true)
    );
}

TEST(PawnTest, PiecesMovedCorrectly) {
    Checkerboard cb = setupBoard();

    EXPECT_EQ(cb.getPiece(Position("B7")), nullptr);
    EXPECT_EQ(cb.getPiece(Position("B2")), nullptr);

    EXPECT_NE(cb.getPiece(Position("B5")), nullptr);
    EXPECT_NE(cb.getPiece(Position("B3")), nullptr);
}

TEST(PawnTest, ValidDiagonalCapture) {
    Checkerboard cb = setupBoard();

    EXPECT_NO_THROW(
        cb.play(Position("B5"), Position("C4"), false)
    );

    EXPECT_EQ(cb.getPiece(Position("B5")), nullptr);
    EXPECT_NE(cb.getPiece(Position("C4")), nullptr);
}

TEST(PawnTest, InvalidDiagonalWithoutCapture) {
    Checkerboard cb;
    cb.initialConditions();

    EXPECT_THROW(
        cb.play(Position("B7"), Position("A6"), false),
        InvalidMoveException
    );
}

TEST(PawnTest, InvalidBackwardMove) {
    Checkerboard cb;
    cb.initialConditions();

    cb.play(Position("B7"), Position("B6"), false);

    EXPECT_THROW(
        cb.play(Position("B6"), Position("B7"), false),
        InvalidMoveException
    );
}

TEST(PawnTest, ForwardBlockedThrows) {
    Checkerboard cb = setupBoard();
    cb.play(Position("B3"), Position("B4"), true);

    EXPECT_THROW(
        cb.play(Position("B5"), Position("B4"), false),
        InvalidMoveException
    );
}