#ifndef GRASS_H_
#define GRASS_H_

#include <memory>

// Placeholder point struct - will replace with NCCA Vec3 (or other) later
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
    [[nodiscard]] int bladesNum() const;
    [[nodiscard]] Point* getControlPoints() const;
    void setControlPoints(Point* _controlPoints);

private:
    int m_bladesNum;
    std::unique_ptr<Point []> m_controlPoints;
};

#endif