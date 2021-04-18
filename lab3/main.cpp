#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "pugixml.hpp"
#include <unordered_map>
#include <fstream>

using namespace std;
using namespace pugi;

enum VEHICLE {
    BUS,
    TRAM,
    TROLLEYBUS,
};

struct coordinates{
    double x;
    double y;
    coordinates() = default;
    coordinates(double x_new , double y_new){
        x = x_new;
        y = y_new;
    }
    explicit coordinates(const string& str){
        string res;
        string str_x;
        string str_y;
        for (auto c : str) {
            if (c == ',') {
                str_x = res;
                res.clear();
            }
            else {
                res += c;
            }
        }
        str_y = res;
        x = stod(str_x);
        y = stod(str_y);
    }
};


struct location{
    string x;
    string y;
    location() = default;

    [[maybe_unused]] location(string x_new , string y_new){
        x = move(x_new);
        y = move(y_new);
    }
    explicit location(const string& str){
        string res;
        for (auto c : str) {
            if (c == ',') {
                x = res;
                res.clear();
            }
            else {
                res += c;
            }
        }
        y = res;
    }
};

vector<string> parseVectro(const string& str){
    vector<string> list;
    string res;
    for (auto c : str) {
        if (c == ',' || c == '.') {
            if (!res.empty()) {
                list.push_back(res);
                res.clear();
            }
            res.clear();
        }
        else {
            res += c;
        }
    }
    if (!res.empty()) {
        list.push_back(res);
    }
    return list;
}

pair<string, int> maxValueStops(const unordered_map<string, vector<coordinates>>& routes_and_coordinates){
    int max_v = 0;
    string max_k;
    for (const auto & routes_and_coordinate : routes_and_coordinates) {
        if (routes_and_coordinate.second.size() > max_v) {
            max_v = routes_and_coordinate.second.size();
            max_k = routes_and_coordinate.first;
        }
    }
    return { max_k, max_v };
}

void maxStops(const unordered_map<string, vector<coordinates>>(&routes_and_coordinates)[3]){
    auto bus = maxValueStops(routes_and_coordinates[BUS]);
    auto tram = maxValueStops(routes_and_coordinates[TRAM]);
    auto trolleybus = maxValueStops(routes_and_coordinates[TROLLEYBUS]);
    cout << "Маршрут с наибольшим количеством остановок для:" << endl;
    cout << "Автобуса - маршрут " << bus.first << " остановок " << bus.second << endl;
    cout << "Трамвая - маршрут " << tram.first << " остановок " << tram.second << endl;
    cout << "Троллейбуса - маршрут " << trolleybus.first << " остановок " << trolleybus.second << endl << endl;
}

double findDist(coordinates p1, coordinates p2) {
    const double RAD = 3.14159265 / 180;
    const int R = 6371;
    double answer = sin((p2.x-p1.x)/2 * RAD) * sin((p2.x-p1.x)/2 * RAD) +
            cos(p1.x * RAD) * cos(p2.x * RAD) * sin((p2.y-p1.y)/2 * RAD) * sin((p2.y-p1.y)/2 * RAD);
    return R * 2 * asin(sqrt(answer));
}

string oneMaxDist(const unordered_map<string, vector<coordinates>>& routes_and_coordinates){
    vector <double> max_distances_for_routes;
    double max_dist_route = 0;
    string max_route;
    for (const auto & routes_and_coordinate : routes_and_coordinates) {
        string route = routes_and_coordinate.first;
        vector <coordinates> coordinates = routes_and_coordinate.second;
        double max_dist = 0;
        for (unsigned i = 0; i < coordinates.size() - 1; ++i) {
            max_dist += findDist(coordinates[i], coordinates[i + 1]);
        }
        if (max_dist > max_dist_route) {
            max_dist_route = max_dist;
            max_route = route;
        }
    }
    return max_route;
}

void maxDist(const unordered_map<string, vector<coordinates>>(&routes_and_coordinates)[3]){
    auto bus = oneMaxDist(routes_and_coordinates[BUS]);
    auto tram = oneMaxDist(routes_and_coordinates[TRAM]);
    auto trolleybus = oneMaxDist(routes_and_coordinates[TROLLEYBUS]);
    cout << "автобуса " << bus << endl;
    cout << "трамвая " << tram << endl;
    cout << "троллейбуса " << trolleybus << endl;
}

void maxValue(unordered_map <string, int>& m) {
    int max_v = 0;
    string max_k;
    for (auto &it : m) {
        if (it.second > max_v) {
            max_v = it.second;
            max_k = it.first;
        }
    }
    cout << "Улица с наибольшим количеством остановок : " << max_k << " - " << max_v << " остановок\n";
}

int main() {
    system("chcp 65001");

    ifstream stream("lab3_spb.xml");

    xml_document doc;
    xml_node station = doc.child("dataset");

    unordered_map<string, vector<coordinates>> routes_and_coordinates[3];
    unordered_map<string, int> street_and_stop;

    for (xml_node transport_station = station.child("transport_station"); transport_station; transport_station = transport_station.next_sibling()){

        string type_of_vehicle = transport_station.child_value("type_of_vehicle");
        string object_type = transport_station.child_value("object_type");
        string name = transport_station.child_value("the_official_name");
        location loc(transport_station.child_value("location"));
        vector<string> routes = parseVectro(transport_station.child_value("routes"));
        coordinates new_coordinates(transport_station.child_value("coordinates"));

        VEHICLE transport;
        if (object_type == "Остановка"){
            if (type_of_vehicle == "Автобус") {
                transport = VEHICLE::BUS;
            }
            else if (type_of_vehicle == "Трамвай") {
                transport = VEHICLE::TRAM;
            }
            else {
                transport = VEHICLE::TROLLEYBUS;
            }
            for (const auto& route : routes) {
                routes_and_coordinates[transport][route].push_back(new_coordinates);
            }
        }

        if (object_type == "Остановка") {
            if (!loc.x.empty()) {
                street_and_stop[loc.x]++;
            }
            if (!loc.y.empty()) {
                street_and_stop[loc.y]++;
            }
        }
    }

    maxStops(routes_and_coordinates);
    maxDist(routes_and_coordinates);
    maxValue(street_and_stop);

    return 0;
}
/*
Маршрут с наибольшим количеством остановок для:
Автобуса - маршрут 211 остановок 123
Трамвая - маршрут 27 остановок 72
Троллейбуса - маршрут 11 остановок 74

автобуса 487 : 1176.83 км
трамвая 6 : 209.046 км
троллейбуса 35 : 188.32 км

Улица с наибольшим количеством остановок : Таллинское ш. - 92 остановок
*/
