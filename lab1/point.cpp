#include "point.h"

using namespace std;

Point::Point() = default;

Point::Point(double x_new, double y_new) {
    x = x_new;
    y = y_new;
}

Point::Point(const Point &point):
    x(point.x), y(point.y){
}

double Point::getX() const {
    return x;
}

double Point::getY() const {
    return y;
}

Point &Point::operator=(const Point &point) {
    x = point.x;
    y = point.y;
    return *this;
}

bool Point::operator==(const Point &p1) const {
    return (p1.x == x && p1.y == y);
}

bool Point::operator!=(const Point &p1) const {
    return (p1.x != x || p1.y != y);
}

double get_len(const Point& p1, const Point& p2){
    return sqrt(pow(p1.getX() - p2.getX(), 2) + pow(p1.getY() - p2.getY(), 2));
}

double get_len(const Point &p1){
    return sqrt(pow(p1.getX(), 2) + pow(p1.getY(), 2));
}

double get_cos(const Point& p1, const Point& p2){
    double answer = p1.getX() * p2.getY() + p2.getX() * p1.getY();
    answer = answer/get_len(p1)/get_len(p2);
    return acos(abs(answer));
}