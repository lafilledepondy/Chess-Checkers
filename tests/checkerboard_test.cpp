#include <gtest/gtest.h>

#include "checkerboard.hpp"
#include "king.hpp"
#include "queen.hpp"
#include "rook.hpp"
#include "pawn.hpp"
#include "exception.hpp"

class CheckerboardAccess : public Checkerboard {
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

TEST(CheckerboardUndoTest, UndoSimpleMoveRestoresBoard) {
    Checkerboard cb;
    cb.initialConditions();

    ASSERT_TRUE(cb.canUndo() == false);

    cb.play(Position("B7"), Position("B5"), false);

    ASSERT_TRUE(cb.canUndo());
    EXPECT_EQ(cb.getPiece(Position("B7")), nullptr);
    EXPECT_NE(cb.getPiece(Position("B5")), nullptr);

    ASSERT_TRUE(cb.undoLastMove());

    EXPECT_NE(cb.getPiece(Position("B7")), nullptr);
    EXPECT_EQ(cb.getPiece(Position("B5")), nullptr);

    // Pawn should recover firstMove after undo and allow double-step again.
    EXPECT_NO_THROW(cb.play(Position("B7"), Position("B5"), false));
}

TEST(CheckerboardUndoTest, UndoCaptureRestoresCapturedPiece) {
    Checkerboard cb;
    cb.initialConditions();

    cb.play(Position("B7"), Position("B5"), false);
    cb.play(Position("B2"), Position("B3"), true);
    cb.play(Position("C2"), Position("C4"), true);

    Piece* attackerBefore = cb.getPiece(Position("B5"));
    Piece* victimBefore = cb.getPiece(Position("C4"));
    ASSERT_NE(attackerBefore, nullptr);
    ASSERT_NE(victimBefore, nullptr);

    ASSERT_NO_THROW(cb.play(Position("B5"), Position("C4"), false));

    Piece* capturedSquareAfterCapture = cb.getPiece(Position("C4"));
    ASSERT_NE(capturedSquareAfterCapture, nullptr);
    EXPECT_EQ(capturedSquareAfterCapture, attackerBefore);

    ASSERT_TRUE(cb.undoLastMove());

    Piece* restoredAttacker = cb.getPiece(Position("B5"));
    Piece* restoredCaptured = cb.getPiece(Position("C4"));

    ASSERT_NE(restoredAttacker, nullptr);
    ASSERT_NE(restoredCaptured, nullptr);
    EXPECT_EQ(restoredAttacker, attackerBefore);
    EXPECT_EQ(restoredCaptured, victimBefore);
}

TEST(CheckerboardUndoTest, UndoEnPassantRestoresCapturedPawn) {
    Checkerboard cb;
    cb.initialConditions();

    cb.play(Position("E7"), Position("E5"), false);
    cb.play(Position("A2"), Position("A3"), true);
    cb.play(Position("E5"), Position("E4"), false);
    cb.play(Position("D2"), Position("D4"), true);

    Piece* attackerBefore = cb.getPiece(Position("E4"));
    Piece* victimBefore = cb.getPiece(Position("D4"));
    ASSERT_NE(attackerBefore, nullptr);
    ASSERT_NE(victimBefore, nullptr);

    ASSERT_NO_THROW(cb.play(Position("E4"), Position("D3"), false));
    ASSERT_TRUE(cb.undoLastMove());

    EXPECT_EQ(cb.getPiece(Position("E4")), attackerBefore);
    EXPECT_EQ(cb.getPiece(Position("D4")), victimBefore);
    EXPECT_EQ(cb.getPiece(Position("D3")), nullptr);
}

TEST(CheckerboardStateTest, DetectCheckByDirectAttack) {
    CheckerboardAccess cb;
    cb.clearBoard();

    cb.placePiece(new King(true), Position("E1"));
    cb.placePiece(new King(false), Position("A8"));
    cb.placePiece(new Rook(false), Position("E8"));

    EXPECT_TRUE(cb.isInCheck(true));

    cb.placePiece(new Pawn(true), Position("E4"));
    EXPECT_FALSE(cb.isInCheck(true));
}

TEST(CheckerboardStateTest, DetectCheckmate) {
    CheckerboardAccess cb;
    cb.clearBoard();

    cb.placePiece(new King(true), Position("A1"));
    cb.placePiece(new King(false), Position("C3"));
    cb.placePiece(new Queen(false), Position("B2"));
    cb.placePiece(new Rook(false), Position("A2"));

    EXPECT_TRUE(cb.isInCheck(true));
    EXPECT_TRUE(cb.isCheckmate(true));
    EXPECT_FALSE(cb.isStalemate(true));
}

TEST(CheckerboardStateTest, DetectStalemate) {
    CheckerboardAccess cb;
    cb.clearBoard();

    cb.placePiece(new King(true), Position("A1"));
    cb.placePiece(new King(false), Position("C2"));
    cb.placePiece(new Queen(false), Position("B3"));

    EXPECT_FALSE(cb.isInCheck(true));
    EXPECT_FALSE(cb.isCheckmate(true));
    EXPECT_TRUE(cb.isStalemate(true));
}

TEST(CheckerboardRulesTest, CannotPlayMoveWhenStillInCheck) {
    CheckerboardAccess cb;
    cb.clearBoard();

    cb.placePiece(new King(true), Position("A1"));
    cb.placePiece(new Pawn(true), Position("H2"));
    cb.placePiece(new King(false), Position("C3"));
    cb.placePiece(new Queen(false), Position("B2"));
    cb.placePiece(new Rook(false), Position("A2"));

    ASSERT_TRUE(cb.isCheckmate(true));
    EXPECT_THROW(cb.play(Position("H2"), Position("H3"), true), InvalidMoveException);
}

TEST(CheckerboardRulesTest, CastlingThroughCheckIsIllegal) {
    CheckerboardAccess cb;
    cb.clearBoard();

    cb.placePiece(new King(false), Position("E1"));
    cb.placePiece(new Rook(false), Position("H1"));
    cb.placePiece(new King(true), Position("A8"));
    cb.placePiece(new Rook(true), Position("F8"));

    EXPECT_THROW(cb.play(Position("E1"), Position("G1"), false), InvalidMoveException);
}
