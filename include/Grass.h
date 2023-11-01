#ifndef GRASS_H_
#define GRASS_H_

#include <memory>

// Placeholder point struct - will replace with NCCA Vec3 (or other) later
struct Point
{
    int x = 0;
    int y = 0;
    int z = 0;
};

class Grass
{
public:
    explicit Grass(int _bladesNum);
    [[nodiscard]] int bladesNum() const;
    [[nodiscard]] Point* getControlPoints() const;

private:
    int m_bladesNum;
    std::unique_ptr<Point []> m_controlPoints;
};

#endif