#include "point.hh"
#include <iostream>
#include <cmath>
using namespace std;

// using Heron's Formula to compute the area of triangle
// Heron's Formula: area = sqrt(s*(s-a)*(s-b)*(s-c))
// s = (a + b + c) / 2
// a, b, c is the edges of triangle
double computeArea(const Point &pa, const Point &pb, const Point &pc)
{
    double da, db, dc, s, area;
    da = pb.distanceTo(pc);
    db = pa.distanceTo(pc);
    dc = pa.distanceTo(pb);
    s  = (da + db + dc) / 2;
    area = sqrt(s * (s - da) * (s - db) * (s - dc));
    return area;
}

int main()
{
    double x, y, z;
    cin >> x >> y >> z;
    Point a(x, y, z);
    cin >> x >> y >> z;
    Point b(x, y, z);
    cin >> x >> y >> z;
    Point c(x, y, z);

    double area = computeArea(a, b, c);
    cout << "The area of " << a << b << c 
        << " is: " << area << endl;
    return 0;
}
