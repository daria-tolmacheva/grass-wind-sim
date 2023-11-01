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
        EXPECT_EQ(plant.getControlPoints()[i].x, 0);
        EXPECT_EQ(plant.getControlPoints()[i].y, 0);
        EXPECT_EQ(plant.getControlPoints()[i].z, 0);
    }
}

