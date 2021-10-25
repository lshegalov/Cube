#pragma once

class Polyhedron;



class Simulation
{
public:
    Simulation();
    ~Simulation();

    bool runOnCube(__in int nNumberOfSteps, __out Results & results, __out std::string & sErrorMsg);

protected:


protected:
    char m_szErrorMsg[ERROR_MSG_LEN];

    Polyhedron* m_pPolyhedron = nullptr;
};

