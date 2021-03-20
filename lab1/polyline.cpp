#include "polyline.h"

Polyline::Polyline()=default;

Polyline::Polyline(const vector<Point> &p):
        num_point(p.size()), points(p) {
}

Polyline::Polyline(const Polyline &polyLine):
        num_point(polyLine.num_point), points(polyLine.points){
}

Polyline &Polyline::operator=(const Polyline &polyLine){
    points = polyLine.points;
    num_point = polyLine.num_point;
    return *this;
}

void Polyline::out_points() const{
    cout << num_point << endl;
    for (int i = 0; i < num_point; i++){
        cout << points[i].getX() << ", " << points[i].getY() << endl;
    }
}

double Polyline::Perimeter() const{
    if (perimeter == 0){
        for (int i = 0; i < num_point - 1; i++){
            perimeter += get_len(points[i], points[i+1]);
        }
    }
    return perimeter;
}

Point Polyline::operator[](int i) const {
    return points[i];
}

Point &Polyline::operator[](int i) {
    return points[i];
}

int Polyline::Size() const{
    return num_point;
}

Point Polyline::back() const {
    return points.back();
}

Closed_polyline::Closed_polyline()=default;

Closed_polyline::Closed_polyline(const vector<Point> &p):
        Polyline(p){
    if (p[0]!=p[p.size()-1]){
        num_point += 1;
        points.push_back(p[0]);
    }
}

Closed_polyline::Closed_polyline(const Closed_polyline &closedPolyline)
        :Polyline(closedPolyline){
}