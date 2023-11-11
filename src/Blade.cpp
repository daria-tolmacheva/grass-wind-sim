#include "Blade.h"

Blade::Blade()
{
    m_controlPoints.resize(controlPointsNum);
}

std::vector<Point> Blade::getControlPoints() const
{
    return m_controlPoints;
}

bool Blade::setControlPoint(int _index, Point _point)
{
    if(_index < controlPointsNum)
    {
        m_controlPoints[_index] = _point;
        return true;
    }
    else
        return false;
}

bool Blade::setControlPoints(std::vector<Point> _points)
{
    if(_points.size() != controlPointsNum)
        return false;
    for(int i = 0; i < controlPointsNum; ++i)
    {
        setControlPoint(i, _points[i]);
    }
    return true;
}