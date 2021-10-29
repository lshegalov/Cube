#pragma once

class Point3D
{
public:
    Point3D(double dX, double dY, double dZ);
    Point3D();
    Point3D(const Point3D & rhs);

    Point3D & operator=(const Point3D&);

    void add(double dX, double dY, double dZ);

    double getX() const { return m_dX; }
    double getY() const { return m_dY; }
    double getZ() const { return m_dZ; }

    bool operator ==(const Point3D&) const;
    bool operator !=(const Point3D&) const;
    bool operator <(const Point3D&) const;

    static void getMidPoint(const Point3D& startPoint, const Point3D& endPoint, OUT Point3D & midPoint);

    static bool isInside(const Point3D& startPoint, const Point3D& endPoint, const Point3D & midPoint);

    void getString(std::string & sDisplayString) const;
private:
    double m_dX;
    double m_dY;
    double m_dZ;
};

