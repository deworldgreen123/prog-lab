#include "polygon.h"

Polygon::Polygon()=default;

Polygon::Polygon(const vector<Point>& p):
        poly(p){
    if (p.size() < 3)
        throw(invalid_argument("YOUR_ERROR: vector.size < 3"));
    if (!Check())
        throw(invalid_argument("YOUR_ERROR: vector is not a polygon"));
}

bool Polygon::Check() const {
    vector<double> prod(poly.Size());
    for (int i = 1; i < poly.Size() - 1; i++){
        Point AB(poly[i].getX() - poly[i - 1].getX(),
                 poly[i].getY() - poly[i - 1].getY());
        Point BC(poly[i + 1].getX() - poly[i].getX(),
                 poly[i + 1].getY() - poly[i].getY());
        prod[i - 1] = AB.getX() * BC.getY() - AB.getY() * BC.getX();
    }
    Point AB(poly[poly.Size() - 1].getX() - poly[poly.Size() - 2].getX(),
             poly[poly.Size() - 1].getY() - poly[poly.Size() - 2].getY());
    Point BC(poly[0].getX() - poly[poly.Size() - 1].getX(),
             poly[0].getY() - poly[poly.Size() - 1].getY());
    Point CD(poly[1].getX() - poly[0].getX(),
             poly[1].getY() - poly[0].getY());
    prod[poly.Size() - 2] = AB.getX() * BC.getY() - AB.getY() * BC.getX();
    prod[poly.Size() - 1] = BC.getX() * CD.getY() - BC.getY() * CD.getX();
    for (int i = 0; i < poly.Size() - 1; i++){
        if (!((prod[i] > 0 && prod[i + 1] > 0)||(prod[i] < 0 && prod[i + 1] < 0))) {
            return false;
        }
    }
    return true;
}

Polygon::Polygon(const Polygon &other):
        poly(other.poly), area(other.area){
}

Polygon &Polygon::operator=(const Polygon &other){
    poly = other.poly;
    area = other.area;
    return *this;
}

void Polygon::out_points() const {
    poly.out_points();
}

double Polygon::Perimeter() const {
    return poly.Perimeter() + get_len(poly[0], poly.back());
}

double Polygon::Area() const {
    if (area == 0) {
        for (int i = 0; i < poly.Size() - 1; i++) {
            area += poly[i].getX() * poly[i + 1].getY() + poly[i + 1].getX() * poly[i].getY();
        }
        area += poly.back().getX() * poly[0].getY() + poly.back().getY() * poly[0].getX();
        area = area / 2.0;
    }
    return area;
}

Triangle::Triangle()=default;

Triangle::Triangle(const vector<Point>& p):
        Polygon(p){
    if (p.size() != 3)
        throw(invalid_argument("YOUR_ERROR: vector.size != 3"));
}

Triangle::Triangle(const Point& p1, const Point& p2, const Point& p3):
        Polygon({p1, p2, p3}){
}

Triangle::Triangle(const Triangle &other):
        Polygon(other){
}

Triangle &Triangle::operator=(const Triangle &other){
    poly = other.poly;
    area = other.area;
    return *this;
}

double Triangle::Area() const {
    double per = Triangle::Perimeter()/2.0;
    area = sqrt(per*(per - get_len(poly[0], poly[1]))*(per - get_len(poly[2], poly[1]))*(per - get_len(poly[0], poly[2])));
    return area;
}

Trapeze::Trapeze()=default;

Trapeze::Trapeze(const vector<Point>& p):
        Polygon(p){
    if (p.size() != 4)
        throw(invalid_argument("YOUR_ERROR: vector.size != 4"));
    if (!Check_trapeze())
        throw(invalid_argument("YOUR_ERROR: vector is not a trapeze"));
}

Trapeze::Trapeze(const Point &p1, const Point &p2, const Point &p3, const Point &p4):
    Polygon({p1, p2, p3, p4}){
    if (!Check())
        throw(invalid_argument("YOUR_ERROR: vector is not a polygon"));
    if (!Check_trapeze())
        throw(invalid_argument("YOUR_ERROR: vector is not a trapeze"));
}

Trapeze::Trapeze(const Trapeze &other):
        Polygon(other){
}

Trapeze &Trapeze::operator=(const Trapeze &other){
    poly = other.poly;
    area = other.area;
    return *this;
}

bool Trapeze::Check_trapeze() {
    Point A = poly[0];
    Point B = poly[1];
    Point C = poly[2];
    Point D = poly[3];
    if (((A.getX() - B.getX()) * (C.getY() - D.getY()) == (A.getY() - B.getY()) * (C.getX() - D.getX())) ||
        ((A.getX() - D.getX()) * (B.getY() - C.getY()) == (A.getY() - D.getY()) * (B.getX() - C.getX())))
        return true;
    return false;
}

Regular_polygon::Regular_polygon()=default;

Regular_polygon::Regular_polygon(const vector<Point>& p)
        :Polygon(p) {
    if (!Check_RP())
        throw(invalid_argument("YOUR_ERROR: vector is not a regular polygon"));
}

bool Regular_polygon::Check_RP() {
    vector<double> prod(poly.Size());
    vector<double> angle(poly.Size());
    for (int i = 1; i < poly.Size() - 1; i++){
        Point AB(poly[i].getX() - poly[i - 1].getX(),
                 poly[i].getY() - poly[i - 1].getY());
        Point BC(poly[i + 1].getX() - poly[i].getX(),
                 poly[i + 1].getY() - poly[i].getY());
        angle[i - 1] = get_cos(AB, BC);
        prod[i - 1] = get_len(AB);
    }
    Point AB(poly[poly.Size() - 1].getX() - poly[poly.Size() - 2].getX(),
             poly[poly.Size() - 1].getY() - poly[poly.Size() - 2].getY());
    Point BC(poly[0].getX() - poly[poly.Size() - 1].getX(),
             poly[0].getY() - poly[poly.Size() - 1].getY());
    Point CD(poly[1].getX() - poly[0].getX(),
             poly[1].getY() - poly[0].getY());
    prod[poly.Size() - 2] = get_len(AB);
    prod[poly.Size() - 1] = get_len(BC);
    angle[poly.Size() - 2] = get_cos(AB, BC);
    angle[poly.Size() - 1] = get_cos(BC, CD);
    for (int i = 0; i < poly.Size() - 1; i++){
        if (!(angle[i] == angle[i + 1]&&prod[i] == prod[i + 1])) {
            return false;
        }
    }
    return true;
}

Regular_polygon::Regular_polygon(const Regular_polygon &other):
        Polygon(other){
}

Regular_polygon &Regular_polygon::operator=(const Regular_polygon &other) {
    poly = other.poly;
    area = other.area;
    return *this;
}

double Regular_polygon::Area() const {
    if (area == 0)
        area = poly.Size() * pow(get_len(poly[0], poly[1]), 2)/(tan(180.0/poly.Size()) * 4.0);
    return area;
}

double Regular_polygon::Perimeter() const {
    return poly.Size() * get_len(poly[0], poly[1]);
}
