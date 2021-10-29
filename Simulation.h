#pragma once

class Polyhedron;


class Simulation
{
public:
    Simulation();
    ~Simulation();

    bool runOnCube(IN int nNumberOfSteps, OUT Results & results, OUT std::string & sErrorMsg);

protected:


protected:
    char m_szErrorMsg[ERROR_MSG_LEN];

    Polyhedron* m_pPolyhedron = nullptr;
};

