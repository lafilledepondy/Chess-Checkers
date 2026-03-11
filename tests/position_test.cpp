#include <gtest/gtest.h>

#include "position.hpp"

TEST(PositionTest, ValidPositions) {
    Position p1(1,5);
    Position p2(p1,1,1);
    Position p3("AT128");

    EXPECT_EQ(p1.toString(), "[A5]");
    EXPECT_EQ(p2.toString(), "[B6]");
    EXPECT_EQ(p3.toString(), "[AT128]");
}

