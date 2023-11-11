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

std::vector<Point> Grass::getControlPoints() const
{
    std::vector<Point> controlPoints;
    for(auto blade : m_blades)
    {
        for(int i = 0; i < Blade::controlPointsNum; ++i)
        controlPoints.push_back(blade.getControlPoints()[i]);
    }
    return controlPoints;
}

bool Grass::setBlades(std::vector<Point> _controlPoints)
{
    if((_controlPoints.size()/Blade::controlPointsNum) != m_blades.size())
        return false;
    for(int i = 0; i < m_blades.size(); ++i)
    {
        std::vector<Point> bladeControlPoints(&_controlPoints[i * Blade::controlPointsNum],
                                              &_controlPoints[i * Blade::controlPointsNum + Blade::controlPointsNum]);
        if(!m_blades[i].setControlPoints(bladeControlPoints))
        {
            return false;
        }
    }
    return true;
}

bool Grass::setBlade(int _index, std::vector<Point> _controlPoints)
{
    if(_index >= m_blades.size() || _controlPoints.size() != Blade::controlPointsNum)
        return false;
    return m_blades[_index].setControlPoints(_controlPoints);
}