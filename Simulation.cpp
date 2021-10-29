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
    OUT Results& results,
    OUT std::string & sErrorMsg)
{
    delete m_pPolyhedron;
    m_pPolyhedron = new Cube();

    if ( ! m_pPolyhedron->buildSurface(OUT sErrorMsg))
    {
        return false;
    }

    for (int nStep = 0; nStep < nNumberOfSteps; nStep++)
    {
        if ( ! m_pPolyhedron->splitOnePolygon(OUT sErrorMsg))
        {
            results.nNumberOfSteps = nStep;

            std::string sTempErrorMsg;
            m_pPolyhedron->getStatsPerPolygons(
                OUT results,
                OUT sTempErrorMsg);

            return false;
        }
        if ((nStep + 1) % 1000 == 0)
        {
            m_pPolyhedron->updateStats(nStep  + 1, __inout results, OUT sErrorMsg );
        }
    }

    results.nNumberOfSteps = nNumberOfSteps;


    if ( ! m_pPolyhedron->getStatsPerPolygons(OUT results,
                                              OUT sErrorMsg))

    {
        return false;
    }

    m_pPolyhedron->updateStats(nNumberOfSteps, __inout results, OUT sErrorMsg);
    return true;
}

