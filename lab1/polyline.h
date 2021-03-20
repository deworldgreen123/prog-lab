#ifndef LAB1_POLYLINE_H
#define LAB1_POLYLINE_H
#include "point.h"
#include <iostream>
#include <vector>

using namespace std;

class Polyline{
protected:
    vector<Point> points;
    int num_point = 0;
    mutable double perimeter = 0;
public:
    Polyline();

    explicit Polyline(const vector<Point>&);

    Polyline(const Polyline &);

    Polyline &operator=(const Polyline &);

    double Perimeter() const;

    void out_points() const;

    Point operator[](int) const;

    Point &operator[](int);

    int Size() const;

    Point back() const;
};

class Closed_polyline: public Polyline{
public:
    Closed_polyline();

    explicit Closed_polyline(const vector<Point>&);

    Closed_polyline(const Closed_polyline &);
};
#endif //LAB1_POLYLINE_H
