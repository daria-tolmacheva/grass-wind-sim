#include <ngl/BezierCurve.h>
#include <ngl/ShaderLib.h>
#include <ngl/Quaternion.h>

#include "Blade.h"

Blade::Blade()
{
    m_windCoof = 500.0f;
    m_dencity = 0.5f;
    m_dampingCoof = 0.5f;
    m_controlPoints.resize(controlPointsNum);
    m_positionOrig.resize(controlPointsNum);
    m_segmentVector.resize(controlPointsNum - 1); // segments connecting control points
    m_segmentOrig.resize(controlPointsNum - 1);
    m_surfaceVector.resize(controlPointsNum - 1);
    m_normalVector.resize(controlPointsNum - 1);
    // Initially surface vector is orthogonal to segment vector and parallel to the ground
    // Normal vector is unit vector orthogonal to segment and surface vectors.
    m_totalForce.resize(controlPointsNum - 1);
    m_angularVelocity.resize(controlPointsNum - 1);
    std::fill_n(m_angularVelocity.begin(), m_angularVelocity.size(), ngl::Vec3(0.0f, 0.0f, 0.0f));
    m_mass.resize(controlPointsNum - 1);
}

ngl::Vec3 Blade::projectToGround(ngl::Vec3 _vec)
{
    ngl::Vec3 v1 = {1.0f, 0.0f, 0.0f};
    ngl::Vec3 v2 = {0.0f, 0.0f, 1.0f};

    return _vec.dot(v1)/_vec.dot(_vec) * v1 + _vec.dot(v2)/_vec.dot(_vec) * v2;
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

            // set mass
            m_mass[i - 1] = m_dencity * m_segmentVector[i - 1].length() * segmentWidthCoefficient[i - 1];
        }
    }
    // Set up original position
    m_segmentOrig = m_segmentVector;
    m_positionOrig = m_controlPoints;

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

// Should probably refactor for readability
void Blade::update(Simulation sim, float _dt)
{
    std::fill_n(m_totalForce.begin(), m_totalForce.size(), ngl::Vec3(0.0f, 0.0f, 0.0f));

    // Swinging
    // Wind velocity at the tip
    ngl::Vec3 vWind;
    vWind = ngl::Vec3(sim.sampleField(m_controlPoints[controlPointsNum].m_x, m_controlPoints[controlPointsNum].m_y, m_controlPoints[controlPointsNum].m_z, X_FIELD),
                      sim.sampleField(m_controlPoints[controlPointsNum].m_x, m_controlPoints[controlPointsNum].m_y, m_controlPoints[controlPointsNum].m_z, Y_FIELD),
                      sim.sampleField(m_controlPoints[controlPointsNum].m_x, m_controlPoints[controlPointsNum].m_y, m_controlPoints[controlPointsNum].m_z, Z_FIELD));

    // Growth vectors - static and current
    ngl::Vec3 growthVecStatic = m_positionOrig[3] - m_positionOrig[0];
    growthVecStatic = projectToGround(growthVecStatic);
    ngl::Vec3 growthVecCurrent = m_controlPoints[3] - m_controlPoints[0];
    growthVecCurrent = projectToGround(growthVecCurrent);
    ngl::Vec3 growthDiff = growthVecStatic - growthVecCurrent;

    // For each segment find the total force on it
    for (int i = 0; i < controlPointsNum-1; ++i)
    {
        if (vWind.length() != 0.0f)
        {
            // Swinging
            //   - Wind force - same as at the tip of the grass blade
            m_totalForce[i] = m_totalForce[i] + m_windCoof * vWind.dot(m_surfaceVector[i]) * m_surfaceVector[i];

            //   - Restoration force
            if (growthDiff.length() != 0.0) {
                m_totalForce[i] = m_totalForce[i] + (m_stiffness[3] / m_stiffness[i]) * m_angularVelocity[i] * _dt * (growthDiff / growthDiff.length());
            }

            // Damping force - NOT JUST A COMPONENT
            m_totalForce[i] = m_totalForce[i] + m_dampingCoof * m_angularVelocity[i];
        }

    }

//    // Bending calculated separately
//
//    std::vector<int> upEdges;
//    std::vector<int> downEdges;
//    for (int i = 0; i < controlPointsNum-1; ++i)
//    {
//        if (m_segmentVector[i].m_y > 1)
//        {
//            upEdges.push_back(i);
//        }
//        else
//        {
//            downEdges.push_back(i);
//        }
//    }
//
//    if( downEdges.empty() || upEdges.empty() )
//    {
//        // Bend vectors - static and current
//        ngl::Vec3 bendVecStatic = m_segmentOrig[4] - m_segmentOrig[0];
//        ngl::Vec3 bendVecCurrent = m_segmentVector[4] - m_segmentVector[0];
//        ngl::Vec3 bendVecDiff = bendVecStatic - bendVecCurrent;
//
//        // For each segment find the total force on it
//        for (int i = 0; i < controlPointsNum-1; ++i)
//        {
//            //   - Wind force - same as at the tip of the grass blade
//            m_totalForce[i] +=
//                    m_windCoof * vWind.dot(m_normalVector[i]) * m_normalVector[i];
//
//            //   - Restoration force
//            m_totalForce[i] += m_stiffness[3]/m_stiffness[i] * m_angularVelocity[i] * _dt * (bendVecDiff/bendVecDiff.length());
//        }
//    }
//    else
//    {
//        // Process down edges
//        // Wind velocity at local tip (up edges)
//        int localTipIndexDown = downEdges[downEdges.size() - 1];
//        ngl::Vec3 vWindDown = ngl::Vec3(
//                sim.sampleField(m_controlPoints[localTipIndexDown].m_x, m_controlPoints[localTipIndexDown].m_y,
//                                m_controlPoints[localTipIndexDown].m_z, X_FIELD),
//                sim.sampleField(m_controlPoints[localTipIndexDown].m_x, m_controlPoints[localTipIndexDown].m_y,
//                                m_controlPoints[localTipIndexDown].m_z, Y_FIELD),
//                sim.sampleField(m_controlPoints[localTipIndexDown].m_x, m_controlPoints[localTipIndexDown].m_y,
//                                m_controlPoints[localTipIndexDown].m_z, Z_FIELD));
//
//        // Bend vectors - static and current
//        ngl::Vec3 bendVecStaticDown = m_segmentOrig[localTipIndexDown] - m_segmentOrig[0];
//        ngl::Vec3 bendVecCurrentDown = m_segmentVector[localTipIndexDown] - m_segmentVector[0];
//        ngl::Vec3 bendVecDiffDown = bendVecStaticDown - bendVecCurrentDown;
//
//        for (int i = 0; i < downEdges.size() - 1; ++i) {
//            int currentIndex = downEdges[i];
//            //   - Wind force - same as at the tip of the grass blade
//            m_totalForce[currentIndex] +=
//                    m_windCoof * vWindDown.dot(m_normalVector[currentIndex]) * m_normalVector[currentIndex];
//
//            //   - Restoration force
//            m_totalForce[currentIndex] +=
//                    m_stiffness[localTipIndexDown] / m_stiffness[currentIndex] * m_angularVelocity[currentIndex] * _dt *
//                    (bendVecDiffDown / bendVecDiffDown.length());
//        }
//
//        // Process up edges
//        // Wind velocity at local tip (up edges)
//        int localTipIndex = upEdges[upEdges.size() - 1];
//        ngl::Vec3 vWindUp = ngl::Vec3(
//                sim.sampleField(m_controlPoints[localTipIndex].m_x, m_controlPoints[localTipIndex].m_y,
//                                m_controlPoints[localTipIndex].m_z, X_FIELD),
//                sim.sampleField(m_controlPoints[localTipIndex].m_x, m_controlPoints[localTipIndex].m_y,
//                                m_controlPoints[localTipIndex].m_z, Y_FIELD),
//                sim.sampleField(m_controlPoints[localTipIndex].m_x, m_controlPoints[localTipIndex].m_y,
//                                m_controlPoints[localTipIndex].m_z, Z_FIELD));
//
//        // Bend vectors - static and current
//        ngl::Vec3 bendVecStatic = m_segmentOrig[localTipIndex] - m_segmentOrig[0];
//        ngl::Vec3 bendVecCurrent = m_segmentVector[localTipIndex] - m_segmentVector[0];
//        ngl::Vec3 bendVecDiff = bendVecStatic - bendVecCurrent;
//
//        for (int i = 0; i < upEdges.size() - 1; ++i) {
//            int currentIndex = upEdges[i];
//            //   - Wind force - same as at the tip of the grass blade
//            m_totalForce[currentIndex] +=
//                    m_windCoof * vWindUp.dot(m_normalVector[currentIndex]) * m_normalVector[currentIndex];
//
//            //   - Restoration force
//            m_totalForce[currentIndex] +=
//                    m_stiffness[localTipIndex] / m_stiffness[currentIndex] * m_angularVelocity[currentIndex] * _dt *
//                    (bendVecDiff / bendVecDiff.length());
//        }
//    }

    // Calculate angular velocities at control points for restorative/damping forces
    for (int i = 0; i < controlPointsNum-1; ++i)
    {
        m_angularVelocity[i] += m_segmentVector[i].cross(m_totalForce[i])/(m_mass[i] * m_segmentVector[i].length() * m_segmentVector[i].length()) * _dt;
    }

    // Calculate new control points
    updateControlPoints(_dt);
}

void Blade::updateControlPoints(float _dt)
{
    for (int i = 0; i < m_angularVelocity.size(); ++i) {
        float angle = m_angularVelocity[i].length() * _dt;

        // create quaternion for rotation
        ngl::Quaternion rotQ = ngl::Quaternion(cos(angle/2), 0, sin(angle/2), 0);

        // apply rotation to points and vectors
        ngl::Quaternion::rotatePoint(rotQ, m_controlPoints[i+1]);
        ngl::Quaternion::rotatePoint(rotQ, m_segmentVector[i]);
        ngl::Quaternion::rotatePoint(rotQ, m_surfaceVector[i]);
        ngl::Quaternion::rotatePoint(rotQ, m_normalVector[i]);

    }
}