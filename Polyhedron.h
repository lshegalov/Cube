#pragma once

#include "DirectionalSide.h"
#include "Poligon.h"

class Point3D;
class Facet;
class Poligon;




//three - dimensional shape with flat polygonal faces
class Polyhedron
{
public:
    Polyhedron();
 
    virtual ~Polyhedron();

    virtual bool buildSurface(OUT std::string& sErrorMsg) = 0;

    // build initial polygons from point sequence 
    bool addPolygonFromPoints(IN const std::vector<Point3D>& points, 
        OUT long & lPolygonSequentialNumber,
        OUT std::string & sErrorMsg);

    bool addPolygonFromSides(
        IN const std::vector<long>& sideNumbers,
        OUT long  & lPolygonSequentialNumber, 
        OUT std::string& sErrorMsg);

    bool getSide(IN long lSideSequentialNumber,
                 OUT DirectionalSide** ppSide,
                 OUT std::string& sErrorMsg);


    // add side if it does not exist, otherwise return existing lEdgeSelSideSequentialNumberquentialNumber
    bool addSide(IN const Point3D& startPoint,
                 IN const Point3D& endPoint,
                 OUT long& lSideSequentialNumber,
                 OUT std::string& sErrorMsg);


    size_t getNumberOfPoligons() const;

    const std::map<long, Poligon>& getPolygons() const { return m_polygons; }

    const std::map<long, DirectionalSide>& getSides() const { return m_sides; }
     
    bool splitOnePolygon(OUT std::string& sErrorMsg);


    bool insertPointIntoSide(IN long lSideSequentialNumber,
                             IN const Point3D& midPoint,
                             OUT std::string & sErrorMsg);

    // clean up if that side is not used in any polygons
    bool deleteUnusedSide(long lSideSequentialNumber, OUT std::string& sErrorMsg);

    bool getStatsPerPolygons(
        OUT Results & results,
        OUT std::string& sErrorMsg);

    bool updateStats(IN int nNumberOfSteps, __inout Results& results, OUT std::string& sErrorMsg);
   

    bool printPolygons(const char * szFileName, OUT std::string& sErrorMsg) const;

    bool printEdges(const char* szFileName, OUT std::string& sErrorMsg) const;

protected:

    // update if may be this side has an opposite
    void updateOppositeSide(long lEdgeSequentialNumber);

    // to the end without checking existing edges
    void appendSide(IN const Point3D& startPoint,
                    IN const Point3D& endPoint,
                    OUT long& lEdgeSequentialNumber);

protected:

    std::map<long, DirectionalSide> m_sides;

    std::map<long, Poligon> m_polygons;

    mutable char m_szErrorMsg[ERROR_MSG_LEN];
};

