#include "stdafx.h"
#include "DirectionalSide.h"

DirectionalSide::DirectionalSide() :
    m_lSideSequentialNumber(-1), 
    m_lOppositeSideSequentialNumber(-1)
{

}

DirectionalSide::DirectionalSide(
    __in long lSideSequentialNumber, // parent
    __in const Point3D& startPoint,
    __in const Point3D& endPoint) :
    m_lSideSequentialNumber(lSideSequentialNumber),
    m_startPoint(startPoint),
    m_endPoint(endPoint)
{

}

bool DirectionalSide::insertOneInternalPoint(
    __in const Point3D & newPoint,
    __out std::string& sErrorMsg)
{
    if (newPoint == m_startPoint)
    {
        char szErrorMsg[ERROR_MSG_LEN];
        sprintf_s(szErrorMsg, "%s:%d new point(%f, %f, %f) == m_startPoint(%f, %f, %f) \n",
            __FILE__, __LINE__, 
            newPoint.getX(), newPoint.getY(), newPoint.getZ(),
            m_startPoint.getX(), m_startPoint.getY(), m_startPoint.getZ());
        sErrorMsg = szErrorMsg;
        return false;
    }

    if (newPoint == m_endPoint)
    {
        char szErrorMsg[ERROR_MSG_LEN];
        sprintf_s(szErrorMsg, "%s:%d new point(%f, %f, %f) == m_endPoint(%f, %f, %f) \n",
            __FILE__, __LINE__,
            newPoint.getX(), newPoint.getY(), newPoint.getZ(),
            m_endPoint.getX(), m_endPoint.getY(), m_endPoint.getZ());
        sErrorMsg = szErrorMsg;
        return false;
    }

    m_internalPoints.insert(newPoint);
    return true;
}

void DirectionalSide::insertInternalPoints(const std::set<Point3D>& internalPoints)
{
    std::string sErrorMsg;
    for (std::set<Point3D>::const_iterator pointIter = internalPoints.cbegin();
        pointIter != internalPoints.cend(); pointIter++)
    {
        const Point3D& point = *pointIter;

        insertOneInternalPoint(point, sErrorMsg);
    }
}

// points between (not including borders
void  DirectionalSide::getInternalPoints(
    __in const Point3D& startIntervalPoint,
    __in const Point3D& endIntervalPoint,
    __out std::set<Point3D>& internalPoints) const
{
    internalPoints.clear();
    for (std::set<Point3D>::const_iterator pointIter = m_internalPoints.cbegin();
        pointIter != m_internalPoints.cend(); pointIter++)
    {
        const Point3D& point = *pointIter;
        if (!Point3D::isInside(startIntervalPoint, endIntervalPoint, point))
        {
            continue;
        }

        internalPoints.insert(point);
    }
}