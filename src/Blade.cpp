#include "Blade.h"

Blade::Blade()
{
    m_controlPoints.resize(4);
}

std::vector<Point> Blade::getControlPoints() const
{
    return m_controlPoints;
}

bool Blade::setControlPoint(int _index, Point _point)
{
    if(_index < 4)
    {
        m_controlPoints[_index] = _point;
        return true;
    }
    else
        return false; // only 4 control points exist
}

bool Blade::setControlPoints(std::vector<Point> _points)
{
    if(_points.size() != 4)
        return false;
    for(int i = 0; i < 4; ++i)
    {
        setControlPoint(i, _points[i]);
    }
    return true;
}