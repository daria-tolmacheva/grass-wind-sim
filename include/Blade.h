#ifndef BLADE_H
#define BLADE_H

#include <vector>

/// This function was originally written by Jon Macey
constexpr float EPSILON = 0.001f;
#define FCompare(a, b) \
  (((a)-EPSILON) < (b) && ((a) + EPSILON) > (b))
/// end of function

// Placeholder point struct - will potentially replace with NGL::Vec3 (or other) later
struct Point
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Point() = default;
    Point(float _x, float _y, float _z) : x{_x}, y{_y}, z{_z} {}

    bool operator==(const Point &_p) const
    {
        return(FCompare(x, _p.x)  &&
               FCompare(y, _p.y)  &&
               FCompare(z, _p.z));
    }

    Point operator-(const Point &_rhs) const
    {
        return {x - _rhs.x, y - _rhs.y, z - _rhs.z};
    }
};

class Blade
{
public:
    Blade();
    static constexpr int controlPointsNum = 4;
    [[nodiscard]] std::vector<Point> getControlPoints() const;
    bool setControlPoint(int _index, Point _point);
    bool setControlPoints(std::vector<Point> _points);
    [[nodiscard]] std::vector<Point> getSegmentVectors() const;
    [[nodiscard]] std::vector<Point> getSurfaceVectors() const;
    [[nodiscard]] std::vector<Point> getNormalVectors() const;
private:
    std::vector<Point> m_controlPoints;
    // For each of 3 blade segments we need to store 3 types of vectors to describe its orientation
    std::vector<Point> m_segmentVector; // Vector connecting 2 adjacent control points
    std::vector<Point> m_surfaceVector; // Orthogonal to segment vector and on the blade plane
    std::vector<Point> m_normalVector; // Unit vector orthogonal to both segment and surface vectors
};

#endif
