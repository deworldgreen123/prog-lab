#ifndef LAB1_POINT_H
#define LAB1_POINT_H
#include <cmath>

class Point{
private:
    double x = 0;
    double y = 0;
public:
    Point();

    Point(double, double);

    Point(const Point &);

    [[nodiscard]] double getX() const;

    [[nodiscard]] double getY() const;

    Point &operator=(const Point &);

    bool operator==(const Point &)const;

    bool operator!=(const Point &)const;
};

double get_len(const Point&, const Point&);

double get_len(const Point&);

double get_cos(const Point&, const Point&);

#endif //LAB1_POINT_H
