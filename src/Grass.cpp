#include "Grass.h"

Grass::Grass(int _bladesNum)
{
    m_blades.resize(_bladesNum); // 4 control points per blade
}

int Grass::bladesNum() const
{
    return m_blades.size();
}

std::vector<Blade> Grass::getBlades() const
{
    return m_blades;
}

std::vector<Point> Grass::getControlPoints() const
{
    std::vector<Point> controlPoints;
    for(auto blade : m_blades)
    {
        for(int i = 0; i < 4; ++i)
        controlPoints.push_back(blade.getControlPoints()[i]);
    }
    return controlPoints;
}

bool Grass::setBlades(std::vector<Point> _controlPoints)
{
    if((_controlPoints.size()/4) != m_blades.size())
        return false;
    for(int i = 0; i < m_blades.size(); ++i)
    {
        std::vector<Point> bladeControlPoints(&_controlPoints[i*4], &_controlPoints[i*4 + 4]);
        if(!m_blades[i].setControlPoints(bladeControlPoints))
        {
            return false;
        }
    }
    return true;
}

bool Grass::setBlade(int _index, std::vector<Point> _controlPoints)
{
    if(_index >= m_blades.size() || _controlPoints.size() != 4)
        return false;
    return m_blades[_index].setControlPoints(_controlPoints);
}