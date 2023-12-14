#include <ngl/VAOFactory.h>
#include <ngl/SimpleIndexVAO.h>
#include <iostream>
#include "Grass.h"

Grass::Grass(int _bladesNum)
{
    m_blades.resize(_bladesNum);
}

int Grass::bladesNum() const
{
    return m_blades.size();
}

std::vector<Blade> Grass::getBlades() const
{
    return m_blades;
}

std::vector<ngl::Vec3> Grass::getControlPoints() const
{
    std::vector<ngl::Vec3> controlPoints;
    for(auto blade : m_blades)
    {
        for(int i = 0; i < Blade::controlPointsNum; ++i)
        controlPoints.push_back(blade.getControlPoints()[i]);
    }
    return controlPoints;
}

bool Grass::setBlades(std::vector<ngl::Vec3> _controlPoints)
{
    if((_controlPoints.size()/Blade::controlPointsNum) != m_blades.size())
        return false;
    for(int i = 0; i < m_blades.size(); ++i)
    {
        std::vector<ngl::Vec3> bladeControlPoints(&_controlPoints[i * Blade::controlPointsNum],
                                              &_controlPoints[i * Blade::controlPointsNum + Blade::controlPointsNum]);
        if(!m_blades[i].setControlPoints(bladeControlPoints))
        {
            return false;
        }
    }
    m_vao = ngl::VAOFactory::createVAO(ngl::simpleIndexVAO, GL_LINE_STRIP_ADJACENCY);
    return true;
}

bool Grass::setBlade(int _index, std::vector<ngl::Vec3> _controlPoints)
{
    if(_index >= m_blades.size() || _controlPoints.size() != Blade::controlPointsNum)
        return false;
    return m_blades[_index].setControlPoints(_controlPoints);
}

void Grass::draw() const
{
  m_vao->bind();
  m_vao->setMode(GL_LINE_STRIP_ADJACENCY);
  m_vao->draw();
  m_vao->unbind();
}

void Grass::createVAO()
{
    std::vector<ngl::Vec3> controlPoints = getControlPoints();
    constexpr GLshort restart = 9999;
    size_t j = 0;
    std::vector<GLshort> index(controlPoints.size()/4 * 5 - 1);
    for(size_t i = 0; i < index.size() - 3; i = i + 5) // this loop looks horrible but it works
    {                                                  // might refactor later to make it readable
        index[i] = j * 4;
        index[i + 1] = j * 4 + 1;
        index[i + 2] = j * 4 + 2;
        index[i + 3] = j * 4 + 3;
        if (i != index.size() - 4) {
          index[i + 4] = restart;
        }
        ++j;
    }

    m_vao->bind();
    m_vao->setData(ngl::SimpleIndexVAO::VertexData(
              controlPoints.size() * sizeof(ngl::Vec3),
              controlPoints[0].m_x,
              index.size(), &index[0],
              GL_UNSIGNED_SHORT));
    m_vao->setVertexAttributePointer(0, 3, GL_FLOAT, 0, 0);
    m_vao->setNumIndices(index.size());
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(restart);

    // now unbind
    m_vao->unbind();
}
