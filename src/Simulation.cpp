/*
 * Class based on fluid simulation implementation by Matthias Müller
 * Available here:
 * https://github.com/matthias-research/pages/blob/master/tenMinutePhysics/17-fluidSim.html
 * (Adapted to 3d and C++).
*/

#include "Simulation.h"

Simulation::Simulation(float _density, int _x, int _y, int _z, float _cellSize)
{
    m_density = _density;
    m_dimensions = ngl::Vec3(_x + 2, _y + 2, _z + 2); // include padding
    m_cellNum = (_x+2) * (_y+2) * (_z+2);
    m_cellSize = _cellSize; // h
    m_velocity.resize(m_cellNum); // u, v -> vec3 for 3 directions
    std::fill_n(m_velocity.begin(), m_cellNum, ngl::Vec3(0.0f, 0.0f, 0.0f));
    m_newVelocity.resize(m_cellNum); // newU, newV -> vec3 for 3 directions
    std::fill_n(m_newVelocity.begin(), m_cellNum, ngl::Vec3(0.0f, 0.0f, 0.0f));
    m_p.resize(m_cellNum);
    std::fill_n(m_p.begin(), m_cellNum, 0.0f);
    m_s.resize(m_cellNum);
    std::fill_n(m_s.begin(), m_cellNum, 1.0f);
}

/*
 * 3D to 1D conversion of grid/array indexes and back based on solution from Samuel Kerrien (Dec 18, 2015)
 * Available here on SctackOverflow (Accessed Jan 2024):
 * https://stackoverflow.com/questions/7367770/how-to-flatten-or-index-3d-array-in-1d-array
 */
int Simulation::to1D(int _x, int _y, int _z) const
{
    return (_z * static_cast<int>(m_dimensions.m_x) * static_cast<int>(m_dimensions.m_y))
            + (_y * static_cast<int>(m_dimensions.m_x)) + _x;
}

// Apply gravity to velocity
void Simulation::integrate(float _dt, float _gravity)
{
    for(size_t k = 1; k < (m_dimensions.m_z-1); ++k)
    {
        for(size_t j = 1; j < (m_dimensions.m_y-2); ++j)
        {
            for(size_t i = 1; i < (m_dimensions.m_x-1); ++i)
            {
                if (m_s[to1D(i, j, k)] != 0.0 && m_s[to1D(i, j - 1, k)] != 0.0)
                {
                    m_velocity[to1D(i, j, k)].m_y += gravity * _dt;
                }
            } // k
        } // j
    } // i
}

void Simulation::solveIncompressibility(int _numIters, float _dt)
{
    float cp = m_density * m_cellSize / _dt;

    for (size_t iter = 0; iter < _numIters; ++iter) {

        for (size_t k = 1; k < (m_dimensions.m_z - 1); ++k)
        {
            for (size_t j = 1; j < (m_dimensions.m_y - 1); ++j)
            {
                for (size_t i = 1; i < (m_dimensions.m_x - 1); ++i)
                {

                    if (m_s[to1D(i, j, k)] == 0.0)
                        continue;

                    float sx0 = m_s[to1D(i-1, j, k)];
                    float sx1 = m_s[to1D(i+1, j, k)];
                    float sy0 = m_s[to1D(i, j-1, k)];
                    float sy1 = m_s[to1D(i, j+1, k)];
                    float sz0 = m_s[to1D(i, j, k+1)];
                    float sz1 = m_s[to1D(i, j, k-1)];
                    float s = sx0 + sx1 + sy0 + sy1 + sz0 + sz1;
                    if (s == 0.0)
                        continue;

                    float div = m_velocity[to1D(i+1, j, k)].m_x - m_velocity[to1D(i, j, k)].m_x +
                                m_velocity[to1D(i, j+1, k)].m_y - m_velocity[to1D(i, j, k)].m_y +
                                m_velocity[to1D(i, j, k+1)].m_z - m_velocity[to1D(i, j, k)].m_z;

                    float p = -div / s;
                    p *= overRelaxation;
                    m_p[to1D(i, j, k)] += cp * p;

                    m_velocity[to1D(i, j, k)].m_x -= sx0 * p;
                    m_velocity[to1D(i+1, j, k)].m_x += sx1 * p;
                    m_velocity[to1D(i, j, k)].m_y -= sy0 * p;
                    m_velocity[to1D(i, j+1, k)].m_y += sy1 * p;
                    m_velocity[to1D(i, j, k)].m_z -= sz0 * p;
                    m_velocity[to1D(i, j, k+1)].m_z += sz1 * p;
                } // i
            } // j
        } // k
    } // iter
}

void Simulation::extrapolate()
{
    // top and bottom sides
    for(size_t i = 0; i < m_dimensions.m_x; ++i)
    {
        for(size_t k = 0; k < m_dimensions.m_z; ++k)
        {
            // top
            m_velocity[to1D(i, 0, k)].m_x = m_velocity[to1D(i, 1, k)].m_x;
            m_velocity[to1D(i, 0, k)].m_z = m_velocity[to1D(i, 1, k)].m_z;
            // bottom
            m_velocity[to1D(i, m_dimensions.m_y-1, k)].m_x = m_velocity[to1D(i, m_dimensions.m_y-2, k)].m_x;
            m_velocity[to1D(i, m_dimensions.m_y-1, k)].m_z = m_velocity[to1D(i, m_dimensions.m_y-2, k)].m_z;
        }
    }

    // front and back sides
    for(size_t i = 0; i < m_dimensions.m_x; ++i)
    {
        for(size_t j = 0; j < m_dimensions.m_y; ++j)
        {
            // front
            m_velocity[to1D(i, j, 0)].m_x = m_velocity[to1D(i, j, 1)].m_x;
            m_velocity[to1D(i, j, 0)].m_y = m_velocity[to1D(i, j, 1)].m_y;

            // back
            m_velocity[to1D(i, j, m_dimensions.m_z-1)].m_x = m_velocity[to1D(i, j, m_dimensions.m_z-2)].m_x;
            m_velocity[to1D(i, j, m_dimensions.m_z-1)].m_y = m_velocity[to1D(i, j, m_dimensions.m_z-2)].m_y;
        }
    }

    // left and right sides
    for(size_t j = 0; j < m_dimensions.m_y; ++j)
    {
        for(size_t k = 0; k < m_dimensions.m_z; ++k)
        {
            // left
            m_velocity[to1D(0, j, k)].m_y = m_velocity[to1D(1, j, k)].m_y;
            m_velocity[to1D(0, j, k)].m_z = m_velocity[to1D(1, j, k)].m_z;

            // right
            m_velocity[to1D(m_dimensions.m_x-1, j, k)].m_y = m_velocity[to1D(m_dimensions.m_x-2, j, k)].m_y;
            m_velocity[to1D(m_dimensions.m_x-1, j, k)].m_z = m_velocity[to1D(m_dimensions.m_x-2, j, k)].m_z;
        }
    }
}

float Simulation::avgVelocityX(int _i, int _j, int _k) const
{
    return (m_velocity[to1D(_i, _j, _k)].m_x + m_velocity[to1D(_i, _j-1, _k)].m_x +
           m_velocity[to1D(_i, _j, _k-1)].m_x + m_velocity[to1D(_i, _j-1, _k-1)].m_x +
           m_velocity[to1D(_i+1, _j, _k)].m_x + m_velocity[to1D(_i+1, _j-1, _k)].m_x +
           m_velocity[to1D(_i+1, _j, _k-1)].m_x + m_velocity[to1D(_i+1, _j-1, _k-1)].m_x) * 0.125f;
}

float Simulation::avgVelocityY(int _i, int _j, int _k) const
{
    return (m_velocity[to1D(_i, _j, _k)].m_y + m_velocity[to1D(_i-1, _j, _k)].m_y +
            m_velocity[to1D(_i, _j, _k-1)].m_y + m_velocity[to1D(_i-1, _j, _k-1)].m_y +
            m_velocity[to1D(_i, _j+1, _k)].m_y + m_velocity[to1D(_i-1, _j+1, _k)].m_y +
            m_velocity[to1D(_i, _j+1, _k-1)].m_y + m_velocity[to1D(_i-1, _j+1, _k-1)].m_y) * 0.125f;
}

float Simulation::avgVelocityZ(int _i, int _j, int _k) const
{
    return (m_velocity[to1D(_i, _j, _k)].m_z + m_velocity[to1D(_i-1, _j, _k)].m_z +
            m_velocity[to1D(_i, _j-1, _k)].m_z + m_velocity[to1D(_i-1, _j-1, _k)].m_z +
            m_velocity[to1D(_i, _j, _k+1)].m_z + m_velocity[to1D(_i-1, _j, _k+1)].m_z +
            m_velocity[to1D(_i, _j-1, _k+1)].m_z + m_velocity[to1D(_i-1, _j-1, _k+1)].m_z) * 0.125f;
}

float Simulation::sampleField(int _i, int _j, int _k, Field _field)
{
    // Check for bounds
    if (_i < 0 || _j < 0 || _k < 0)
    {
        return 0.0f;
    }

    float h1 = 1.0f / m_cellSize;
    float h2 = 0.5f * m_cellSize;

    float x = std::max(std::min(static_cast<float>(_i), m_dimensions.m_x * m_cellSize), m_cellSize);
    float y = std::max(std::min(static_cast<float>(_j), m_dimensions.m_y * m_cellSize), m_cellSize);
    float z = std::max(std::min(static_cast<float>(_k), m_dimensions.m_z * m_cellSize), m_cellSize);

    float dx = 0.0;
    float dy = 0.0;
    float dz = 0.0;

    switch (_field) {
        case X_FIELD: dy = h2; dz = h2; break;
        case Y_FIELD: dx = h2; dz = h2; break;
        case Z_FIELD: dx = h2; dy = h2; break;
    }

    int x0 = static_cast<int>(std::min(std::floor((x-dx) * h1), m_dimensions.m_x - 1));
    int y0 = static_cast<int>(std::min(std::floor((y-dy) * h1), m_dimensions.m_y - 1));
    int z0 = static_cast<int>(std::min(std::floor((z-dx) * h1), m_dimensions.m_z - 1));

    float tx = ((x-dx) - x0 * m_cellSize) * h1;
    float ty = ((y-dy) - y0 * m_cellSize) * h1;
    float tz = ((z-dz) - z0 * m_cellSize) * h1;

    int x1 = std::min(x0 + 1, static_cast<int>(m_dimensions.m_x - 1));
    int y1 = std::min(y0 + 1, static_cast<int>(m_dimensions.m_y - 1));
    int z1 = std::min(z0 + 1, static_cast<int>(m_dimensions.m_z - 1));

    float sx = 1.0f - tx;
    float sy = 1.0f - ty;
    float sz = 1.0f - tz;

    float final_value = 0.0;

    switch (_field) {
        case X_FIELD:
            final_value = m_velocity[to1D(x0, y0, z0)].m_x * tx * ty * tz +
                          m_velocity[to1D(x1, y0, z0)].m_x * sx * ty * tz +
                          m_velocity[to1D(x0, y1, z0)].m_x * tx * sy * tz +
                          m_velocity[to1D(x1, y1, z0)].m_x * sx * sy * tz +
                          m_velocity[to1D(x0, y0, z1)].m_x * tx * ty * sz +
                          m_velocity[to1D(x1, y0, z1)].m_x * sx * ty * sz +
                          m_velocity[to1D(x0, y1, z1)].m_x * tx * sy * sz +
                          m_velocity[to1D(x1, y1, z1)].m_x * sx * sy * sz;
            break;
        case Y_FIELD:
            final_value = m_velocity[to1D(x0, y0, z0)].m_y * tx * ty * tz +
                          m_velocity[to1D(x1, y0, z0)].m_y * sx * ty * tz +
                          m_velocity[to1D(x0, y1, z0)].m_y * tx * sy * tz +
                          m_velocity[to1D(x1, y1, z0)].m_y * sx * sy * tz +
                          m_velocity[to1D(x0, y0, z1)].m_y * tx * ty * sz +
                          m_velocity[to1D(x1, y0, z1)].m_y * sx * ty * sz +
                          m_velocity[to1D(x0, y1, z1)].m_y * tx * sy * sz +
                          m_velocity[to1D(x1, y1, z1)].m_y * sx * sy * sz;
            break;
        case Z_FIELD:
            final_value = m_velocity[to1D(x0, y0, z0)].m_z * tx * ty * tz +
                          m_velocity[to1D(x1, y0, z0)].m_z * sx * ty * tz +
                          m_velocity[to1D(x0, y1, z0)].m_z * tx * sy * tz +
                          m_velocity[to1D(x1, y1, z0)].m_z * sx * sy * tz +
                          m_velocity[to1D(x0, y0, z1)].m_z * tx * ty * sz +
                          m_velocity[to1D(x1, y0, z1)].m_z * sx * ty * sz +
                          m_velocity[to1D(x0, y1, z1)].m_z * tx * sy * sz +
                          m_velocity[to1D(x1, y1, z1)].m_z * sx * sy * sz;
            break;
    }

    return final_value;

}

void Simulation::advectVelocity(float _dt)
{
    m_newVelocity = m_velocity;

    float h2 = m_cellSize * 0.5f;

    for (int k = 1; k < m_dimensions.m_z; ++k)
    {
        for (int j = 1; j < m_dimensions.m_y; ++j)
        {
            for (int i = 1; i < m_dimensions.m_x; ++i)
            {
                // x component
                if ((m_s[to1D(i, j, k)] != 0.0f)
                 && (m_s[to1D(i-1, j, k)] != 0.0f)
                 && (j < m_dimensions.m_y - 1)
                 && (k < m_dimensions.m_z - 1) )
                {
                    float x = i * m_cellSize;
                    float y = j * m_cellSize + h2;
                    float z = k * m_cellSize + h2;

                    float velocityX = m_velocity[to1D(i, j, k)].m_x;
                    float velocityY = avgVelocityY(i, j, k);
                    float velocityZ = avgVelocityZ(i, j, k);

                    x -= _dt * velocityX;
                    y -= _dt * velocityY;
                    z -= _dt * velocityZ;

                    m_velocity[to1D(i, j, k)].m_x = sampleField(i, j, k, X_FIELD);
                }
                // y component
                if ((m_s[to1D(i, j, k)] != 0.0f)
                 && (m_s[to1D(i, j-1, k)] != 0.0f)
                 && (i < m_dimensions.m_x - 1)
                 && (k < m_dimensions.m_z - 1) )
                {
                    float x = i * m_cellSize + h2;
                    float y = j * m_cellSize;
                    float z = k * m_cellSize + h2;

                    float velocityX = avgVelocityX(i, j, k);
                    float velocityY = m_velocity[to1D(i, j, k)].m_y;
                    float velocityZ = avgVelocityZ(i, j, k);

                    x -= _dt * velocityX;
                    y -= _dt * velocityY;
                    z -= _dt * velocityZ;

                    m_velocity[to1D(i, j, k)].m_y = sampleField(i, j, k, Y_FIELD);
                }
                // z component
                if ((m_s[to1D(i, j, k)] != 0.0f)
                 && (m_s[to1D(i, j, k-1)] != 0.0f)
                 && (i < m_dimensions.m_x - 1)
                 && (j < m_dimensions.m_y - 1) )
                {
                    float x = i * m_cellSize + h2;
                    float y = j * m_cellSize + h2;
                    float z = k * m_cellSize;

                    float velocityX = avgVelocityX(i, j, k);
                    float velocityY = avgVelocityY(i, j, k);
                    float velocityZ = m_velocity[to1D(i, j, k)].m_z;

                    x -= _dt * velocityX;
                    y -= _dt * velocityY;
                    z -= _dt * velocityZ;

                    m_velocity[to1D(i, j, k)].m_z = sampleField(i, j, k, Z_FIELD);
                }
            } // i
        } // j
    } // k
    m_velocity = m_newVelocity;
}

void Simulation::simulate(float _dt, int _numIter)
{
    integrate(_dt, gravity);
    std::fill_n(m_p.begin(), m_cellNum, 0.0);
    solveIncompressibility(_numIter, _dt);
    extrapolate();
    advectVelocity(_dt);
}

void Simulation::setVelocity(float _value, Plane _entryPlane)
{
    std::fill_n(m_velocity.begin(), m_cellNum, ngl::Vec3(0.0f, 0.0f, 0.0f));
    switch (_entryPlane) {
        case Z_POS:
            for (int i = 0; i < m_dimensions.m_x; ++i)
            {
                for (int j = 0; j < m_dimensions.m_y; ++j)
                {
                    m_velocity[to1D(i, j, 1)].m_z = _value;
                }
            }
            break;
        case Z_NEG:
            for (int i = 0; i < m_dimensions.m_x; ++i)
            {
                for (int j = 0; j < m_dimensions.m_y; ++j)
                {
                    m_velocity[to1D(i, j, static_cast<int>(m_dimensions.m_z-1))].m_z = -_value;
                }
            }
            break;
        case X_POS:
            for (int k = 0; k < m_dimensions.m_z; ++k)
            {
                for (int j = 0; j < m_dimensions.m_y; ++j)
                {
                    m_velocity[to1D(1, j, k)].m_x = _value;
                }
            }
            break;
        case X_NEG:
            for (int k = 0; k < m_dimensions.m_z; ++k)
            {
                for (int j = 0; j < m_dimensions.m_y; ++j)
                {
                    m_velocity[to1D(static_cast<int>(m_dimensions.m_x-1), j, k)].m_x = -_value;
                }
            }
            break;
    }
}