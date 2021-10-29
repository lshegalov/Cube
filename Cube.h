#pragma once

#include "Polyhedron.h"

class Cube: public Polyhedron
{
public:
    Cube();
    virtual ~Cube();

    virtual bool buildSurface(OUT std::string& sErrorMsg);
};

