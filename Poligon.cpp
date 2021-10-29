#include "stdafx.h"

#include "DirectionalSide.h"


#include "Polyhedron.h"

#include "Poligon.h"

Poligon::Poligon()
{
    m_pParent = nullptr;

    m_lSequentialNumber = -1;
}


Poligon::Poligon(Polyhedron* pParent, 
    IN long lSequentialNumber,
    IN const std::vector<long>& sideNumbers) :
    m_pParent(pParent),
    m_lSequentialNumber(lSequentialNumber),
    m_sideNumbers(sideNumbers)
{
}


size_t Poligon::getNumberOfSides() const 
{ 
    return m_sideNumbers.size();
}


bool Poligon::getNumberOfPoints(OUT size_t* pnNumberOfPoints, OUT std::string& sErrorMsg) const
{
    size_t nTotalNumberOfPoints = m_sideNumbers.size();

    for (long lSideNumber : m_sideNumbers)
    {
        DirectionalSide* pSide = nullptr;

        if ( ! m_pParent->getSide(IN lSideNumber,
                                  OUT & pSide,
                                  OUT sErrorMsg))
        {
            return false;
        }
        nTotalNumberOfPoints += pSide->getNumberOfInternalPoints();
    }

    *pnNumberOfPoints = nTotalNumberOfPoints;

    return true;
}

bool Poligon::updateSides(
    IN const std::vector<long>& sideNumbers,
    OUT std::string& sErrorMsg)
{
    m_sideNumbers = sideNumbers;

    return checkValidityOfPoligon(OUT sErrorMsg);
}

// must be closed with consecutive edges
bool Poligon::checkValidityOfPoligon(OUT std::string& sErrorMsg) const
{
    char szErrorMsg[ERROR_MSG_LEN];
    sErrorMsg[0] = '\0';

    if (m_sideNumbers.empty())
    {
        sprintf_s(szErrorMsg, "%s:%d empty sides \n",
                  __FILE__, __LINE__);
        sErrorMsg = szErrorMsg;
        return false;
    }

    int nSide = 0;

    Point3D globalStartPoint;
    Point3D prevEndPoint;

    std::set<long> lTotalSideNumbers;

    for (long lSideNumber : m_sideNumbers)
    {
        DirectionalSide* pSide = nullptr;

        lTotalSideNumbers.insert(lSideNumber);

        if ( ! m_pParent->getSide(IN lSideNumber,
                                  OUT & pSide,
                                  OUT sErrorMsg))
        {
            return false;
        }

        assert(pSide != nullptr);

        if(pSide->getSideSequentialNumber() != lSideNumber)
        {
            sprintf_s(szErrorMsg, "%s:%d getSequentialNumber=%ld <> %ld \n",
                __FILE__, __LINE__, pSide->getSideSequentialNumber(), lSideNumber);
            sErrorMsg = szErrorMsg;
            return false;
        }

        const Point3D & startPoint = pSide->getStartPoint();
        const Point3D& endPoint = pSide->getEndPoint();

        if (startPoint == endPoint)
        {     
            sprintf_s(szErrorMsg, "%s:%d lSideSequentialNumber=%ld, nSide=%d, startPoint=(%f,%f,%f) == endPoint=(%f,%f,%f) \n",
                      __FILE__, __LINE__, lSideNumber, nSide,
                      startPoint.getX(), startPoint.getY(), startPoint.getZ(), 
                      endPoint.getX(), endPoint.getY(), endPoint.getZ());
            sErrorMsg = szErrorMsg;
            return false;
        }


        if (nSide == 0)
        {
            globalStartPoint = startPoint;
            prevEndPoint = endPoint;
            nSide++;
            continue;
        }


        if (prevEndPoint != startPoint)
        {
            sprintf_s(szErrorMsg, "%s:%d lSideSequentialNumber=%ld, nEdge=%d,  prevEndPoint=(%f,%f,%f) != startPoint=(%f,%f,%f) \n",
                __FILE__, __LINE__, lSideNumber, nSide,
                prevEndPoint.getX(), prevEndPoint.getY(), prevEndPoint.getZ(),
                startPoint.getX(), startPoint.getY(), startPoint.getZ());
            sErrorMsg = szErrorMsg;
            return false;
        }

        prevEndPoint = endPoint;
        nSide++;
    }

    if (prevEndPoint != globalStartPoint)
    {
        sprintf_s(szErrorMsg, "%s:%d nSide=%d, prevEndPoint=(%f,%f,%f) != startPoint=(%f,%f,%f)\n",
                  __FILE__, __LINE__, nSide,
                  prevEndPoint.getX(), prevEndPoint.getY(), prevEndPoint.getZ(),
                  globalStartPoint.getX(), globalStartPoint.getY(), globalStartPoint.getZ());
        sErrorMsg = szErrorMsg;
        return false;
    }

    if (lTotalSideNumbers.size() != m_sideNumbers.size())
    {
        sprintf_s(szErrorMsg, "%s:%d lTotalSideNumbers.size()=%zd != m_sideNumbers.size()=%zd\n",
            __FILE__, __LINE__, lTotalSideNumbers.size(), m_sideNumbers.size());
        sErrorMsg = szErrorMsg;
        return false;

    }
    return true;
}

