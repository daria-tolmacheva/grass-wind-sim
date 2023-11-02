#ifndef GRASS_H_
#define GRASS_H_

#include <vector>
#include "Blade.h"

class Grass
{
public:
    explicit Grass(int _bladesNum);
    [[nodiscard]] int bladesNum() const;
    [[nodiscard]] std::vector<Blade> getBlades() const;
    std::vector<Point> getControlPoints() const;
//    void setBlades(std::vector<Point> _controlPoints);

private:
    std::vector<Blade> m_blades;
};

#endif