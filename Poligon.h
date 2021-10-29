#pragma once


class DirectionalEdge;

class Polyhedron;

// plane figure that is described by a finite number of straight line segments connected to form a closed polygonal chain


class Poligon
{
public:
    Poligon();
    Poligon(IN Polyhedron* pParent,
            IN long lSequentialNumber,
            IN const std::vector<long> & sideNumbers);

    // must be closed with consecutive sides
    bool checkValidityOfPoligon(OUT std::string& sErrorMsg) const;

    size_t getNumberOfSides() const;

    bool getNumberOfPoints(OUT size_t * pnNumberOfPoints, OUT std::string& sErrorMsg) const;

    const std::vector<long> & getSideNumbers() const { return m_sideNumbers; }

    bool updateSides(IN const std::vector<long> & sideNumbers,
                     OUT std::string& sErrorMsg);

    long getSequentialNumber() const { return m_lSequentialNumber; }


private:
    Polyhedron* m_pParent = nullptr;

    long m_lSequentialNumber = -1;

    std::vector<long> m_sideNumbers;
};

