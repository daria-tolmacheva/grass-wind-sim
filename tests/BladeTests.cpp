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

TEST(Blade, setControlPoint)
{
    Blade b;
    for(int i = 0; i < 4; ++i)
    {
        b.setControlPoint(i, Point(1.0f, 1.0f, 1.0f));
        EXPECT_EQ(b.getControlPoints()[i], Point(1.0f, 1.0f, 1.0f));
    }
    // Fails to add more than 4 control points
    EXPECT_FALSE(b.setControlPoint(4, Point(1.0f, 1.0f, 1.0f)));
}

TEST(Blade, setControlPoints)
{
    Blade b;
    std::vector<Point> bladeControlPoints = { {0.0f,  0.0f,  0.0f},
                                              {0.0f,  7.0f,  1.0f},
                                              {0.0f,  9.0f,  4.0f},
                                              {0.0f,  10.0f, 6.0f} };
    EXPECT_TRUE(b.setControlPoints(bladeControlPoints));
    EXPECT_EQ(b.getControlPoints(), bladeControlPoints);
    // Fails if less/more than 4 control points
    EXPECT_FALSE(b.setControlPoints({Point(0.0f, 0.0f, 0.0f)}));
}