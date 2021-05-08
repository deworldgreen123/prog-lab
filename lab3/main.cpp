#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "pugixml.hpp"
#include <unordered_map>

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
    string first_street;
    string second_street;
    location() = default;

    [[maybe_unused]] location(string x_new , string y_new){
        first_street = move(x_new);
        second_street = move(y_new);
    }
    explicit location(const string& str){
        string res;
        for (auto c : str) {
            if (c == ',') {
                first_street = res;
                res.clear();
            }
            else {
                res += c;
            }
        }
        second_street = res;
    }
};



vector<string> vectorParse(const string& str){
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

class Routes_and_coordinates{
private:
    unordered_map<string, vector<coordinates>> routes_and_coordinates[3];
public:
    Routes_and_coordinates() = default;

    void addStop(xml_node transport_station){
        string type_of_vehicle = transport_station.child_value("type_of_vehicle");
        string object_type = transport_station.child_value("object_type");
        string name = transport_station.child_value("the_official_name");
        location loc = location(transport_station.child_value("location"));
        vector<string> routes = vectorParse(transport_station.child_value("routes"));
        coordinates new_coordinates = coordinates(transport_station.child_value("coordinates"));

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
    }

    pair<string, int> maxValueStops(int i){
        int max_v = 0;
        string max_k;
        for (const auto & routes_and_coordinate : routes_and_coordinates[i]) {
            if (routes_and_coordinate.second.size() > max_v) {
                max_v = routes_and_coordinate.second.size();
                max_k = routes_and_coordinate.first;
            }
        }
        return { max_k, max_v };
    }

    void maxStops(){
        auto bus = maxValueStops(BUS);
        auto tram = maxValueStops(TRAM);
        auto trolleybus = maxValueStops(TROLLEYBUS);
        cout << "Маршрут с наибольшим количеством остановок для:" << endl;
        cout << "Автобуса - маршрут " << bus.first << " остановок " << bus.second << endl;
        cout << "Трамвая - маршрут " << tram.first << " остановок " << tram.second << endl;
        cout << "Троллейбуса - маршрут " << trolleybus.first << " остановок " << trolleybus.second << endl << endl;
    }

    static double findDist(coordinates p1, coordinates p2) {
        const double RAD = 3.14159265 / 180;
        const int R = 6371;
        double answer = sin((p2.x-p1.x)/2 * RAD) * sin((p2.x-p1.x)/2 * RAD) +
                        cos(p1.x * RAD) * cos(p2.x * RAD) * sin((p2.y-p1.y)/2 * RAD) * sin((p2.y-p1.y)/2 * RAD);
        return R * 2 * asin(sqrt(answer));
    }

    pair<string, double> oneMaxDist(int i){
        vector <double> max_distances_for_routes;
        double max_dist_route = 0;
        string max_route;
        for (const auto & routes_and_coordinate : routes_and_coordinates[i]) {
            string route = routes_and_coordinate.first;
            vector <coordinates> coordinates = routes_and_coordinate.second;
            double max_dist = 0;
            for (unsigned j = 0; j < coordinates.size() - 1; ++j) {
                max_dist += findDist(coordinates[j], coordinates[j + 1]);
            }
            if (max_dist > max_dist_route) {
                max_dist_route = max_dist;
                max_route = route;
            }
        }
        return {max_route, max_dist_route};
    }

    void maxDist(){
        auto bus = oneMaxDist(BUS);
        auto tram = oneMaxDist(TRAM);
        auto trolleybus = oneMaxDist(TROLLEYBUS);
        cout << "автобус " << bus.first << " " << bus.second << endl;
        cout << "трамвай " << tram.first << " " << tram.second  << endl;
        cout << "троллейбус " << trolleybus.first << " " << trolleybus.second  << endl;
    }
};

class Street_and_stop{
private:
    unordered_map<string, int> street_and_stop;
public:
    Street_and_stop() = default;

    void addStop(xml_node transport_station){
        string object_type = transport_station.child_value("object_type");
        location loc(transport_station.child_value("location"));

        if (object_type == "Остановка") {
            if (!loc.first_street.empty()) {
                street_and_stop[loc.first_street]++;
            }
            if (!loc.second_street.empty()) {
                street_and_stop[loc.second_street]++;
            }
        }
    }
    void maxValue() {
        int max_v = 0;
        string max_k;
        for (auto &it : street_and_stop) {
            if (it.second > max_v) {
                max_v = it.second;
                max_k = it.first;
            }
        }
        cout << "Улица с наибольшим количеством остановок : " << max_k << " - " << max_v << " остановок\n";
    }
};

int main() {
    system("chcp 65001");

    xml_document doc;
    doc.load_file("lab3_spb.xml");
    xml_node station = doc.child("dataset");

    Routes_and_coordinates routes_and_coordinates;
    Street_and_stop street_and_stop;

    for (xml_node transport_station = station.child("transport_station"); transport_station; transport_station = transport_station.next_sibling()){
        routes_and_coordinates.addStop(transport_station);
        street_and_stop.addStop(transport_station);
    }

    routes_and_coordinates.maxStops();
    routes_and_coordinates.maxDist();
    street_and_stop.maxValue();

    return 0;
}
