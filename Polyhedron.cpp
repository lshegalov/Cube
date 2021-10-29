#include "stdafx.h"

#include "DirectionalSide.h"
#include "Poligon.h"
#include "SyncFileWriter.h"

#include "Splitter.h"

#include "Polyhedron.h"

Polyhedron::Polyhedron()
{
    m_szErrorMsg[0] = '\0';
}

// virtual
Polyhedron::~Polyhedron()
{
}

bool Polyhedron::addPolygonFromPoints(
    IN const std::vector<Point3D>& points,
    OUT long& lPolygonSequentialNumber,
    OUT std::string& sErrorMsg)
{
    lPolygonSequentialNumber = -1;

    std::vector<long> lSideSequentialNumbers;

    int nPoint = 0;

    Point3D globalStartPoint;

    Point3D prevPoint;
    for (const Point3D& point : points)
    {
        if (nPoint == 0)
        {
            globalStartPoint = point;
            prevPoint = point;
            nPoint++;
            continue;
        }

        long lSideSequentialNumber = -1;

        // add edge if it does not exist, otherwise return existing lEdgeSequentialNumber
        if ( ! addSide(IN prevPoint, // startPoint
                       IN point,     // endPoint,
                       OUT lSideSequentialNumber,
                       OUT sErrorMsg))
        {
            return false;
        }
        
        lSideSequentialNumbers.push_back(lSideSequentialNumber);
        prevPoint = point;
        nPoint++;
    }

    long lLastSideSequentialNumber = -1;
  
    
    // add edge if it does not exist, otherwise return existing lEdgeSequentialNumber
    if ( ! addSide(IN prevPoint, globalStartPoint,
                   OUT lLastSideSequentialNumber,
                   OUT sErrorMsg))
    {
        return false;
    }

    lSideSequentialNumbers.push_back(lLastSideSequentialNumber);

    lPolygonSequentialNumber = 0;
    std::map<long, Poligon>::const_reverse_iterator polyIter = m_polygons.rbegin();
    if (polyIter != m_polygons.crend())
    {
        lPolygonSequentialNumber = polyIter->first;
        lPolygonSequentialNumber++;
    }

    Poligon poligon(this, lPolygonSequentialNumber, lSideSequentialNumbers);
    m_polygons[lPolygonSequentialNumber] = poligon;

    return poligon.checkValidityOfPoligon(OUT sErrorMsg);
}

bool Polyhedron::addPolygonFromSides(
    IN const std::vector<long>& sideNumbers,
    OUT long& lPolygonSequentialNumber,
    OUT std::string& sErrorMsg)
{
    lPolygonSequentialNumber = (long) m_polygons.size();

    Poligon poligon(this, lPolygonSequentialNumber, sideNumbers);
    m_polygons[lPolygonSequentialNumber] = poligon;

    return poligon.checkValidityOfPoligon(OUT sErrorMsg);
}

// add side if it does not exist, otherwise return existing lEdgeSequentialNumber
bool Polyhedron::addSide(
    IN const Point3D& startPoint,
    IN const Point3D& endPoint,
    OUT long & lSideSequentialNumber,
    OUT std::string& sErrorMsg)
{
    if (startPoint == endPoint)
    {
        sprintf_s(m_szErrorMsg, "%s:%d startPoint ==endPoint \n",
            __FILE__, __LINE__);
        sErrorMsg = m_szErrorMsg;
        return false;
    }

    for (std::map<long, DirectionalSide>::const_iterator sideIter = m_sides.cbegin();
        sideIter != m_sides.cend(); sideIter++)
    {
        long lSequentialNumber = sideIter->first;
        const DirectionalSide & side = sideIter->second;

        const Point3D& startCurrent = side.getStartPoint();
        const Point3D& endCurrent = side.getEndPoint();
        
        if ((startPoint != startCurrent) ||
            (endPoint != endCurrent))
        {
            continue;
        }

        lSideSequentialNumber = lSequentialNumber;
        return true;
    }

    // new edge

    // append to the end without checking existing edges
    appendSide(IN startPoint,
               IN endPoint,
               OUT lSideSequentialNumber);

    return true;
}

// to the end without checking existing edges
void Polyhedron::appendSide(
    IN const Point3D& startPoint,
    IN const Point3D& endPoint,
    OUT long& lSideSequentialNumber)
{
    // new side

    std::map<long, DirectionalSide>::const_reverse_iterator sideIter = m_sides.rbegin();
    if (sideIter == m_sides.crend())
    {
        lSideSequentialNumber = 0;
    }
    else
    {
        lSideSequentialNumber = sideIter->first;
        lSideSequentialNumber++;
    }

#ifdef _DEBUG
    if (TEST_SIDE == lSideSequentialNumber)
    {
        int k = 0;
    }
#endif

    DirectionalSide newSide(lSideSequentialNumber, startPoint, endPoint);

    m_sides[lSideSequentialNumber] = newSide;

    // update if may be this edge has an opposite
    updateOppositeSide(lSideSequentialNumber);
}

// update if may be this edge has an opposite
void Polyhedron::updateOppositeSide(long lForwardSideSequentialNumber)
{
    DirectionalSide & forwardSide = m_sides[lForwardSideSequentialNumber];

    // check opposite direction
    const Point3D & endPoint = forwardSide.getStartPoint();
    const Point3D & startPoint = forwardSide.getEndPoint();


    for (std::map<long, DirectionalSide>::iterator sideIter = m_sides.begin();
        sideIter != m_sides.end(); sideIter++)
    {
        long lSequentialNumber = sideIter->first;
        if (lSequentialNumber == lForwardSideSequentialNumber)
        {
            // can not be opposite to itself
            continue;
        }
        DirectionalSide & currentSide = sideIter->second;

        const Point3D & startCurrent = currentSide.getStartPoint();
        const Point3D & endCurrent = currentSide.getEndPoint();

        if ((startPoint != startCurrent) ||
            (endPoint != endCurrent))
        {
            continue;
        }
        
        forwardSide.setOppositeSideNumber(lSequentialNumber);

        currentSide.setOppositeSideNumber(lForwardSideSequentialNumber);
        break;
    }
}

bool Polyhedron::getSide(
    IN long lSequentialNumber,
    OUT DirectionalSide** ppSide,
    OUT std::string& sErrorMsg) 
{
    std::map<long, DirectionalSide>::iterator sideIter = m_sides.find(lSequentialNumber);
    if (sideIter == m_sides.cend())
    {
        sprintf_s(m_szErrorMsg, "%s:%d nEdgeIndex=%ld is not found size=%zd\n",
            __FILE__, __LINE__, lSequentialNumber, m_sides.size());
        sErrorMsg = m_szErrorMsg;

        *ppSide = nullptr;
        return false;
    }
    *ppSide = & sideIter->second;

    return true;
}

// main split function
bool Polyhedron::splitOnePolygon(OUT std::string& sErrorMsg)
{
    sErrorMsg[0] = '\0';

    size_t nNumberOfPolygons = m_polygons.size();

    int nMaxAttempt = 20;

    int nIndex = -1;

    bool bIsFound = false;

    int nAttempt = 0;

    while (nAttempt < nMaxAttempt)
    {
        nAttempt++;

        // rand() generate random numbers in the range [0, RAND_MAX). 
        nIndex = (int)(nNumberOfPolygons * (1.0 * rand()) / RAND_MAX);

        if (nIndex < 0 || nIndex >= (int) nNumberOfPolygons)
        {

            continue;
        }

        bIsFound = true;
        break;
    }

    if ( ! bIsFound)
    {
        // GREP
        sprintf_s(m_szErrorMsg, "%s:%d could not find new edge= >= %zd \n",
            __FILE__, __LINE__,  nNumberOfPolygons);
        sErrorMsg = m_szErrorMsg;
        return false;
    }

    Poligon& polygon = m_polygons[nIndex];

    Splitter splitter(this);

    long lNewPolygonSequentialNumber = -1;
    if ( ! splitter.splitPoligon(__inout polygon, OUT lNewPolygonSequentialNumber, OUT sErrorMsg))
    {
        return false;
    }

    return true;
}

size_t Polyhedron::getNumberOfPoligons() const
{ 
    return m_polygons.size(); 
}

// clean up if that side is not used in any polygons
bool Polyhedron::deleteUnusedSide(
    IN long lSideSequentialNumber, 
    OUT std::string& sErrorMsg)
{
    std::map<long, DirectionalSide>::iterator sideIter = m_sides.find(lSideSequentialNumber);
    if (sideIter == m_sides.end())
    {
        sprintf_s(m_szErrorMsg, "%s:%d nSide Index=%ld is not found \n",
            __FILE__, __LINE__, lSideSequentialNumber);
        sErrorMsg = m_szErrorMsg;
        return false;
    }
    DirectionalSide & side = sideIter->second;
    long lOppositeSide = side.getOppositeSideSequentialNumber();

    // check what that edge is not referenced in all polygons
    //bool bIsFound = false;

    for (std::map<long, Poligon>::const_iterator polygonIter = m_polygons.cbegin();
        polygonIter != m_polygons.cend(); polygonIter++)
    {
        long lPolygonSeqNumber = polygonIter->first;

        const Poligon& otherPolygon = polygonIter->second;
       
        const std::vector<long> & sideNumbers = otherPolygon.getSideNumbers();

        for (const long& lCurrentSequentialNumber : sideNumbers)
        {
            if (lCurrentSequentialNumber == lSideSequentialNumber)
            {
                sprintf_s(m_szErrorMsg, "%s:%d nEdgeIndex=%ld is referenced in polygon=%ld was found \n",
                    __FILE__, __LINE__, lSideSequentialNumber, lPolygonSeqNumber);
                sErrorMsg = m_szErrorMsg;
                return true;
            }
        }
    }

    m_sides.erase(lSideSequentialNumber);

    if (lOppositeSide < 0)
        return true;

    sideIter = m_sides.find(lOppositeSide);
    if (sideIter == m_sides.end())
    {
        sprintf_s(m_szErrorMsg, "%s:%d nSide Index=%ld is not found \n",
            __FILE__, __LINE__, lSideSequentialNumber);
        sErrorMsg = m_szErrorMsg;
        return false;
    }

    DirectionalSide & oppositeSide = sideIter->second;
    oppositeSide.setOppositeSideNumber(-1);
    return true;
}

bool Polyhedron::getStatsPerPolygons(
    OUT Results & results,
    OUT std::string& sErrorMsg)
{
    results.lNumberOfPolygons = (long) m_polygons.size();

    results.distributionPerEdges.clear();
    results.distributionPerSides.clear();

    const int nNumberOfPolygons = (int) m_polygons.size();

    results.nTotalNumberOfEdges = 0;
    results.nTotalNumberOfSides = 0;

    for (std::map<long, Poligon>::const_iterator polygonIter = m_polygons.cbegin();
        polygonIter != m_polygons.cend(); polygonIter++)
    {
        const Poligon & polygon = polygonIter->second;

        size_t nNumberOfPoints = 0;
        if ( ! polygon.getNumberOfPoints(OUT & nNumberOfPoints, OUT sErrorMsg))
        {
            return false;
        }

        results.distributionPerEdges[(int) nNumberOfPoints] += 1;

        results.nTotalNumberOfEdges += (int) nNumberOfPoints;

        int nNumberOfSides = (int) polygon.getNumberOfSides();

        results.nTotalNumberOfSides += nNumberOfSides;
        
        results.distributionPerSides[(int) nNumberOfSides] += 1;
    }

    results.dAverageNumberOfSidesPerPolygons = (1.0 * results.nTotalNumberOfSides) / nNumberOfPolygons;

    results.dAverageNumberOfEdgesPerPolygons = (1.0 * results.nTotalNumberOfEdges) / nNumberOfPolygons;

    return true;

}


bool Polyhedron::updateStats(IN int nNumberOfSteps, __inout Results& results, OUT std::string& sErrorMsg)
{
    int nTotalNumberOfEdges = 0;
    int nTotalNumberOfSides = 0;

    const size_t nNumberOfPolygons = m_polygons.size();

    for (std::map<long, Poligon>::const_iterator polygonIter = m_polygons.cbegin();
        polygonIter != m_polygons.cend(); polygonIter++)
    {
        const Poligon& polygon = polygonIter->second;

        size_t nNumberOfPoints = 0;
        if (! polygon.getNumberOfPoints(OUT & nNumberOfPoints, OUT sErrorMsg))
        {
            return false;
        }

        nTotalNumberOfEdges += (int) nNumberOfPoints;

        int nNumberOfSides = (int) polygon.getNumberOfSides();

        nTotalNumberOfSides += nNumberOfSides;
    }


    results.dMeanNumberOfSidesPerPolygons[nNumberOfSteps] = (1.0 * nTotalNumberOfSides) / nNumberOfPolygons;

    results.dMeanNumberOfEdgesPerPolygons[nNumberOfSteps] = (1.0 * nTotalNumberOfEdges) / nNumberOfPolygons;

    return true;

}

bool Polyhedron::printEdges(
    const char* szFileName,
    OUT std::string& sErrorMsg)  const
{
    SyncFileWriter fileWriter;

    const char* szOutputDirectory = "D:\\AlexLevin\\src\\ConsoleApplication1";

    if ( ! fileWriter.createFile(IN szOutputDirectory,
        IN szFileName, // szOutputFileName, // with or without .csv or other extention
        IN ".csv",     // szExtention,      // ".csv", ".ok" ...
        OUT sErrorMsg))
    {
        return false;
    }
    

    for (std::map<long, DirectionalSide>::const_iterator sideIter = m_sides.cbegin();
        sideIter != m_sides.cend(); sideIter++)
    {
        long lSequentialNumber = sideIter->first;
        const DirectionalSide & side = sideIter->second;

        fileWriter.write("Edge, %d,", lSequentialNumber);

        const Point3D& startPoint = side.getStartPoint();

        fileWriter.write("Start, x=,%.6f,y=,%.6f,z=, %.6f,", startPoint.getX(), startPoint.getY(), startPoint.getZ());

        const Point3D& endPoint = side.getEndPoint();

        fileWriter.write("End, x=,%.6f,y=,%.6f,z=, %.6f,\n", endPoint.getX(), endPoint.getY(), endPoint.getZ());
        
    }

    return true;
}

bool Polyhedron::printPolygons(
    const char* szFileName,
    OUT std::string& sErrorMsg)  const
{
    SyncFileWriter fileWriter;

    const char* szOutputDirectory = "D:\\AlexLevin\\src\\ConsoleApplication1";

    if (!fileWriter.createFile(IN szOutputDirectory,
        IN szFileName, // szOutputFileName, // with or without .csv or other extention
        IN ".csv",     // szExtention,      // ".csv", ".ok" ...
        OUT sErrorMsg))
    {
        return false;
    }

    for (std::map<long, Poligon>::const_iterator polygonIter = m_polygons.cbegin();
        polygonIter != m_polygons.cend(); polygonIter++)
    {
        long lPolygonSeqNumber = polygonIter->first;
        const Poligon& otherPolygon = polygonIter->second;

        fileWriter.write("Poligon, %d, Edge ##:,", lPolygonSeqNumber);

        const std::vector<long>& sideNumbers = otherPolygon.getSideNumbers();

        for (const long& lSideSequentialNumber : sideNumbers)
        {
            fileWriter.write("%.ld,", lSideSequentialNumber);
        }
        fileWriter.write("\n");
    }

    return true;
}

bool Polyhedron::insertPointIntoSide(
    IN long lSideSequentialNumber,
    IN const Point3D& midPoint,
    OUT  std::string& sErrorMsg)
{
    DirectionalSide* pSide = nullptr;
    if ( ! getSide(IN lSideSequentialNumber,
                   OUT & pSide,
                   OUT sErrorMsg))
    {
        return false;
    }

    return pSide->insertOneInternalPoint(IN midPoint, OUT sErrorMsg);
}