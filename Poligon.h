#pragma once


class DirectionalEdge;

class Polyhedron;

// plane figure that is described by a finite number of straight line segments connected to form a closed polygonal chain


class Poligon
{
public:
    Poligon();
    Poligon(__in Polyhedron* pParent,
            __in long lSequentialNumber,
            __in const std::vector<long> & sideNumbers);

    // must be closed with consecutive sides
    bool checkValidityOfPoligon(__out std::string& sErrorMsg) const;

    size_t getNumberOfSides() const;

    bool getNumberOfPoints(__out size_t * pnNumberOfPoints, __out std::string& sErrorMsg) const;

    const std::vector<long> & getSideNumbers() const { return m_sideNumbers; }

    bool updateSides(__in const std::vector<long> & sideNumbers,
                     __out std::string& sErrorMsg);

    long getSequentialNumber() const { return m_lSequentialNumber; }


private:
    Polyhedron* m_pParent = nullptr;

    long m_lSequentialNumber = -1;

    std::vector<long> m_sideNumbers;
};

