#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <vector>
#include <gtest/gtest.h>
#include <ngl/Vec3.h>

enum Field
{
    X_FIELD,
    Y_FIELD,
    Z_FIELD
};
enum Plane
{
    Z_POS, // z positive
    Z_NEG, // z negative
    X_POS, // x positive
    X_NEG  // x negative
};

class Simulation
{
    friend class SimulationTest;
    FRIEND_TEST(Simulation, ctor);
    FRIEND_TEST(Simulation, to1D);
    FRIEND_TEST(Simulation, setVelocity);
    FRIEND_TEST(Simulation, integrate);
    FRIEND_TEST(Simulation, solveIncompressibility);
    FRIEND_TEST(Simulation, extrapolate);
    FRIEND_TEST(Simulation, avgVelocityX);
    FRIEND_TEST(Simulation, avgVelocityY);
    FRIEND_TEST(Simulation, avgVelocityZ);
    FRIEND_TEST(Simulation, sampleField);
    FRIEND_TEST(Simulation, advectVelocity);
public:
    Simulation(float _density, int _x, int _y, int _z, float _cellSize);

    void simulate(float _dt, int _numIter);
    void setVelocity(float _value, Plane _entryPlane);

private:
    static constexpr float gravity = 9.8;
    static constexpr float overRelaxation = 1.9;

    float m_density;
    ngl::Vec3 m_dimensions;
    int m_cellNum;
    float m_cellSize; // h
    std::vector<ngl::Vec3> m_velocity; // u, v -> vec3 for 3 directions
    std::vector<ngl::Vec3> m_newVelocity; // newU, newV -> vec3 for 3 directions
    std::vector<float> m_p;
    std::vector<float> m_s;

    // Utility functions
    [[nodiscard]] int to1D(int _x, int _y, int _z) const;
    //ngl::Vec3 to3D(int _index);

    void integrate(float _dt, float _gravity);
    void solveIncompressibility(int _numIters, float _dt);
    void extrapolate();
    [[nodiscard]] float avgVelocityX(int _i, int _j, int _k) const;
    [[nodiscard]] float avgVelocityY(int _i, int _j, int _k) const;
    [[nodiscard]] float avgVelocityZ(int _i, int _j, int _k) const;
    float sampleField(int _i, int _j, int _k, Field _field);
    void advectVelocity(float _dt);
};

#endif