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
    return true;
}

bool Grass::setBlade(int _index, std::vector<ngl::Vec3> _controlPoints)
{
    if(_index >= m_blades.size() || _controlPoints.size() != Blade::controlPointsNum)
        return false;
    return m_blades[_index].setControlPoints(_controlPoints);
}

void Grass::draw(ngl::Mat4 MVP) const
{
  for(auto blade : m_blades)
  {
    blade.draw(MVP);
  }
}

