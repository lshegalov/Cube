#include "stdafx.h"


#include "DirectionalSide.h"
#include "Point3D.h"
#include "Poligon.h"

#include "Cube.h"

Cube::Cube()
{


}

// virtual 
Cube::~Cube()
{

}

// virtual
bool Cube::buildSurface(OUT std::string& sErrorMsg)
{
    
    // bottom and top faces
    for (int nI = 0; nI < 2; nI++)
    {
        double z;

        if (nI == 0)
        {
            z = 0; 
            //sFaceName = "Bottom";
        }
        else
        {
            z = 1; 
            //sFaceName = "Top";
        }
        std::vector<Point3D> points;
        points.push_back(Point3D(0, 0, z));
        points.push_back(Point3D(0, 1, z));
        points.push_back(Point3D(1, 1, z));
        points.push_back(Point3D(1, 0, z));

        // build initial polygons from point sequence 
        long lPolygonSequentialNumber = -1;
        if ( ! addPolygonFromPoints(IN points, OUT lPolygonSequentialNumber, OUT sErrorMsg))
        {
            return false;
        }
    }

    // side faces
    for (int nI = 0; nI < 2; nI++)
    {
        double x;

        if (nI == 0)
        {
            x = 0; 
            // "Left side";
        }
        else
        {
            x = 1; 
            // "Right side";
        }

        std::vector<Point3D> points;
        points.push_back(Point3D(x, 0, 0));
        points.push_back(Point3D(x, 1, 0));
        points.push_back(Point3D(x, 1, 1));
        points.push_back(Point3D(x, 0, 1));

        // build initial polygons from point sequence 
        long lPolygonSequentialNumber = -1;
        if ( ! addPolygonFromPoints(IN points, OUT lPolygonSequentialNumber, OUT sErrorMsg))
        {
            return false;
        }
    }

    // front/back faces
    for (int nI = 0; nI < 2; nI++)
    {
        double y;

        if (nI == 0)
        {
            y = 0; 
            //sFaceName = "Front side";
        }
        else
        {
            y = 1; 
            //sFaceName = "back side";
        }

        std::vector<Point3D> points;
        points.push_back(Point3D(0, y, 0));
        points.push_back(Point3D(0, y, 1));
        points.push_back(Point3D(1, y, 1));
        points.push_back(Point3D(1, y, 0));

        // build initial polygons from point sequence 
        long lPolygonSequentialNumber = -1;
        if ( ! addPolygonFromPoints(IN points, OUT lPolygonSequentialNumber, OUT sErrorMsg))
        {
            return false;
        }
    }

    return true;
}