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

std::vector<ngl::Vec3> Blade::getControlPoints() const
{
    return m_controlPoints;
}

bool Blade::setControlPoint(int _index, ngl::Vec3 _point)
{
    if(_index < controlPointsNum)
    {
        m_controlPoints[_index] = _point;
        return true;
    }
    else
        return false;
}

bool Blade::setControlPoints(std::vector<ngl::Vec3> _points)
{
    if(_points.size() != controlPointsNum)
        return false;
    ngl::Vec3 bladeDirection = _points[controlPointsNum - 1] - _points[0];
    for(int i = 0; i < controlPointsNum; ++i)
    {
        setControlPoint(i, _points[i]);
        // Calculate and set orientation vectors
        if(i > 0) // One vector per segment, hence one less than number of control points
        {
            // m_segmentVector - vector connecting adjacent control points
            m_segmentVector[i - 1] = m_controlPoints[i] - m_controlPoints[i - 1];

            // m_surfaceVector - perpendicular to y-axis, parallel to ground, goes from control point
            m_surfaceVector[i - 1] = bladeDirection.cross(ngl::Vec3(0.0f, 1.0f, 0.0f));
            m_surfaceVector[i - 1].normalize();
            m_surfaceVector[i - 1] *= (baseSegmentWidth * segmentWidthCoefficient[i - 1]);

            // m_normalVector - unit cross product of segment and surface vectors
            m_normalVector[i - 1] = m_segmentVector[i - 1].cross(m_surfaceVector[i - 1]);
            m_normalVector[i - 1].normalize();
        }
    }
    return true;
}

std::vector<ngl::Vec3> Blade::getSegmentVectors() const
{
    return m_segmentVector;
}

std::vector<ngl::Vec3> Blade::getSurfaceVectors() const
{
    return m_surfaceVector;
}

std::vector<ngl::Vec3> Blade::getNormalVectors() const
{
    return m_normalVector;
}