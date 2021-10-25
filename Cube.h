#pragma once

#include "Polyhedron.h"

class Cube: public Polyhedron
{
public:
    Cube();
    virtual ~Cube();

    virtual bool buildSurface(__out std::string& sErrorMsg);
};

