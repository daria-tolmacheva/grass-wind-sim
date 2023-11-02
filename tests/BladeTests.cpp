#include <gtest/gtest.h>

#include "Blade.h"

TEST(Blade, ctor)
{
    Blade b;
    for(auto &controlPoint : b.getControlPoints())
    {
        EXPECT_EQ(controlPoint.x, 0.0f);
        EXPECT_EQ(controlPoint.y, 0.0f);
        EXPECT_EQ(controlPoint.z, 0.0f);
    }
}

TEST(Blade, addControlPoint)
{
    Blade b;
    for(int i; i < 4; ++i)
    {
        b.addControlPoint(i, Point(1.0f, 1.0f, 1.0f));
        EXPECT_EQ(b.getControlPoints()[i].x, 1.0f);
        EXPECT_EQ(b.getControlPoints()[i].y, 1.0f);
        EXPECT_EQ(b.getControlPoints()[i].z, 1.0f);
    }
    // Fails to add more than 4 control points
    EXPECT_FALSE(b.addControlPoint(4, Point(1.0f, 1.0f, 1.0f)));
}