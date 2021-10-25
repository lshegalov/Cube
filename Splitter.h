#pragma once

#include "Point3D.h"

class Polyhedron;

class Splitter
{
public:
    Splitter(Polyhedron* pPolyhedron);

    bool splitPoligon(__inout Poligon& oldPolygon, 
                      __out long& lNewPolygonSequentialNumber,
                      __out std::string& sErrorMsg);

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

    bool createSplittedEdgeInfos(__in const Poligon& oldPolygon, __out std::string& sErrorMsg);

    bool generateRandomEdgesToSplit(__in const Poligon& oldPolygon, 
                                    __out std::set<int> & edgesSidesToSplit,
                                    __out std::string& sErrorMsg);

    bool checkValidityOfSelectedSides(__in const std::vector<long>& oldSideNumbers, // side sequential numbers
                                      __in const std::set<int> & edgesSidesToSplit,
                                      __out std::string& sErrorMsg);

    SplittedEdgeInfo* getFirstSplitInfo();

    SplittedEdgeInfo* getSecondSplitInfo();

    // split existing 2 sides into two each
    bool createFourNewSides(__out std::string& sErrorMsg);

    bool createTwoNewSidesOutOfOldOne(__inout SplittedEdgeInfo& splitInfo, // returns lFirstHalfSideSequentialNumber,
                                                                          // lSecondHalfSideSequentialNumber,
                                      __out std::string& sErrorMsg);

    // insert midPoint into old side and its opposite
    bool insertMidPointInTheOldSide(
        __in const SplittedEdgeInfo & splitInfo, 
        __out std::string& sErrorMsg);

    // create new sides between mid points
    bool insertMidPointSides(__out std::string& sErrorMsg);

    bool insertPointIntoSide(__in long lSideSequentialNumber,
                             __in const Point3D & newPoint,
                             __out std::string& sErrorMsg);

    bool makeNewPolygonAndUpdateOld(__inout Poligon& oldPolygon,
                                    __out long& lNewPolygonSequentialNumber,
                                    __out std::string& sErrorMsg);

private:
    Polyhedron* m_pPolyhedron = nullptr;

    char m_szErrorMsg[ERROR_MSG_LEN];

    std::map<int, // edge # in oriented edges
             SplittedEdgeInfo> m_splitInfos;
};

