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
            EXPECT_EQ(plant.getBlades()[b].getControlPoints()[i].x, 0.0f);
            EXPECT_EQ(plant.getBlades()[b].getControlPoints()[i].y, 0.0f);
            EXPECT_EQ(plant.getBlades()[b].getControlPoints()[i].z, 0.0f);
        }
    }
}

TEST(Grass, getControlPoints)
{
    Grass plant(2);
    for(auto p : plant.getControlPoints())
    {
        p.x = 0.0f;
        p.y = 0.0f;
        p.z = 0.0f;
    }
}
