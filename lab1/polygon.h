#ifndef LAB1_POLYGON_H
#define LAB1_POLYGON_H
#include "point.h"
#include "polyline.h"
#include <exception>
#include <vector>
#include <cmath>

using namespace std;

class Polygon{
protected:
    Polyline poly;
    mutable double area = 0;
public:
    Polygon();

    explicit Polygon(const vector<Point>&);

    Polygon(const Polygon&);

    Polygon &operator=(const Polygon &);

    void out_points() const;

    virtual double Perimeter() const;

    virtual double Area() const;

    bool Check() const;
};

class Triangle: public Polygon{
public:
    Triangle();

    explicit Triangle(const vector<Point>&);

    Triangle(const Point&, const Point&, const Point&);

    Triangle(const Triangle &);

    Triangle &operator=(const Triangle &);

    double Area() const override ;
};

class Trapeze: public Polygon{
public:
    Trapeze();

    explicit Trapeze(const vector<Point>&);

    Trapeze(const Point&, const Point&, const Point&, const Point&);

    Trapeze(const Trapeze &);

    Trapeze &operator=(const Trapeze &);

    bool Check_trapeze();
};

class Regular_polygon: public Polygon{
public:
    Regular_polygon();

    explicit Regular_polygon(const vector<Point>&);

    Regular_polygon(const Regular_polygon &);

    Regular_polygon &operator=(const Regular_polygon &);

    double Perimeter() const override;

    double Area() const override ;

    bool Check_RP();
};


#endif //LAB1_POLYGON_H
