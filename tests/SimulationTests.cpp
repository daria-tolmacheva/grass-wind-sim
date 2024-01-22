#include <gtest/gtest.h>
#include <ngl/Vec3.h>

#include "Simulation.h"

TEST(Simulation, ctor)
{
    Simulation sim(10.0f, 5, 5, 5, 1.0f);
    EXPECT_FLOAT_EQ(sim.m_density, 10.0f);
    EXPECT_EQ(sim.m_dimensions, ngl::Vec3(7, 7, 7));
    EXPECT_EQ(sim.m_cellNum, 343);
    EXPECT_FLOAT_EQ(sim.m_cellSize, 1.0f);
    EXPECT_EQ(sim.m_velocity.size(), 343);
    EXPECT_EQ(sim.m_newVelocity.size(), 343);
    EXPECT_EQ(sim.m_p.size(), 343);
    EXPECT_EQ(sim.m_s.size(), 343);
}

TEST(Simulation, to1D)
{
    Simulation sim(10.0f, 3, 3, 3, 1.0f);
    EXPECT_EQ(sim.to1D(0, 0, 0), 0);
    EXPECT_EQ(sim.to1D(2, 3, 0), 17);
    EXPECT_EQ(sim.to1D(1, 1, 1), 31);
    EXPECT_EQ(sim.to1D(3, 3, 2), 68);
    EXPECT_EQ(sim.to1D(3, 2, 3), 88);
    EXPECT_EQ(sim.to1D(4, 4, 4), 124);
}

TEST(Simulation, setVelocity)
{
    Simulation sim(10.0f, 3, 4, 5, 1.0f);
    sim.setVelocity(2.0f, Z_POS);
    for (int i = 0; i < sim.m_dimensions.m_x; ++i) {
        for (int j = 0; j < sim.m_dimensions.m_y; ++j) {
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(i, j, 1)].m_z, 2.0f);
        }
    }
    sim.setVelocity(2.0f, Z_NEG);
    for (int i = 0; i < sim.m_dimensions.m_x; ++i) {
        for (int j = 0; j < sim.m_dimensions.m_y; ++j) {
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(i, j, 1)].m_z, 2.0f);
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(i, j, static_cast<int>(sim.m_dimensions.m_z-1))].m_z, -2.0f);
        }
    }
    sim.setVelocity(2.0f, X_POS);
    for (int k = 0; k < sim.m_dimensions.m_z; ++k) {
        for (int j = 0; j < sim.m_dimensions.m_y; ++j) {
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(1, j, k)].m_x, 2.0f);
        }
    }
    sim.setVelocity(2.0f, X_NEG);
    for (int k = 0; k < sim.m_dimensions.m_z; ++k) {
        for (int j = 0; j < sim.m_dimensions.m_y; ++j) {
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(1, j, k)].m_x, 2.0f);
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(static_cast<int>(sim.m_dimensions.m_x-1), j, k)].m_x, -2.0f);
        }
    }
}

TEST(Simulation, resetVelocity)
{
    Simulation sim(10.0f, 3, 4, 5, 1.0f);
    sim.setVelocity(2.0f, Z_POS);
    sim.resetVelocity();
    EXPECT_FLOAT_EQ(sim.avgVelocityZ(1, 1, 1), 0.0);
}

TEST(Simulation, integrate)
{
    Simulation sim(10.0f, 3, 4, 5, 1.0f);
    sim.setVelocity(2.0f, Z_POS);
    sim.integrate(0.1f, Simulation::gravity);
    for (int i = 1; i < sim.m_dimensions.m_x-1; ++i) {
        for (int j = 1; j < sim.m_dimensions.m_y-2; ++j) {
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(i, j, 1)].m_y, 0.98f);
        }
        EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(i, sim.m_dimensions.m_y-1, 1)].m_y, 0.0f);
    }
}

TEST(Simulation, extrapolate)
{
    Simulation sim(10.0f, 3, 4, 5, 1.0f);
    sim.setVelocity(2.0f, Z_POS);
    sim.solveIncompressibility(40, 0.1f);
    sim.extrapolate();
    for (int i = 0; i < sim.m_dimensions.m_x; ++i)
    {
        for (int j = 0; j < sim.m_dimensions.m_y; ++j)
        {
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(i, j, 0)].m_x, sim.m_velocity[sim.to1D(i, j, 1)].m_x);
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(i, j, 0)].m_y, sim.m_velocity[sim.to1D(i, j, 1)].m_y);
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(i, j, sim.m_dimensions.m_z-1)].m_x, sim.m_velocity[sim.to1D(i, j, sim.m_dimensions.m_z-2)].m_x);
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(i, j, sim.m_dimensions.m_z-1)].m_y, sim.m_velocity[sim.to1D(i, j, sim.m_dimensions.m_z-2)].m_y);
        }
    }
    for (int i = 0; i < sim.m_dimensions.m_x; ++i)
    {
        for (int k = 0; k < sim.m_dimensions.m_z; ++k)
        {
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(i, 0, k)].m_x, sim.m_velocity[sim.to1D(i, 1, k)].m_x);
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(i, 0, k)].m_z, sim.m_velocity[sim.to1D(i, 1, k)].m_z);
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(i, sim.m_dimensions.m_y-1, k)].m_x, sim.m_velocity[sim.to1D(i, sim.m_dimensions.m_z-2, k)].m_x);
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(i, sim.m_dimensions.m_y-1, k)].m_z, sim.m_velocity[sim.to1D(i, sim.m_dimensions.m_z-2, k)].m_z);
        }
    }
    for (int j = 0; j < sim.m_dimensions.m_y; ++j)
    {
        for (int k = 0; k < sim.m_dimensions.m_z; ++k)
        {
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(0, j, k)].m_y, sim.m_velocity[sim.to1D(1, j, k)].m_y);
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(0, j, k)].m_z, sim.m_velocity[sim.to1D(1, j, k)].m_z);
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(sim.m_dimensions.m_x-1, j, k)].m_y, sim.m_velocity[sim.to1D(sim.m_dimensions.m_x-2, j, k)].m_y);
            EXPECT_FLOAT_EQ(sim.m_velocity[sim.to1D(sim.m_dimensions.m_x-1, j, k)].m_z, sim.m_velocity[sim.to1D(sim.m_dimensions.m_x-2, j, k)].m_z);
        }
    }
}

TEST(Simulation, avgVelocityX)
{
    Simulation sim(10.0f, 3, 4, 5, 1.0f);
    sim.setVelocity(2.0f, X_POS);
    EXPECT_FLOAT_EQ(sim.avgVelocityX(1, 1, 1), 1.0);
}

TEST(Simulation, avgVelocityY)
{
    Simulation sim(10.0f, 3, 4, 5, 1.0f);
    sim.integrate(0.1f, Simulation::gravity);
    EXPECT_FLOAT_EQ(sim.avgVelocityY(1, 1, 1), 0.245f);
}

TEST(Simulation, avgVelocityZ)
{
    Simulation sim(10.0f, 3, 4, 5, 1.0f);
    sim.setVelocity(2.0f, Z_POS);
    EXPECT_FLOAT_EQ(sim.avgVelocityZ(1, 1, 1), 1.0);
}

