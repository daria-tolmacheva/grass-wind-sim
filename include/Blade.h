#ifndef BLADE_H
#define BLADE_H

#include <vector>
#include <ngl/Vec3.h>

class Blade
{
public:
    Blade();
    static constexpr int controlPointsNum = 4;

    // I'll probably have a different mechanism of defining grass blade width later.
    // Potentially set those values together with control points.
    static constexpr float baseSegmentWidth = 0.1f;
    const static inline std::vector<float> segmentWidthCoefficient = {1.0f, 0.9f, 0.5f};

    [[nodiscard]] std::vector<ngl::Vec3> getControlPoints() const;
    bool setControlPoint(int _index, ngl::Vec3 _point);
    bool setControlPoints(std::vector<ngl::Vec3> _points);
    [[nodiscard]] std::vector<ngl::Vec3> getSegmentVectors() const;
    [[nodiscard]] std::vector<ngl::Vec3> getSurfaceVectors() const;
    [[nodiscard]] std::vector<ngl::Vec3> getNormalVectors() const;
    void draw(ngl::Mat4 MVP) const;

private:
    std::vector<ngl::Vec3> m_controlPoints;
    // For each of 3 blade segments we need to store 3 types of vectors to describe its orientation
    std::vector<ngl::Vec3> m_segmentVector; // Vector connecting 2 adjacent control points
    std::vector<ngl::Vec3> m_surfaceVector; // Orthogonal to segment vector and on the blade plane
    std::vector<ngl::Vec3> m_normalVector; // Unit vector orthogonal to both segment and surface vectors
};

#endif
