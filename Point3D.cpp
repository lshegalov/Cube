#include "stdafx.h"

#include "Point3D.h"

Point3D::Point3D(double dX, double dY, double dZ)
{
    m_dX = dX;
    m_dY = dY;
    m_dZ = dZ;
}

Point3D::Point3D()
{
    m_dX = 0;
    m_dY = 0;
    m_dZ = 0;
}

Point3D::Point3D(const Point3D & rhs):
    m_dX(0),
    m_dY(0),
    m_dZ(0)
{
    if (this == &rhs)
        return;

    m_dX = rhs.m_dX;
    m_dY = rhs.m_dY;
    m_dZ = rhs.m_dZ;
}

Point3D & Point3D::operator=(const Point3D & rhs)
{
    if (this == &rhs)
        return *this;

    m_dX = rhs.m_dX;
    m_dY = rhs.m_dY;
    m_dZ = rhs.m_dZ;

    return *this;
}

bool Point3D::operator ==(const Point3D & rhs) const
{
    if (fabs(m_dX - rhs.m_dX) > DBL_EPSILON)
    {
        return false;
    }

    if (fabs(m_dY - rhs.m_dY) > DBL_EPSILON)
    {
        return false;
    }


    if (fabs(m_dZ - rhs.m_dZ) > DBL_EPSILON)
    {
        return false;
    }

    return true;
}


bool Point3D::operator !=(const Point3D& rhs) const
{
    if (fabs(m_dX - rhs.m_dX) > DBL_EPSILON)
    {
        return true;
    }

    if (fabs(m_dY - rhs.m_dY) > DBL_EPSILON)
    {
        return true;
    }


    if (fabs(m_dZ - rhs.m_dZ) > DBL_EPSILON)
    {
        return true;
    }

    return false;
}


bool Point3D::operator <(const Point3D& rhs) const
{
    if (m_dX < rhs.m_dX)
    {
        return true;
    }

    if (m_dX > rhs.m_dX)
    {
        return false;
    }

    if (m_dY < rhs.m_dY)
    {
        return true;
    }

    if (m_dY > rhs.m_dY)
    {
        return false;
    }


    if (m_dZ < rhs.m_dZ)
    {
        return true;
    }

    return false;
}

void Point3D::add(double dX, double dY, double dZ)
{
    m_dX += dX;
    m_dY += dY;
    m_dZ += dZ;
}

// static
void Point3D::getMidPoint(const Point3D& startPoint, const Point3D& endPoint, __out Point3D& midPoint) 
{
    midPoint.m_dX = (startPoint.m_dX + endPoint.m_dX) / 2.;

    midPoint.m_dY = (startPoint.m_dY + endPoint.m_dY) / 2.;

    midPoint.m_dZ = (startPoint.m_dZ + endPoint.m_dZ) / 2.;
}

//static 
bool Point3D::isInside(const Point3D& startPoint, const Point3D& endPoint, const Point3D& midPoint)
{
    if (startPoint.m_dX < endPoint.m_dX)
    {
        if (midPoint.m_dX < startPoint.m_dX)
        {
            return false;
        }

        if (midPoint.m_dX > endPoint.m_dX)
        {
            return false;
        }
    }
    else
    {
        // startPoint.m_dX >= endPoint.m_dX
        if (midPoint.m_dX > startPoint.m_dX)
        {
            return false;
        }

        if (midPoint.m_dX < endPoint.m_dX)
        {
            return false;
        }
    }

    if (startPoint.m_dY < endPoint.m_dY)
    {
        if (midPoint.m_dY < startPoint.m_dY)
        {
            return false;
        }

        if (midPoint.m_dY > endPoint.m_dY)
        {
            return false;
        }
    }
    else
    {
        // startPoint.m_dY >= endPoint.m_dY
        if (midPoint.m_dY > startPoint.m_dY)
        {
            return false;
        }

        if (midPoint.m_dY < endPoint.m_dY)
        {
            return false;
        }
    }

    if (startPoint.m_dZ < endPoint.m_dZ)
    {
        if (midPoint.m_dZ < startPoint.m_dZ)
        {
            return false;
        }

        if (midPoint.m_dZ > endPoint.m_dZ)
        {
            return false;
        }
    }
    else
    {
        // startPoint.m_dZ >= endPoint.m_dZ
        if (midPoint.m_dZ > startPoint.m_dZ)
        {
            return false;
        }

        if (midPoint.m_dZ < endPoint.m_dZ)
        {
            return false;
        }
    }

    if (midPoint == startPoint)
    {
        return false;
    }

    if (midPoint == endPoint)
    {
        return false;
    }

    return true;
}

void Point3D::getString(std::string& sDisplayString) const
{
    char szErrorMsg[ERROR_MSG_LEN];
 

    sprintf_s(szErrorMsg, "%.2f:%.2f:%.2f",
              m_dX, m_dY, m_dZ);

    sDisplayString = szErrorMsg;
}
