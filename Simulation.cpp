#include "stdafx.h"

#include "Cube.h"
#include "Poligon.h"
#include "Polyhedron.h"
#include "Simulation.h"

Simulation::Simulation():
    m_pPolyhedron(nullptr)
{
    m_szErrorMsg[0] = '\0';
}

Simulation::~Simulation()
{
    delete m_pPolyhedron;
}


bool Simulation::runOnCube(int nNumberOfSteps, 
    __out Results& results,
    __out std::string & sErrorMsg)
{
    delete m_pPolyhedron;
    m_pPolyhedron = new Cube();

    if ( ! m_pPolyhedron->buildSurface(__out sErrorMsg))
    {
        return false;
    }

    for (int nStep = 0; nStep < nNumberOfSteps; nStep++)
    {
        if ( ! m_pPolyhedron->splitOnePolygon(__out sErrorMsg))
        {
            results.nNumberOfSteps = nStep;

            std::string sTempErrorMsg;
            m_pPolyhedron->getStatsPerPolygons(
                __out results,
                __out sTempErrorMsg);

            return false;
        }
        if ((nStep + 1) % 1000 == 0)
        {
            m_pPolyhedron->updateStats(nStep  + 1, __inout results, __out sErrorMsg );
        }
    }

    results.nNumberOfSteps = nNumberOfSteps;


    if ( ! m_pPolyhedron->getStatsPerPolygons(__out results,
                                              __out sErrorMsg))

    {
        return false;
    }

    m_pPolyhedron->updateStats(nNumberOfSteps, __inout results, __out sErrorMsg);
    return true;
}

