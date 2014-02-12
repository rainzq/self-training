#include "point.hh"
#include <cmath>
#include <iostream>

// Accessors
double Point::get_x() const
{
    return x;
}

double Point::get_y() const
{
    return y;
}

double Point::get_z() const
{
    return z;
}

// Mutators
void Point::set_x(double val)
{
    x = val;
}

void Point::set_y(double val)
{
    y = val;
}

void Point::set_z(double val)
{
    z = val;
}

// calculate the distance to another point
double Point::distanceTo(const Point &p) const
{
    double dx, dy, dz, dist;
    dx = x - p.x;
    dy = y - p.y;
    dz = z - p.z;
    dist = sqrt(dx * dx + dy * dy + dz * dz);
    return dist;
}

std::ostream & operator << (std::ostream &out, const Point &p)
{
    out << "(" 
        << p.x
        << ", "
        << p.y
        << ", "
        << p.z
        << ")";
    return out;
}
