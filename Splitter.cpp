#include "stdafx.h"
#include "Poligon.h"
#include "Polyhedron.h"

#include "Splitter.h"

Splitter::Splitter(Polyhedron* pPolyhedron):
    m_pPolyhedron(pPolyhedron)
{

}

bool Splitter::splitPoligon(
    __inout Poligon& oldPolygon, 
    __out long & lNewPolygonSequentialNumber,
    __out std::string& sErrorMsg)
{
    lNewPolygonSequentialNumber = -1;

    const long lOldPolygonSequentialNumber = oldPolygon.getSequentialNumber();

    if ( ! createSplittedEdgeInfos(__in oldPolygon, __out sErrorMsg))
    {
        return false;
    }

    // split existing 2 sides to split into two each
    if ( ! createFourNewSides(__out sErrorMsg))
    {
        return false;
    }

    // create new sides between mid points
    if ( ! insertMidPointSides(__out sErrorMsg))
    {
        return false;
    }

    if ( ! makeNewPolygonAndUpdateOld(__inout oldPolygon,
                                      __out lNewPolygonSequentialNumber,
                                      __out sErrorMsg))
    {
        return false;
    }

    for (std::map<int, // side # in oriented edges
        SplittedEdgeInfo>::const_iterator splitIter = m_splitInfos.cbegin();
        splitIter != m_splitInfos.cend(); splitIter++)
    {
        int nEdge = splitIter->first;
        const SplittedEdgeInfo& splitInfo = splitIter->second;

        // insert midPoint into old side and its opposite
        if ( ! insertMidPointInTheOldSide(__in splitInfo,
                                          __out sErrorMsg))
        {
            return false;
        }
    }

    // clean up if that side is not used in any polygons
    for (std::map<int, // side # in oriented edges
        SplittedEdgeInfo>::const_iterator splitIter = m_splitInfos.cbegin();
        splitIter != m_splitInfos.cend(); splitIter++)
    {
        int nEdge = splitIter->first;
        const SplittedEdgeInfo& splitInfo = splitIter->second;

        // clean up if that side is not used in any polygons
        if ( ! m_pPolyhedron->deleteUnusedSide(splitInfo.lOldSideSequentialNumber, __out sErrorMsg))
        {
            return false;
        }
    }

    return true;
}

bool Splitter::createSplittedEdgeInfos(__in const Poligon& oldPolygon, __out std::string& sErrorMsg)
{
    const int nNumberOfSides = (int) oldPolygon.getNumberOfSides();

    const std::vector<long>& oldSideNumbers = oldPolygon.getSideNumbers();


    std::set<int> edgesSidesToSplit;
    if ( ! generateRandomEdgesToSplit(__in oldPolygon,
                                      __out edgesSidesToSplit,
                                      __out sErrorMsg))
    {
        return false;
    }

    assert(edgesSidesToSplit.size() == 2);

    sErrorMsg = "";
    if ( ! checkValidityOfSelectedSides(__in oldSideNumbers, // side sequential numbers
                                        __in edgesSidesToSplit,
                                        __out sErrorMsg))
    {
            return false;
    }

    return true;

}

bool Splitter::checkValidityOfSelectedSides(
    __in const std::vector<long>& oldSideNumbers, // side sequential numbers
    __in const std::set<int> & edgesSidesToSplit,
    __out std::string& sErrorMsg)
{
    m_splitInfos.clear();

    int nSide = 0;
    for (std::vector<long>::const_iterator sideIter = oldSideNumbers.cbegin();
        sideIter != oldSideNumbers.cend(); sideIter++, nSide++)
    {
        if (edgesSidesToSplit.count(nSide) == 0)
        {
            continue;
        }

        const long lSideSequentialNumber = *sideIter;

        SplittedEdgeInfo splitInfo;
        splitInfo.nSide = nSide;
        splitInfo.lOldSideSequentialNumber = lSideSequentialNumber;

        DirectionalSide * pSide = nullptr;
        if ( ! m_pPolyhedron->getSide(__in lSideSequentialNumber,
                                      __out & pSide,
                                      __out sErrorMsg))
        {
            return false;
        }

        splitInfo.startPoint = pSide->getStartPoint();
        splitInfo.endPoint = pSide->getEndPoint();

        Point3D::getMidPoint(__in splitInfo.startPoint, __in splitInfo.endPoint, __out splitInfo.midPoint);

        m_splitInfos[nSide] = splitInfo;

        if (m_splitInfos.size() == 2)
            break;
    }

    // Exclude a rare case of equal mid points
    SplittedEdgeInfo* pFirstSplitInfo = getFirstSplitInfo();
    if (pFirstSplitInfo == nullptr)
    {
        sprintf_s(m_szErrorMsg, "%s:%d pFirstSplitInfo == nullptr \n",
            __FILE__, __LINE__);
        sErrorMsg = m_szErrorMsg;
        return false;
    }

    SplittedEdgeInfo* pSecondSplitInfo = getSecondSplitInfo();
    if (pSecondSplitInfo == nullptr)
    {
        sprintf_s(m_szErrorMsg, "%s:%d pSecondSplitInfo == nullptr \n",
            __FILE__, __LINE__);
        sErrorMsg = m_szErrorMsg;
        return false;
    }

    if (pFirstSplitInfo->midPoint == pSecondSplitInfo->midPoint)
    {
       sprintf_s(m_szErrorMsg, "%s:%d midpoints are the same  (%f,%f,%f) == (%f,%f,%f) \n",
                 __FILE__, __LINE__,
                 pFirstSplitInfo->midPoint.getX(), pFirstSplitInfo->midPoint.getY(), pFirstSplitInfo->midPoint.getZ(),
                 pSecondSplitInfo->midPoint.getX(), pSecondSplitInfo->midPoint.getY(), pSecondSplitInfo->midPoint.getZ());
        sErrorMsg = m_szErrorMsg;
        return false;
    }

    return true;
}

bool Splitter::generateRandomEdgesToSplit(
    __in const Poligon& oldPolygon,
    __out std::set<int> & edgesSidesToSplit,
    __out std::string& sErrorMsg)
{
    const int nNumberOfSides = (int) oldPolygon.getNumberOfSides();

    edgesSidesToSplit.clear();

    int nMaxAttempts = 50;
    int nAttempt = 0;
    while (nAttempt < nMaxAttempts)
    {
        nAttempt++;

        // rand() generate random numbers in the range [0, RAND_MAX). 
        int nIndex = (int) (nNumberOfSides * (1.0 * rand()) / RAND_MAX);

        if (nIndex >= nNumberOfSides)
        {
            continue;
        }

        edgesSidesToSplit.insert(nIndex);

        if (edgesSidesToSplit.size() == 2)
        {
            return true;
        }
    }

    sprintf_s(m_szErrorMsg, "%s:%d nAttempt=%d >= nMaxAttempts=%d \n",
        __FILE__, __LINE__, nAttempt, nMaxAttempts);
    sErrorMsg = m_szErrorMsg;
    return false;
}

Splitter::SplittedEdgeInfo * Splitter::getFirstSplitInfo()
{
    if (m_splitInfos.empty())
        return nullptr;

    std::map<int, // edge # in oriented edges
        SplittedEdgeInfo>::iterator splitIter = m_splitInfos.begin();
    if (splitIter == m_splitInfos.end())
    {
        return nullptr;
    }

    SplittedEdgeInfo& splitInfo = splitIter->second;
    return &splitInfo;
}

Splitter::SplittedEdgeInfo* Splitter::getSecondSplitInfo()
{
    if (m_splitInfos.empty())
        return nullptr;

    std::map<int, // edge # in oriented edges
        SplittedEdgeInfo>::iterator splitIter = m_splitInfos.begin();
    if (splitIter == m_splitInfos.end())
    {
        return nullptr;
    }

    splitIter++;
    if (splitIter == m_splitInfos.end())
    {
        return nullptr;
    }

    SplittedEdgeInfo& splitInfo = splitIter->second;
    return &splitInfo;
}

// split existing 2 sides to split into two each
bool Splitter::createFourNewSides(
    __out std::string& sErrorMsg)
{
    for (std::map<int, // edge # in oriented edges
        SplittedEdgeInfo>::iterator splitIter = m_splitInfos.begin();
        splitIter != m_splitInfos.end(); splitIter++)
    {
        //int nSide = splitIter->first;
        SplittedEdgeInfo & splitInfo = splitIter->second;

        if ( ! createTwoNewSidesOutOfOldOne(__inout splitInfo, // returns lFirstHalfSideSequentialNumber,
                                                               // lSecondHalfSideSequentialNumber,
                                            __out sErrorMsg))
        {
            return false;
        }
    }

    return true;
}


bool Splitter::createTwoNewSidesOutOfOldOne(
    __inout SplittedEdgeInfo& splitInfo, // returns lFirstHalfSideSequentialNumber,
                                         // lSecondHalfSideSequentialNumber,
    __out std::string& sErrorMsg)
{
    splitInfo.lFirstHalfSideSequentialNumber = -1;
    splitInfo.lSecondHalfSideSequentialNumber = -1;

    DirectionalSide* pOldSide = nullptr;
    if ( ! m_pPolyhedron->getSide(__in splitInfo.lOldSideSequentialNumber,
        __out & pOldSide,
        __out sErrorMsg))
    {
        return false;
    }

    for (int nPart = 0; nPart < 2; nPart++)
    {
        Point3D startIntervalPoint;
        Point3D endIntervalPoint;

        if (nPart == 0)
        {
            startIntervalPoint = splitInfo.startPoint;
            endIntervalPoint = splitInfo.midPoint;
        }
        else
        {
            startIntervalPoint = splitInfo.midPoint;
            endIntervalPoint = splitInfo.endPoint;
        }

        long lNewSideSequentialNumber = -1;

        // add side if it does not exist, otherwise return existing lSideSequentialNumber
        if (!m_pPolyhedron->addSide(__in startIntervalPoint,
                                    __in endIntervalPoint,
                                    __out lNewSideSequentialNumber,
                                    __out sErrorMsg))
        {
            return false;
        }

        if (TEST_SIDE == lNewSideSequentialNumber)
        {
            int k = 0;
        }

        if (nPart == 0)
        {
            splitInfo.lFirstHalfSideSequentialNumber = lNewSideSequentialNumber;
        }
        else
        {
            splitInfo.lSecondHalfSideSequentialNumber = lNewSideSequentialNumber;
        }

        std::set<Point3D> internalPoints;

        // points between (not including borders
        pOldSide->getInternalPoints(__in startIntervalPoint, __in endIntervalPoint,
                                    __out internalPoints);

        if (internalPoints.empty())
        {
            continue;
        }

        DirectionalSide* pNewSide = nullptr;
        if ( ! m_pPolyhedron->getSide(__in lNewSideSequentialNumber,
                                        __out & pNewSide,
                                        __out sErrorMsg))
        {
            return false;
        }

        pNewSide->insertInternalPoints(internalPoints);
    }

    return true;
}

// insert midPoint into old side and its opposite
bool Splitter::insertMidPointInTheOldSide(
    __in const SplittedEdgeInfo& splitInfo,
    __out std::string& sErrorMsg)
{
    // replace lOldEdgeSequentialNumber by lFirstHalfEdgeSequentialNumber, lSecondHalfEdgeSequentialNumber for all polygons except for lOldPolygonSequentialNumber
    if ( ! m_pPolyhedron->insertPointIntoSide(__in splitInfo.lOldSideSequentialNumber,
                                              __in splitInfo.midPoint,
                                              __out sErrorMsg))
    {
        return false;
    }

    DirectionalSide* pSide = nullptr;
    if ( ! m_pPolyhedron->getSide(__in splitInfo.lOldSideSequentialNumber,
                                  __out & pSide,
                                  __out sErrorMsg))
    {
        return false;
    }

    long lOppositeSequentialNumber = pSide->getOppositeSideSequentialNumber();
    if (lOppositeSequentialNumber < 0)
    {
        return true;
    }

    return m_pPolyhedron->insertPointIntoSide(__in lOppositeSequentialNumber,
                                                __in splitInfo.midPoint,
                                                __out sErrorMsg);
}

// create new sides between mid points
bool Splitter::insertMidPointSides(
    __out std::string& sErrorMsg)
{
    SplittedEdgeInfo* pFirstSplitInfo = getFirstSplitInfo();
    if (pFirstSplitInfo == nullptr)
    {
        sprintf_s(m_szErrorMsg, "%s:%d pFirstSplitInfo == nullptr \n",
            __FILE__, __LINE__);
        sErrorMsg = m_szErrorMsg;
        return false;
    }

    SplittedEdgeInfo* pSecondSplitInfo = getSecondSplitInfo();
    if (pSecondSplitInfo == nullptr)
    {
        sprintf_s(m_szErrorMsg, "%s:%d pSecondSplitInfo == nullptr \n",
            __FILE__, __LINE__);
        sErrorMsg = m_szErrorMsg;
        return false;
    }


    if (! m_pPolyhedron->addSide(__in pFirstSplitInfo->midPoint, // startPoint,
                                 __in pSecondSplitInfo->midPoint,
                                 __out pFirstSplitInfo->lInsertedMidPointSideNumber,
                                 __out sErrorMsg))
    {
        return false;
    }

    if ( ! m_pPolyhedron->addSide(__in pSecondSplitInfo->midPoint, // startPoint,
                                  __in pFirstSplitInfo->midPoint,
                                  __out pSecondSplitInfo->lInsertedMidPointSideNumber,
                                  __out sErrorMsg))
    {
        return false;
    }

    DirectionalSide* pFirstSide = nullptr;
    if ( ! m_pPolyhedron->getSide(__in pFirstSplitInfo->lInsertedMidPointSideNumber,
                                 __out & pFirstSide,
                                 __out sErrorMsg))
    {
        return false;
    }
    pFirstSide->setOppositeSideNumber(pSecondSplitInfo->lInsertedMidPointSideNumber);

    DirectionalSide* pSecondSide = nullptr;
    if ( ! m_pPolyhedron->getSide(__in pSecondSplitInfo->lInsertedMidPointSideNumber,
                                  __out & pSecondSide,
                                   __out sErrorMsg))
    {
        return false;
    }

    pSecondSide->setOppositeSideNumber(pFirstSplitInfo->lInsertedMidPointSideNumber);

    return true;
}

bool Splitter::makeNewPolygonAndUpdateOld(
    __inout Poligon& oldPolygon,
    __out long& lNewPolygonSequentialNumber,
    __out std::string& sErrorMsg)
{
    SplittedEdgeInfo* pFirstSplitInfo = getFirstSplitInfo();
    if (pFirstSplitInfo == nullptr)
    {
        sprintf_s(m_szErrorMsg, "%s:%d pFirstSplitInfo == nullptr \n",
            __FILE__, __LINE__);
        sErrorMsg = m_szErrorMsg;
        return false;
    }

    SplittedEdgeInfo* pSecondSplitInfo = getSecondSplitInfo();
    if (pSecondSplitInfo == nullptr)
    {
        sprintf_s(m_szErrorMsg, "%s:%d pSecondSplitInfo == nullptr \n",
            __FILE__, __LINE__);
        sErrorMsg = m_szErrorMsg;
        return false;
    }
    
    std::vector<long> oldSideNumbers = oldPolygon.getSideNumbers();

    std::vector<long> firstSideNumbers;
    std::vector<long> secondSideNumbers;

    int nSide = 0;

    enum Stages
    {
        FirstPartStage,  // before first stage
        SecondPartStage, // After first split
        ThirdPartStage   // After second split
    };
    Stages eStage = FirstPartStage;
    for (std::vector<long>::const_iterator sideIter = oldSideNumbers.cbegin();
        sideIter != oldSideNumbers.cend(); sideIter++, nSide++)
    {
        const long lSideSequentialNumber = *sideIter;

        if ((pFirstSplitInfo->lOldSideSequentialNumber != lSideSequentialNumber) &&
            (pSecondSplitInfo->lOldSideSequentialNumber != lSideSequentialNumber))
        {
            switch (eStage)
            {
            case FirstPartStage:  // before first stage
            case ThirdPartStage:   // After second split

                firstSideNumbers.push_back(lSideSequentialNumber);
                break;

            case SecondPartStage: // After first split
                secondSideNumbers.push_back(lSideSequentialNumber);
                break;
            default:
                assert(false);
            }
            continue;
        }

        // first split
        if (pFirstSplitInfo->lOldSideSequentialNumber == lSideSequentialNumber)
        {
            firstSideNumbers.push_back(pFirstSplitInfo->lFirstHalfSideSequentialNumber);
            firstSideNumbers.push_back(pFirstSplitInfo->lInsertedMidPointSideNumber);

            secondSideNumbers.push_back(pFirstSplitInfo->lSecondHalfSideSequentialNumber);

            eStage = SecondPartStage;
            continue;
        }

        // second split
        if (pSecondSplitInfo->lOldSideSequentialNumber == lSideSequentialNumber)
        {
            secondSideNumbers.push_back(pSecondSplitInfo->lFirstHalfSideSequentialNumber);
            secondSideNumbers.push_back(pSecondSplitInfo->lInsertedMidPointSideNumber);

            firstSideNumbers.push_back(pSecondSplitInfo->lSecondHalfSideSequentialNumber);

            eStage = ThirdPartStage;
            continue;
        }
    }

    // update old polygon by firstVector
    // 
    if ( ! oldPolygon.updateSides(__in firstSideNumbers,
                                  __out sErrorMsg))
    {
        return false;
    }


    // new polygon with second vector
    // 
    // in all remaining 
    if ( ! m_pPolyhedron->addPolygonFromSides(__in secondSideNumbers,
                                            __out lNewPolygonSequentialNumber,
                                            __out sErrorMsg))
    {
        return false;
    }

    return true;
}

bool Splitter::insertPointIntoSide(
    __in long lSideSequentialNumber,
    __in const Point3D& newPoint,
    __out std::string& sErrorMsg)
{
    DirectionalSide * pSide = nullptr;

    if ( ! m_pPolyhedron->getSide(__in lSideSequentialNumber,
                                  __out & pSide,
                                  __out sErrorMsg))
    {
        return false;
    }

    if (pSide == nullptr)
    {
        sprintf_s(m_szErrorMsg, "%s:%d pSide == nullptr \n",
                  __FILE__, __LINE__);
        sErrorMsg = m_szErrorMsg;
        return false;
    }

    return pSide->insertOneInternalPoint(newPoint, sErrorMsg);
}