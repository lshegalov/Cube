#pragma once

#include "Point3D.h"

class Polyhedron;

class Splitter
{
public:
    Splitter(Polyhedron* pPolyhedron);

    bool splitPoligon(__inout Poligon& oldPolygon, 
                      OUT long& lNewPolygonSequentialNumber,
                      OUT std::string& sErrorMsg);

private:
    struct SplittedEdgeInfo
    {
        int nSide; // side # in DirectionalSide(s)

        long lOldSideSequentialNumber;

        Point3D startPoint;
        Point3D endPoint;

        // new info

        Point3D midPoint;

        // inside the same sequential number
        long lFirstHalfSideSequentialNumber;
        long lSecondHalfSideSequentialNumber;

        // started from mid point
        long lInsertedMidPointSideNumber;

        SplittedEdgeInfo()
        {
            nSide = -1; // nSide # in DirectionalSide(s)

            lOldSideSequentialNumber = -1;

            lFirstHalfSideSequentialNumber = -1;

            lSecondHalfSideSequentialNumber = -1;

            // started from mid point
            lInsertedMidPointSideNumber = -1;
        }
    };

    bool createSplittedEdgeInfos(IN const Poligon& oldPolygon, OUT std::string& sErrorMsg);

    bool generateRandomEdgesToSplit(IN const Poligon& oldPolygon, 
                                    OUT std::set<int> & edgesSidesToSplit,
                                    OUT std::string& sErrorMsg);

    bool checkValidityOfSelectedSides(IN const std::vector<long>& oldSideNumbers, // side sequential numbers
                                      IN const std::set<int> & edgesSidesToSplit,
                                      OUT std::string& sErrorMsg);

    SplittedEdgeInfo* getFirstSplitInfo();

    SplittedEdgeInfo* getSecondSplitInfo();

    // split existing 2 sides into two each
    bool createFourNewSides(OUT std::string& sErrorMsg);

    bool createTwoNewSidesOutOfOldOne(__inout SplittedEdgeInfo& splitInfo, // returns lFirstHalfSideSequentialNumber,
                                                                          // lSecondHalfSideSequentialNumber,
                                      OUT std::string& sErrorMsg);

    // insert midPoint into old side and its opposite
    bool insertMidPointInTheOldSide(
        IN const SplittedEdgeInfo & splitInfo, 
        OUT std::string& sErrorMsg);

    // create new sides between mid points
    bool insertMidPointSides(OUT std::string& sErrorMsg);

    bool insertPointIntoSide(IN long lSideSequentialNumber,
                             IN const Point3D & newPoint,
                             OUT std::string& sErrorMsg);

    bool makeNewPolygonAndUpdateOld(__inout Poligon& oldPolygon,
                                    OUT long& lNewPolygonSequentialNumber,
                                    OUT std::string& sErrorMsg);

private:
    Polyhedron* m_pPolyhedron = nullptr;

    char m_szErrorMsg[ERROR_MSG_LEN];

    std::map<int, // edge # in oriented edges
             SplittedEdgeInfo> m_splitInfos;
};

