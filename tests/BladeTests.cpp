#include <gtest/gtest.h>

#include "Blade.h"

TEST(Blade, ctor)
{
    Blade b;
    for(auto &controlPoint : b.getControlPoints())
    {
        EXPECT_EQ(controlPoint, Point(0.0f, 0.0f, 0.0f));
    }
}

TEST(Blade, addControlPoint)
{
    Blade b;
    for(int i; i < 4; ++i)
    {
        b.addControlPoint(i, Point(1.0f, 1.0f, 1.0f));
        EXPECT_EQ(b.getControlPoints()[i], Point(1.0f, 1.0f, 1.0f));
    }
    // Fails to add more/less than 4 control points
    EXPECT_FALSE(b.addControlPoint(4, Point(1.0f, 1.0f, 1.0f)));
}