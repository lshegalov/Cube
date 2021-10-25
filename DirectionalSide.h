#pragma once

#include "Point3D.h"

class DirectionalSide
{
public:
    DirectionalSide();
    DirectionalSide(long lSequentialNumber, const Point3D& startPoint, const Point3D& endPoint);

    long getSideSequentialNumber() const { return m_lSideSequentialNumber; };

    const Point3D & getStartPoint() const { return m_startPoint; }
    const Point3D & getEndPoint() const { return m_endPoint; }

    bool insertOneInternalPoint(const Point3D & newPoint, __out std::string& sErrorMsg);

    void insertInternalPoints(const std::set<Point3D>& internalPoints);

    size_t getNumberOfInternalPoints() const { return m_internalPoints.size(); }

    // points between (not including borders
    void getInternalPoints(const Point3D & startIntervalPoint, const Point3D & endIntervalPoint,
                           std::set<Point3D> & internalPoints) const;

    void setOppositeSideNumber(long lOppositeSequentialNumber)
    {
        m_lOppositeSideSequentialNumber = lOppositeSequentialNumber;
    }
    long getOppositeSideSequentialNumber() const { return m_lOppositeSideSequentialNumber; };

private:
    long m_lSideSequentialNumber = -1;

    long m_lOppositeSideSequentialNumber = -1;

    //std::vector<long> m_lEdgeSequentialNumbers;

    std::set<Point3D> m_internalPoints;

    Point3D m_startPoint;

    Point3D m_endPoint;
};

