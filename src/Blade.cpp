#include "Blade.h"

Blade::Blade()
{
    m_controlPoints.resize(controlPointsNum);
    m_segmentVector.resize(controlPointsNum - 1); // segments connecting control points
    m_surfaceVector.resize(controlPointsNum - 1);
    m_normalVector.resize(controlPointsNum - 1);
    // Initially surface vector is orthogonal to segment vector and parallel to the ground
    // Normal vector is unit vector orthogonal to segment and surface vectors.
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
        // Calculate and set orientation vectors
        if(i > 0) // One vector per segment, hence one less than number of control points
        {
            m_segmentVector[i - 1] = m_controlPoints[i] - m_controlPoints[i - 1];
            /*
             * Calculate other vectors here too.
             * m_surfaceVector - perpendicular to y-axis, parallel to ground, goes from control point
             * m_normalVector - unit cross product of segment and surface vectors
             */
        }
    }
    return true;
}

std::vector<Point> Blade::getSegmentVectors() const
{
    return m_segmentVector;
}

std::vector<Point> Blade::getSurfaceVectors() const
{
    return m_surfaceVector;
}

std::vector<Point> Blade::getNormalVectors() const
{
    return m_normalVector;
}