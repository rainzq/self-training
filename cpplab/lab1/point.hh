#include <iostream>

class Point {
    private:
        double x;
        double y;
        double z;

    public:
        Point(): x(0.0), y(0.0), z(0.0) {}; // default constructor
        Point(double a, double b, double c): x(a), y(a), z(c) {};

        ~Point() {};

        // Accessor methods
        double get_x() const;
        double get_y() const;
        double get_z() const;

        // Mutator methods
        void set_x(double val);
        void set_y(double val);
        void set_z(double val);

        // calculate the distance to another point
        double distanceTo(const Point &p) const;

    friend std::ostream & operator << (std::ostream &out, const Point & p);
};

