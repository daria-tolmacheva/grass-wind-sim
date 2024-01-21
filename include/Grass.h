#ifndef GRASS_H_
#define GRASS_H_

#include <vector>
#include <ngl/Vec3.h>
#include <ngl/AbstractVAO.h>
#include "Blade.h"

class Grass
{
public:
    explicit Grass(int _bladesNum);
    [[nodiscard]] int bladesNum() const;
    [[nodiscard]] std::vector<Blade> getBlades() const;
    std::vector<ngl::Vec3> getControlPoints() const;
    bool setBlades(std::vector<ngl::Vec3> _controlPoints);
    bool setBlade(int _index, std::vector<ngl::Vec3> _controlPoints);
    void draw() const;
    void createVAO();

private:
    std::vector<Blade> m_blades;
    std::unique_ptr<ngl::AbstractVAO> m_vao;
};

#endif