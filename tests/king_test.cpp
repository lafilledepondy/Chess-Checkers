#include <gtest/gtest.h>

#include "exception.hpp"
#include "checkerboard.hpp"
#include "king.hpp"
#include "pawn.hpp"

class KingBoardAccess : public Checkerboard {
public:
    void clearBoard() {
        for (int y = 1; y <= getHeight(); ++y) {
            for (int x = 1; x <= getWidth(); ++x) {
                addPiece(nullptr, Position(x, y));
            }
        }
    }

    void placePiece(Piece* piece, const Position& pos) {
        addPiece(piece, pos);
    }
};

Checkerboard setupKingBoard() {
    Checkerboard cb;
    cb.initialConditions();

    cb.play(Position("E2"), Position("E3"), true);   // white pawn
    cb.play(Position("D7"), Position("D6"), false);  // black pawn 

    return cb;
}

TEST(KingTest, ValidOneSquareMoves) {
    Checkerboard cb = setupKingBoard();

    EXPECT_NO_THROW(cb.play(Position("E1"), Position("E2"), true));
}

TEST(KingTest, MoveTooFarThrows) {
    Checkerboard cb = setupKingBoard();

    EXPECT_THROW(
        cb.play(Position("E1"), Position("E3"), true),
        InvalidMoveException
    );
}

TEST(KingTest, CannotMoveOntoOwnPiece) {
    Checkerboard cb;
    cb.initialConditions();

    EXPECT_THROW(
        cb.play(Position("E1"), Position("D1"), true), // queen is there
        InvalidMoveException
    );
}

TEST(KingTest, CaptureEnemy) {
    KingBoardAccess cb;
    cb.clearBoard();

    cb.placePiece(new King(false), Position("E4"));
    cb.placePiece(new King(true), Position("A8"));
    cb.placePiece(new Pawn(true), Position("D5"));

    EXPECT_NO_THROW(cb.play(Position("E4"), Position("D5"), false));
}

TEST(KingTest, SamePositionThrows) {
    Checkerboard cb = setupKingBoard();

    EXPECT_THROW(
        cb.play(Position("E1"), Position("E1"), true),
        InvalidMoveException
    );
}

TEST(KingTest, KingsideCastlingMovesKingAndRook) {
    Checkerboard cb;
    cb.initialConditions();

    cb.play(Position("E2"), Position("E3"), true);
    cb.play(Position("A7"), Position("A6"), false);
    cb.play(Position("F1"), Position("E2"), true);
    cb.play(Position("A6"), Position("A5"), false);
    cb.play(Position("G1"), Position("F3"), true);
    cb.play(Position("A5"), Position("A4"), false);

    EXPECT_NO_THROW(cb.play(Position("E1"), Position("G1"), true));

    EXPECT_NE(cb.getPiece(Position("G1")), nullptr);
    EXPECT_NE(cb.getPiece(Position("F1")), nullptr);
    EXPECT_EQ(cb.getPiece(Position("H1")), nullptr);
}

TEST(KingTest, CastlingNotAllowedIfRookAlreadyMoved) {
    Checkerboard cb;
    cb.initialConditions();

    cb.play(Position("H2"), Position("H3"), true);
    cb.play(Position("A7"), Position("A6"), false);
    cb.play(Position("H1"), Position("H2"), true);
    cb.play(Position("A6"), Position("A5"), false);
    cb.play(Position("H2"), Position("H1"), true);
    cb.play(Position("A5"), Position("A4"), false);

    cb.play(Position("E2"), Position("E3"), true);
    cb.play(Position("B7"), Position("B6"), false);
    cb.play(Position("F1"), Position("E2"), true);
    cb.play(Position("B6"), Position("B5"), false);
    cb.play(Position("G1"), Position("F3"), true);
    cb.play(Position("C7"), Position("C6"), false);

    EXPECT_THROW(
        cb.play(Position("E1"), Position("G1"), true),
        InvalidMoveException
    );
}
