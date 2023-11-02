#ifndef GRASS_H_
#define GRASS_H_

#include <vector>

// Placeholder point struct - will potentially replace with NGL::Vec3 (or other) later
struct Point
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

class Grass
{
public:
    explicit Grass(int _bladesNum);
    Grass(std::vector<Point> _controlPoints);
    [[nodiscard]] int bladesNum() const;
    [[nodiscard]] std::vector<Point> getControlPoints() const;
    void setControlPoints(std::vector<Point> _controlPoints);

private:
    int m_bladesNum;
    std::vector<Point> m_controlPoints;
};

#endif