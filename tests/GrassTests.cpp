#include <gtest/gtest.h>
#include "Grass.h"

TEST(Grass, ctor)
{
    Grass plant(4);
    int bladesNumber = plant.bladesNum();
    EXPECT_EQ(bladesNumber, 4);
    // 4 Bezier curve control points per blade
    for (int i = 0; i < bladesNumber * 4; ++i)
    {
        EXPECT_EQ(plant.getControlPoints()[i].x, 0.0f);
        EXPECT_EQ(plant.getControlPoints()[i].y, 0.0f);
        EXPECT_EQ(plant.getControlPoints()[i].z, 0.0f);
    }
}

TEST (Grass, setControlPoints)
{
    Grass plant(4);
    std::unique_ptr<Point []> plantControlPoints(new Point[16] { { 0.0f,  0.0f,  0.0f}, // first blade
                                                                    { 0.0f,  7.0f,  1.0f},
                                                                    { 0.0f,  9.0f,  4.0f},
                                                                    { 0.0f,  10.0f, 6.0f},
                                                                    { 0.0f,  0.0f,  0.0f}, // second blade
                                                                    { 0.0f,  7.0f,  0.0f},
                                                                    { 10.0f, 10.0f, 0.0f},
                                                                    { 3.0f,  14.0f, 0.0f},
                                                                    { 0.0f,  0.0f,  0.0f}, // third blade
                                                                    { 0.0f,  4.0f, -0.5f},
                                                                    { 0.0f,  4.5f, -5.0f},
                                                                    { 0.0f,  1.5f, -11.5f},
                                                                    { 0.0f,  0.0f,  0.0f}, // forth blade
                                                                    { -1.0f, 8.0f,  0.0f},
                                                                    { -3.5f, 3.5f,  0.0f},
                                                                    { -1.5f, 1.5f,  0.0f} });
    plant.setControlPoints(plantControlPoints.get());
    for(int i = 0; i < plant.bladesNum() * 4; ++i)
    {
        EXPECT_EQ(plant.getControlPoints()[i].x, plantControlPoints[i].x);
        EXPECT_EQ(plant.getControlPoints()[i].y, plantControlPoints[i].y);
        EXPECT_EQ(plant.getControlPoints()[i].z, plantControlPoints[i].z);
    }
}