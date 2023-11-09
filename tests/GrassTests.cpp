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
        p.x = 0.0f;
        p.y = 0.0f;
        p.z = 0.0f;
    }
}
//
//TEST(Grass, setBlade)
//{
//    Grass plant(4);
//    plant.setBlade();
//
//}
//TEST(Grass, setCorrectControlPoints)
//{
//    Grass plant(4);
//    std::vector<Point> plantControlPoints = { {0.0f,  0.0f,  0.0f}, // first blade
//                                              {0.0f,  7.0f,  1.0f},
//                                              {0.0f,  9.0f,  4.0f},
//                                              {0.0f,  10.0f, 6.0f},
//                                              {0.0f,  0.0f,  0.0f}, // second blade
//                                              {0.0f,  7.0f,  0.0f},
//                                              {10.0f, 10.0f, 0.0f},
//                                              {3.0f,  14.0f, 0.0f},
//                                              {0.0f,  0.0f,  0.0f}, // third blade
//                                              {0.0f,  4.0f, -0.5f},
//                                              {0.0f,  4.5f, -5.0f},
//                                              {0.0f,  1.5f, -11.5f},
//                                              {0.0f,  0.0f,  0.0f}, // forth blade
//                                              {-1.0f, 8.0f,  0.0f},
//                                              {-3.5f, 3.5f,  0.0f},
//                                              {-1.5f, 1.5f,  0.0f} };
//    plant.setControlPoints(plantControlPoints);
//    for(int i = 0; i < plant.bladesNum() * 4; ++i)
//    {
//        EXPECT_EQ(plant.getControlPoints()[i].x, plantControlPoints[i].x);
//        EXPECT_EQ(plant.getControlPoints()[i].y, plantControlPoints[i].y);
//        EXPECT_EQ(plant.getControlPoints()[i].z, plantControlPoints[i].z);
//    }
//}
//
//TEST(Grass, ctorVector)
//{
//    Grass plant(std::vector<Point>{{0.0f, 0.0f, 0.0f}, // single blade
//                                   {0.0f, 0.0f, 0.0f},
//                                   {0.0f, 0.0f, 0.0f},
//                                   {0.0f, 0.0f, 0.0f}});
//    EXPECT_EQ(plant.bladesNum(), 1);
//    for (int i = 0; i < plant.bladesNum() * 4; ++i)
//    {
//        EXPECT_EQ(plant.getControlPoints()[i].x, 0.0f);
//        EXPECT_EQ(plant.getControlPoints()[i].y, 0.0f);
//        EXPECT_EQ(plant.getControlPoints()[i].z, 0.0f);
//    }
//}