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

    virtual bool buildSurface(__out std::string& sErrorMsg) = 0;

    // build initial polygons from point sequence 
    bool addPolygonFromPoints(__in const std::vector<Point3D>& points, 
        __out long & lPolygonSequentialNumber,
        __out std::string & sErrorMsg);

    bool addPolygonFromSides(
        __in const std::vector<long>& sideNumbers,
        __out long  & lPolygonSequentialNumber, 
        __out std::string& sErrorMsg);

    bool getSide(__in long lSideSequentialNumber,
                 __out DirectionalSide** ppSide,
                 __out std::string& sErrorMsg);


    // add side if it does not exist, otherwise return existing lEdgeSelSideSequentialNumberquentialNumber
    bool addSide(__in const Point3D& startPoint,
                 __in const Point3D& endPoint,
                 __out long& lSideSequentialNumber,
                 __out std::string& sErrorMsg);


    size_t getNumberOfPoligons() const;

    const std::map<long, Poligon>& getPolygons() const { return m_polygons; }

    const std::map<long, DirectionalSide>& getSides() const { return m_sides; }
     
    bool splitOnePolygon(__out std::string& sErrorMsg);


    bool insertPointIntoSide(__in long lSideSequentialNumber,
                             __in const Point3D& midPoint,
                             __out std::string & sErrorMsg);

    // clean up if that side is not used in any polygons
    bool deleteUnusedSide(long lSideSequentialNumber, __out std::string& sErrorMsg);

    bool getStatsPerPolygons(
        __out Results & results,
        __out std::string& sErrorMsg);

    bool updateStats(__in int nNumberOfSteps, __inout Results& results, __out std::string& sErrorMsg);
   

    bool printPolygons(const char * szFileName, __out std::string& sErrorMsg) const;

    bool printEdges(const char* szFileName, __out std::string& sErrorMsg) const;

protected:

    // update if may be this side has an opposite
    void updateOppositeSide(long lEdgeSequentialNumber);

    // to the end without checking existing edges
    void appendSide(__in const Point3D& startPoint,
                    __in const Point3D& endPoint,
                    __out long& lEdgeSequentialNumber);

protected:

    std::map<long, DirectionalSide> m_sides;

    std::map<long, Poligon> m_polygons;

    mutable char m_szErrorMsg[ERROR_MSG_LEN];
};

