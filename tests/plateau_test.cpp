#include <gtest/gtest.h>

#include "exception.hpp"
#include "plateau.hpp"

class TestPlateau : public Plateau {
public:
    TestPlateau(int h, int w) : Plateau(h, w) {}

    using Plateau::addPiece;
    using Plateau::addCaptureScore;
    using Plateau::undoCaptureScore;
    using Plateau::movePiece;

    void initialConditions() override {}
};

class TestPiece : public Piece {
public:
    TestPiece(bool black) : Piece(black, "pawn") {}

    bool isValidMove(const Position &start_pos, const Position &end_pos,
                             bool isCapture, Plateau *board) const override {
        return true;
    }
};

TEST(PlateauTest, GetSize) {
    TestPlateau p(8,8);

    EXPECT_EQ(p.getHeight(), 8);
    EXPECT_EQ(p.getWidth(), 8);
}

TEST(PlateauTest, IsInside) {
    TestPlateau p(8,8);

    Position pos(3,4);

    EXPECT_TRUE(p.isInside(pos));
}

TEST(PlateauTest, NotInside) {
    TestPlateau p(8,8);

    Position pos1(0,4);
    Position pos2(9,2);

    EXPECT_FALSE(p.isInside(pos1));
    EXPECT_FALSE(p.isInside(pos2));
}

TEST(PlateauTest, GetPieceNotInsideThrows) {
    TestPlateau p(8,8);

    Position pos(9,1);

    EXPECT_THROW(p.getPiece(pos), InvalidMoveException);
}

TEST(PlateauTest, AddPiece) {
    TestPlateau p(8,8);

    Position pos(2,3);
    Piece* piece = new TestPiece(true);

    p.addPiece(piece, pos);

    EXPECT_EQ(p.getPiece(pos), piece);
}

TEST(PlateauTest, MovePiece) {
    TestPlateau p(8,8);

    Position start(2,2);
    Position end(3,3);

    Piece* piece = new TestPiece(true);

    p.addPiece(piece, start);
    p.movePiece(start, end);

    EXPECT_EQ(p.getPiece(start), nullptr);
    EXPECT_EQ(p.getPiece(end), piece);
}

TEST(PlateauTest, PlayThrowsIfStartEmpty) {
    TestPlateau p(8,8);

    Position start(2,2);
    Position end(3,3);

    EXPECT_THROW(p.play(start,end,true), InvalidMoveException);
}

TEST(PlateauTest, PlayThrowsWrongColor) {
    TestPlateau p(8,8);

    Position start(2,2);
    Position end(3,3);

    Piece* piece = new TestPiece(false);

    p.addPiece(piece,start);

    EXPECT_THROW(p.play(start,end,true), InvalidMoveException);
}

TEST(PlateauTest, PlayThrowsSameColor) {
    TestPlateau p(8,8);

    Position start(2,2);
    Position end(3,3);

    Piece* p1 = new TestPiece(true);
    Piece* p2 = new TestPiece(true);

    p.addPiece(p1,start);
    p.addPiece(p2,end);

    EXPECT_THROW(p.play(start,end,true), InvalidMoveException);
}

TEST(PlateauTest, CaptureScoreTracksRawCapturedMaterial) {
    TestPlateau p(8,8);
    TestPiece capturedWhite(false);
    TestPiece capturedBlack(true);

    EXPECT_EQ(p.getScore(false), 0);
    EXPECT_EQ(p.getScore(true), 0);

    p.addCaptureScore(true, &capturedWhite);
    EXPECT_EQ(p.getScore(true), 1);
    EXPECT_EQ(p.getScore(false), 0);
    EXPECT_EQ(p.getMaterialAdvantage(true), 1);
    EXPECT_EQ(p.getMaterialAdvantage(false), -1);

    p.addCaptureScore(false, &capturedBlack);
    EXPECT_EQ(p.getScore(true), 1);
    EXPECT_EQ(p.getScore(false), 1);
    EXPECT_EQ(p.getMaterialAdvantage(true), 0);
    EXPECT_EQ(p.getMaterialAdvantage(false), 0);

    p.undoCaptureScore(false, &capturedBlack);
    p.undoCaptureScore(true, &capturedWhite);
    EXPECT_EQ(p.getScore(true), 0);
    EXPECT_EQ(p.getScore(false), 0);
}
