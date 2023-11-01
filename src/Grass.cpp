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