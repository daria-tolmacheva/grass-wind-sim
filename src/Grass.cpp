#include "Grass.h"

Grass::Grass(int _bladesNum) : m_bladesNum{_bladesNum}
{
    m_controlPoints.resize(m_bladesNum * 4); // 4 control points per blade
}

Grass::Grass(std::vector<Point> _controlPoints) : m_controlPoints{_controlPoints}
{
    //need to add checks or rework
    m_bladesNum = m_controlPoints.size() / 4; // 4 control points per blade
}

int Grass::bladesNum() const
{
    return m_bladesNum;
}

std::vector<Point> Grass::getControlPoints() const
{
    return m_controlPoints;
}

void Grass::setControlPoints(std::vector<Point> _controlPoints)
{
    m_controlPoints = _controlPoints; // -> clang tidy suggests to change it to the following line:
                                      // m_controlPoints = std::move(_controlPoints);
}