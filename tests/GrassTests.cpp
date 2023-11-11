#include <gtest/gtest.h>

#include "Grass.h"

TEST(Grass, ctorBladesNum)
{
    Grass plant(4);
    EXPECT_EQ(plant.bladesNum(), 4);
    // 4 Bezier curve control points per blade
    for (int b = 0; b < plant.bladesNum(); ++b) {
        for(int i = 0; i < 4; ++i)
        {
            EXPECT_EQ(plant.getBlades()[b].getControlPoints()[i], Point(0.0f, 0.0f, 0.0f));
        }
    }
}

TEST(Grass, getControlPoints)
{
    Grass plant(2);
    for(auto p : plant.getControlPoints())
    {
        EXPECT_EQ(p, Point(0.0f, 0.0f, 0.0f));
    }
}

TEST(Grass, setBlades)
{
    Grass plant(4);
    std::vector<Point> plantControlPoints = { {0.0f,  0.0f,  0.0f}, // first blade
                                              {0.0f,  7.0f,  1.0f},
                                              {0.0f,  9.0f,  4.0f},
                                              {0.0f,  10.0f, 6.0f},
                                              {0.0f,  0.0f,  0.0f}, // second blade
                                              {0.0f,  7.0f,  0.0f},
                                              {10.0f, 10.0f, 0.0f},
                                              {3.0f,  14.0f, 0.0f},
                                              {0.0f,  0.0f,  0.0f}, // third blade
                                              {0.0f,  4.0f, -0.5f},
                                              {0.0f,  4.5f, -5.0f},
                                              {0.0f,  1.5f, -11.5f},
                                              {0.0f,  0.0f,  0.0f}, // forth blade
                                              {-1.0f, 8.0f,  0.0f},
                                              {-3.5f, 3.5f,  0.0f},
                                              {-1.5f, 1.5f,  0.0f} };
    EXPECT_TRUE(plant.setBlades(plantControlPoints));
    EXPECT_EQ(plant.getControlPoints(), plantControlPoints);
    // Fails if less/more than 4 control points per blade
    EXPECT_FALSE(plant.setBlades({Point(1.0f, 1.0f, 1.0f)}));
}

TEST(Grass, setBlade)
{
    Grass plant(1);
    std::vector<Point> bladeControlPoints = { {0.0f,  0.0f,  0.0f}, // first blade
                                              {0.0f,  7.0f,  1.0f},
                                              {0.0f,  9.0f,  4.0f},
                                              {0.0f,  10.0f, 6.0f} };
    EXPECT_TRUE(plant.setBlade(0, bladeControlPoints));
    EXPECT_EQ(plant.getControlPoints(), bladeControlPoints);
    // Fails to set blade with less/more than 4 control points
    EXPECT_FALSE(plant.setBlade(0, {Point(1.0f, 1.0f, 1.0f)}));
    // Fails to set blade outside of blades number index
    EXPECT_FALSE(plant.setBlade(3, bladeControlPoints));

}