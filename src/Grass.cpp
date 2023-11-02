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
    controlPoints.resize(m_blades.size() * 4);
    for(auto blade : m_blades)
    {
        for(int i = 0; i < 4; ++i)
        controlPoints.push_back(blade.getControlPoints()[i]);
    }
    return controlPoints;
}

//void Grass::setControlPoints(std::vector<Point> _controlPoints)
//{
//    m_controlPoints = _controlPoints; // -> clang tidy suggests to change it to the following line:
//                                      // m_controlPoints = std::move(_controlPoints);
//}