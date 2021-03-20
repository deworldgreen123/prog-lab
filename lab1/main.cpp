#include <vector>
#include "point.h"
#include "polyline.h"
#include "polygon.h"

using namespace std;

int main() {
    Point df(0, 0);
    Point fd(5, 0);
    Point ff(5, 5);
    Point dd(0, 6);

    vector<Point> arr;
    arr.push_back(df);
    arr.push_back(fd);
    arr.push_back(ff);
    arr.push_back(dd);

    Regular_polygon d1(arr);
    d1.out_points();

    return 0;
}
