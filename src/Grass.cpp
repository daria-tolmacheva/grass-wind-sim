#include "Grass.h"

Grass::Grass(int _bladesNum)
{
    m_bladesNum = _bladesNum;
    m_controlPoints = std::make_unique<Point []>(m_bladesNum*4); // 4 control points per blade
}

int Grass::bladesNum() const
{
    return m_bladesNum;
}

Point *Grass::getControlPoints() const
{
    return m_controlPoints.get();
}

void Grass::setControlPoints(Point* _controlPoints)
{
    for(int i = 1; i < m_bladesNum * 4; ++i) // 4 control points per blade
    {
        m_controlPoints[i] = _controlPoints[i];
    }
}