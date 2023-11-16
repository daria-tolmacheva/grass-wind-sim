#include <gtest/gtest.h>
#include <ngl/Vec3.h>
#include <cmath>

#include "Blade.h"

TEST(Blade, ctor)
{
    Blade b;
    for(auto &controlPoint : b.getControlPoints())
    {
        EXPECT_EQ(controlPoint, ngl::Vec3(0.0f, 0.0f, 0.0f));
    }
}

TEST(Blade, setControlPoint)
{
    Blade b;
    for(int i = 0; i < 4; ++i)
    {
        b.setControlPoint(i, ngl::Vec3(1.0f, 1.0f, 1.0f));
        EXPECT_EQ(b.getControlPoints()[i], ngl::Vec3(1.0f, 1.0f, 1.0f));
    }
    // Fails to add more than 4 control points
    EXPECT_FALSE(b.setControlPoint(4, ngl::Vec3(1.0f, 1.0f, 1.0f)));
}

TEST(Blade, setControlPoints)
{
    Blade b;
    std::vector<ngl::Vec3> bladeControlPoints = { {0.0f,  0.0f,  0.0f},
                                              {0.0f,  7.0f,  1.0f},
                                              {0.0f,  9.0f,  4.0f},
                                              {0.0f,  10.0f, 6.0f} };
    EXPECT_TRUE(b.setControlPoints(bladeControlPoints));
    EXPECT_EQ(b.getControlPoints(), bladeControlPoints);
    // Fails if less/more than 4 control points
    EXPECT_FALSE(b.setControlPoints({ngl::Vec3(0.0f, 0.0f, 0.0f)}));
}

TEST(Blade, segmentVector)
{
    Blade b;
    std::vector<ngl::Vec3> bladeControlPoints = { {0.0f,  0.0f,  0.0f},
                                                {0.0f,  1.0f,  0.0f},
                                                {0.0f,  2.0f,  0.0f},
                                                {0.0f,  2.0f,  1.0f} };
    b.setControlPoints(bladeControlPoints);
    EXPECT_EQ(b.getSegmentVectors()[0], ngl::Vec3(0.0f, 1.0f, 0.0f));
    EXPECT_EQ(b.getSegmentVectors()[1], ngl::Vec3(0.0f, 1.0f, 0.0f));
    EXPECT_EQ(b.getSegmentVectors()[2], ngl::Vec3(0.0f, 0.0f, 1.0f));

}

TEST(Blade, surfaceVector)
{
    Blade b;
    std::vector<ngl::Vec3> bladeControlPoints = { {0.0f,  0.0f,  0.0f},
                                                  {0.0f,  1.0f,  0.0f},
                                                  {0.0f,  2.0f,  0.0f},
                                                  {0.0f,  3.0f,  -1.0f} };
    b.setControlPoints(bladeControlPoints);
    EXPECT_EQ(b.getSurfaceVectors()[0], ngl::Vec3(0.1f, 0.0f, 0.0f));
    EXPECT_EQ(b.getSurfaceVectors()[1], ngl::Vec3(0.09f, 0.0f, 0.0f));
    EXPECT_EQ(b.getSurfaceVectors()[2], ngl::Vec3(0.05f, 0.0f, 0.0f));

}

TEST(Blade, normalVector)
{
    Blade b;
    std::vector<ngl::Vec3> bladeControlPoints = { {0.0f,  0.0f,  0.0f},
                                                  {0.0f,  1.0f,  0.0f},
                                                  {0.0f,  2.0f,  0.0f},
                                                  {0.0f,  3.0f,  -1.0f} };
    b.setControlPoints(bladeControlPoints);
    EXPECT_EQ(b.getNormalVectors()[0], ngl::Vec3(0.0f, 0.0f, -1.0f));
    EXPECT_EQ(b.getNormalVectors()[1], ngl::Vec3(0.0f, 0.0f, -1.0f));
    EXPECT_EQ(b.getNormalVectors()[2], ngl::Vec3(0.0f, -0.05/sqrt(0.005), -0.05/sqrt(0.005)));
}