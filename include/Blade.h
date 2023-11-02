#ifndef BLADE_H
#define BLADE_H

#include <vector>

// Placeholder point struct - will potentially replace with NGL::Vec3 (or other) later
struct Point
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Point() = default;
    Point(float _x, float _y, float _z) : x{_x}, y{_y}, z{_z} {}
};

class Blade
{
public:
    Blade();
    [[nodiscard]] std::vector<Point> getControlPoints() const;
    bool addControlPoint(int _index, Point _point);
private:
    std::vector<Point> m_controlPoints;
};

#endif
