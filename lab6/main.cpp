#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <random>

enum COLOR {
    WHITE,   // Top
    YELLOW,    // Bottom
    GREEN, // Front
    BLUE,  // Back
    ORANGE, // Left
    RED,  // Right
    ZERO
};

enum POS {
    TOP,
    BOTTOM,
    FRONT,
    BACK,
    LEFT,
    RIGHT
};

enum AXIS {
    X,
    Y,
    Z
};

template<typename T>
void swap(T& x1, T& x2, T& x3, T& x4){
    auto tmp = x1;
    x1 = x2;
    x2 = x3;
    x3 = x4;
    x4 = tmp;
}

class SmallCube{
private:
    std::vector<COLOR> color;

    void rotateAxis(AXIS axis){
        switch (axis) {
            case AXIS::X: {
                swap(color[POS::TOP], color[POS::LEFT], color[POS::BOTTOM], color[POS::RIGHT]);
                break;
            }
            case AXIS::Y: {
                swap(color[POS::FRONT], color[POS::TOP], color[POS::BACK], color[POS::BOTTOM]);
                break;
            }
            case AXIS::Z: {
                swap(color[POS::BACK], color[POS::LEFT], color[POS::FRONT], color[POS::RIGHT]);
                break;
            }
        }
    }

public:
    explicit SmallCube(){
        color.resize(6, COLOR::ZERO); // верх, низ, впереди, сзади, лево, право
    }

    SmallCube &operator=(const SmallCube& other){
        color = other.color;
        return *this;
    };

    void rotate(AXIS axis, unsigned int sign){
        for (unsigned int i = 0; i < sign; i++)
            rotateAxis(axis);
    }

    void set(int i, COLOR _color)
    {
        color[i] = _color;
    }

    COLOR operator[](size_t i) const {
        return color[i];
    }

    COLOR &operator[](size_t i) {
        return color[i];
    }
};

COLOR parse(char it){
    switch (it) {
        case 'B': {
            return COLOR::BLUE;
        }
        case 'R': {
            return COLOR::RED;
        }
        case 'Y': {
            return COLOR::YELLOW;
        }
        case 'G': {
            return COLOR::GREEN;
        }
        case 'O': {
            return COLOR::ORANGE;
        }
        case 'W': {
            return COLOR::WHITE;
        }
        default:{
            throw(std::invalid_argument("YOUR_ERROR"));
        }
    }
}

char parse(COLOR it){
    switch (it) {
        case COLOR::BLUE: {
            return 'B';
        }
        case COLOR::RED: {
            return 'R';
        }
        case COLOR::YELLOW: {
            return 'Y';
        }
        case COLOR::GREEN: {
            return 'G';
        }
        case COLOR::ORANGE: {
            return 'O';
        }
        case COLOR::WHITE: {
            return 'W';
        }
        case ZERO:
            break;
    }
    return ZERO;
}

class Cube{
private:
    SmallCube cube[3][3][3];
    std::vector<COLOR> color;
    std::string com;
public:
    Cube &operator=(const Cube& other){
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                for (int k = 0; k < 3; k++){
                    cube[i][j][k] = other.cube[i][j][k];
                }
            }
        }
        color = other.color;
        return *this;
    }

    explicit Cube(std::vector<COLOR> color_new = {  COLOR::WHITE,   // Top
                                                    COLOR::YELLOW,    // Bottom
                                                    COLOR::GREEN, // Front
                                                    COLOR::BLUE,  // Back
                                                    COLOR::ORANGE, // Left
                                                    COLOR::RED,  // Right
    }): color(std::move(color_new))
    {
        int i, j;

        for(i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                cube[0][i][j].set(POS::TOP, color[0]);
                cube[2][i][j].set(POS::BOTTOM, color[1]);
                cube[i][2][j].set(POS::FRONT, color[2]);
                cube[i][0][j].set(POS::BACK, color[3]);
                cube[i][j][0].set(POS::LEFT, color[4]);
                cube[i][j][2].set(POS::RIGHT, color[5]);
            }
        }
    }

    void rotateSign(POS pos, unsigned int sign = 1){
        int i, j, k;
        if (pos == POS::TOP || pos == POS::BOTTOM){
            i = (pos == POS::TOP) ? 0 : 2;

            for (k = 0; k < sign; k++){
                swap(cube[i][0][0], cube[i][2][0], cube[i][2][2], cube[i][0][2]);
                swap(cube[i][0][1], cube[i][1][0], cube[i][2][1], cube[i][1][2]);
            }

            for (j = 0; j < 3; j++){
                for (k = 0; k < 3; k++){
                    cube[i][j][k].rotate(AXIS::Z, sign);
                }
            }
        } else if (pos == POS::FRONT || pos == POS::BACK){
            j = (pos == POS::FRONT) ? 2 : 0;

            for (k = 0; k < sign; k++){
                swap(cube[0][j][0], cube[2][j][0], cube[2][j][2], cube[0][j][2]);
                swap(cube[0][j][1], cube[1][j][0], cube[2][j][1], cube[1][j][2]);
            }

            for (i = 0; i < 3; i++){
                for (k = 0; k < 3; k++){
                    cube[i][j][k].rotate(AXIS::X, sign);
                }
            }
        } else {
            k = (pos == POS::LEFT) ? 0 : 2;

            for (i = 0; i < sign; i++){
                swap(cube[0][0][k], cube[2][0][k], cube[2][2][k], cube[0][2][k]);
                swap(cube[0][1][k], cube[1][0][k], cube[2][1][k], cube[1][2][k]);
            }

            for (i = 0; i < 3; i++){
                for (j = 0; j < 3; j++){
                    cube[i][j][k].rotate(AXIS::Y, sign);
                }
            }
        }
    }

    void rotateCube(AXIS axis, unsigned int sign = 1){
        int i, j, k;
        switch (axis){
            case X: {
                com += "X" + std::to_string(sign) + " ";
                rotateSign(FRONT, sign);
                rotateSign(BACK, sign);

                j = 1;

                for (k = 0; k < sign; k++){
                    swap(cube[0][j][0], cube[2][j][0], cube[2][j][2], cube[0][j][2]);
                    swap(cube[0][j][1], cube[1][j][0], cube[2][j][1], cube[1][j][2]);

                    swap(color[POS::TOP], color[POS::LEFT], color[POS::BOTTOM], color[POS::RIGHT]);
                }

                for (i = 0; i < 3; i++){
                    for (k = 0; k < 3; k++){
                        cube[i][j][k].rotate(AXIS::X, sign);
                    }
                }
                break;
            }
            case Y: {
                com += "Y" + std::to_string(sign) + " ";
                rotateSign(LEFT, sign);
                rotateSign(RIGHT, sign);

                k = 1;

                for (i = 0; i < sign; i++){
                    swap(cube[0][0][k], cube[2][0][k], cube[2][2][k], cube[0][2][k]);
                    swap(cube[0][1][k], cube[1][0][k], cube[2][1][k], cube[1][2][k]);

                    swap(color[POS::FRONT], color[POS::TOP], color[POS::BACK], color[POS::BOTTOM]);
                }

                for (i = 0; i < 3; i++){
                    for (j = 0; j < 3; j++){
                        cube[i][j][k].rotate(AXIS::Y, sign);
                    }
                }
                break;
            }
            case Z: {
                com += "Z" + std::to_string(sign) + " ";
                rotateSign(TOP, sign);
                rotateSign(BOTTOM, sign);

                i = 1;

                for (k = 0; k < sign; k++){
                    swap(cube[i][0][0], cube[i][2][0], cube[i][2][2], cube[i][0][2]);
                    swap(cube[i][0][1], cube[i][1][0], cube[i][2][1], cube[i][1][2]);

                    swap(color[POS::BACK], color[POS::LEFT], color[POS::FRONT], color[POS::RIGHT]);
                }

                for (j = 0; j < 3; j++){
                    for (k = 0; k < 3; k++){
                        cube[i][j][k].rotate(AXIS::Z, sign);
                    }
                }
                break;
            }
        }
    }

    void check(){
        int check[3][3][3];
        int i, j, k;
        for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++)
                for (k = 0; k < 3; k++)
                    check[i][j][k] = 0;

        for (i = 0; i < 6; i++){
            auto c = findCentre(i);
            if (c[0] == -1)
                throw(std::invalid_argument("YOUR_ERROR: it's no Rubik's Cube"));
            check[c[0]][c[1]][c[2]]++;
            for (j = (i/2)*2 + 2; j < 6; j++){
                c = findEdge(i, j);
                if (c[0] == -1)
                    throw(std::invalid_argument("YOUR_ERROR: it's no Rubik's Cube"));
                check[c[0]][c[1]][c[2]]++;
                for (k = (j/4)*2 + 4; k < 6; k++){
                    c = findAngle(i, j, k);
                    if (c[0] == -1)
                        throw(std::invalid_argument("YOUR_ERROR: it's no Rubik's Cube"));
                    check[c[0]][c[1]][c[2]]++;
                }
            }
        }
        for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++)
                for (k = 0; k < 3; k++)
                    if (i != 1 && j != 1 && k != 1)
                        if (check[i][j][k] != 1)
                            throw(std::invalid_argument("YOUR_ERROR: it's no Rubik's Cube"));

    }

    void randomCube(){
        std::vector<POS> pos{TOP,
                             BOTTOM,
                             FRONT,
                             BACK,
                             LEFT,
                             RIGHT};

        std::vector<AXIS> axis{X,
                               Y,
                               Z};

        std::random_device rd;
        std::mt19937 mersenne(rd());
        unsigned int a = 30;
        for (int t = 0; t < 8; t++)
            a += mersenne()%10;

        for (unsigned int k = 0; k < a; k++){
            rotateSign(pos[mersenne() % 6], mersenne() % 4);
            // rotateCube(axis[mersenne() % 3], mersenne() % 4);
        }
        check();
    }

    void rotateString(std::string str){
        std::string commands;
        com += str + " ";
        for (int i = 0; i < str.size() - 1; i++){
            if (str[i] == ' ')
                continue;
            if (str[i] == 'U'){
                if (str[i + 1] == '2') {
                    rotateSign(TOP, 2);
                }
                else if (str[i + 1] == '3') {
                    rotateSign(TOP, 3);
                }
                else {
                    rotateSign(TOP, 1);
                }
            } else if (str[i] == 'D'){
                if (str[i + 1] == '2') {
                    rotateSign(BOTTOM, 2);
                }
                else if (str[i + 1] == '3') {
                    rotateSign(BOTTOM, 1);
                }
                else {
                    rotateSign(BOTTOM, 3);
                }
            } else if (str[i] == 'F'){
                if (str[i + 1] == '2') {
                    rotateSign(FRONT, 2);
                }
                else if (str[i + 1] == '3') {
                    rotateSign(FRONT, 3);
                }
                else {
                    rotateSign(FRONT, 1);
                }
            } else if (str[i] == 'B'){
                if (str[i + 1] == '2') {
                    rotateSign(BACK, 2);
                }
                else if (str[i + 1] == '3') {
                    rotateSign(BACK, 1);
                }
                else {
                    rotateSign(BACK, 3);
                }
            } else if (str[i] == 'L'){
                if (str[i + 1] == '2') {
                    rotateSign(LEFT, 2);
                }
                else if (str[i + 1] == '3') {
                    rotateSign(LEFT, 3);
                }
                else {
                    rotateSign(LEFT, 1);
                }
            } else if (str[i] == 'R'){
                if (str[i + 1] == '2') {
                    rotateSign(RIGHT, 2);
                }
                else if (str[i + 1] == '3') {
                    rotateSign(RIGHT, 1);
                }
                else {
                    rotateSign(RIGHT, 3);
                }
            }
        }
        if (str.back() == 'U'){
            rotateSign(TOP, 1);
        }
        else if (str.back() == 'D'){
            rotateSign(BOTTOM, 3);
        }
        else if (str.back() == 'F'){
            rotateSign(FRONT, 1);
        }
        else if (str.back() == 'B'){
            rotateSign(BACK, 3);
        }
        else if (str.back() == 'L'){
            rotateSign(LEFT, 1);
        }
        else if (str.back() == 'R'){
            rotateSign(RIGHT, 3);
        }
    }

    void fileRead(const std::string& txt){
        std::ifstream in(txt);
        char it;
        int i, j;
        color.resize(6);

        for (i = 0; i < 3; i++){
            for (j = 0; j < 3 ; j++){
                in >> it;
                cube[0][i][j].set(POS::TOP, parse(it));
                if (i == 1 && j == 1){
                    color[POS::TOP] = cube[0][1][1][POS::TOP];
                }
            }
        }
        for (i = 0; i < 3; i++){
            for (j = 0; j < 3 ; j++){
                in >> it;
                cube[i][j][0].set(POS::LEFT, parse(it));
                if (i == 1 && j == 1){
                    color[POS::LEFT] = cube[1][1][0][POS::LEFT];
                }
            }
            for (j = 0; j < 3 ; j++){
                in >> it;
                cube[i][2][j].set(POS::FRONT, parse(it));
                if (i == 1 && j == 1){
                    color[POS::FRONT] = cube[1][2][1][POS::FRONT];
                }
            }
            for (j = 0; j < 3 ; j++){
                in >> it;
                cube[i][2 - j][2].set(POS::RIGHT, parse(it));
                if (i == 1 && j == 1){
                    color[POS::RIGHT] = cube[1][1][2][POS::RIGHT];
                }
            }
            for (j = 0; j < 3 ; j++){
                in >> it;
                cube[i][0][2 - j].set(POS::BACK, parse(it));
                if (i == 1 && j == 1){
                    color[POS::BACK] = cube[1][0][1][POS::BACK];
                }
            }
        }
        for (i = 0; i < 3; i++){
            for (j = 0; j < 3 ; j++){
                in >> it;
                cube[2][2 - i][j].set(POS::BOTTOM, parse(it));
                if (i == 1 && j == 1){
                    color[POS::BOTTOM] = cube[2][1][1][POS::BOTTOM];
                }
            }
        }

        check();
    }

    void fileWrite(const std::string& txt) const{
        std::ofstream out(txt);
        out << *this;
    }

    friend std::ostream &operator<<(std::ostream &out, const Cube &other);

    void solved() {
        sol();
        std::cout << com << "\n";
        com.clear();
    }

private:
    void sol(){
        std::vector<int> c;
        while (!checkCross())
            cross();

        while (!checkAngle()) {
            angle();
        }
        rotateCube(X, 2);

        while (!checkEdge()) {
            edge();
        }

        while (!checkCrossDown()){
            F2U();
            FRUR3U3F3();
            if (cube[0][0][1][TOP] == color[TOP] && cube[0][2][1][TOP] == color[TOP])
                rotateCube(Z, 1);
        }

        while (!checkF2L()){
            rotateString("U");
        }

        while (!checkCross()){
            rotateCube(Z, 1);
            if (cube[0][2][1][FRONT] == color[FRONT] && cube[0][0][1][BACK] == color[BACK]) {
                RUR3URU2R3U();
                rotateString("U3");
                rotateCube(Z, 3);
                RUR3URU2R3U();
                break;
            }
            if (cube[0][2][1][FRONT] != color[FRONT] && cube[0][1][0][LEFT] != color[LEFT]){
                RUR3URU2R3U();
                break;
            }
        }

        while (!checkAngleDown()) {
            URU3L3UR3U3L();
        }

        for (int j = 0; j < 4; j++){

            for (int i = 0; i < 4; i++){
                c = findAngle(TOP, FRONT, RIGHT);
                if (c[0] == 0 && c[1] == 2 && c[2] == 2){
                    break;
                }
                rotateCube(Z, 1);
            }

            c = findAngle(TOP, FRONT, LEFT);
            if (c[0] == 0 && c[1] == 0 && c[2] == 2){
                URU3L3UR3U3L();
                URU3L3UR3U3L();
            }
            else if (c[0] == 0 && c[1] == 0 && c[2] == 0){
                URU3L3UR3U3L();
            }
        }

        for (int i = 0; i < 4; i++){
            int k = ((i == 0) ? RIGHT : ((i == 1) ? BACK : ((i == 2) ? LEFT : FRONT)));
            while (cube[0][2][2][TOP] != color[TOP] || cube[0][2][2][RIGHT] != color[k])
                R3D3RD();
            rotateString("U");
        }
        rotateCube(X, 2);

        if (!checkCube())
            sol();
    }

    void F2U() {
        if (cube[0][1][0][TOP] == color[TOP] && cube[0][2][1][TOP] == color[TOP] &&
            cube[0][0][1][TOP] != color[TOP] && cube[0][1][2][TOP] != color[TOP]) {
            rotateString("U");
        }
        if (cube[0][1][0][TOP] != color[TOP] && cube[0][2][1][TOP] == color[TOP] &&
            cube[0][1][2][TOP] == color[TOP] && cube[0][0][1][TOP] != color[TOP]) {
            rotateString("U2");
        }
        if (cube[0][1][0][TOP] != color[TOP] && cube[0][2][1][TOP] != color[TOP] &&
            cube[0][1][2][TOP] == color[TOP] && cube[0][0][1][TOP] == color[TOP]) {
            rotateString("U3");
        }
        if (cube[0][0][1][TOP] == color[TOP] && cube[0][2][1][TOP] == color[TOP]) {
            rotateString("U");
        }
    }

    void cross(){
        std::vector<int> c;
        for (int i = 0; i < 4; i++){
            c = findEdge(TOP, RIGHT);
            if (!(c[0] == 0 && c[1] == 1 && c[2] == 2))
                solveCross(c);
            rotateCube(Z, 1);
        }
        for (int i = 0; i < 4; i++){
            if (cube[0][2][1][TOP] != color[TOP]){
                FU3RU();
            }
            rotateCube(Z, 1);
        }
    }

    void angle(){
        std::vector<int> c;
        for (int i = 0; i < 4; i++){
            c = findAngle(TOP, FRONT,RIGHT);
            if (!(c[0] == 0 && c[1] == 2 && c[2] == 2))
                solveAngle(c);
            rotateCube(Z, 1);
        }
        for (int i = 0; i < 4; i++){
            while (cube[0][2][2][TOP] != color[TOP] && cube[0][2][2][RIGHT] != color[RIGHT]){
                R3D3RD();
            }
            rotateCube(Z, 1);
        }
    }

    void edge(){
        std::vector<int> c;
        for (int i = 0; i < 4; i++){
            c = findEdge(FRONT, RIGHT);
            if (!(c[0] == 1 && c[1] == 2 && c[2] == 2)){
                solveEdge(c);
            }
            else if (cube[1][2][2][FRONT] == color[RIGHT] && cube[1][2][2][RIGHT] == color[FRONT]){
                URU3R3U3F3UF();
                rotateString("U2");
                URU3R3U3F3UF();
            }
            rotateCube(Z, 1);
        }
    }

    void solveCross(std::vector<int> c) {
        if (c[0] == 0){
            if (c[1] == 0){
                rotateString("B2");
            }
            else if (c[1] == 1){
                if (c[2] == 0){
                    rotateString("L2");
                }
                else {
                    rotateString("R2");
                }
            }
            else {
                rotateString("F2");
            }
        }

        if (c[0] == 1){
            if (c[1] == 0 && c[2] == 0) {
                rotateString("B");
            }
            if (c[1] == 0 && c[2] == 2){
                rotateString("R");
            }
            if (c[1] == 2 && c[2] == 0){
                rotateString("L");
            }
            if (c[1] == 2 && c[2] == 2){
                rotateString("F");
            }
        }
        c = findEdge(TOP, RIGHT);
        if (c[2] == 0) {
            rotateString("D2");
        }
        else if (c[1] == 0) {
            rotateString("D3");
        }
        else if (c[1] == 2) {
            rotateString("D");
        }
        rotateString("R2");
    }

    void solveAngle(std::vector<int> c) {
        if (c[0] == 0) {
            if (c[1] == 0){
                if (c[2] == 0){
                    rotateCube(Z, 2);
                    R3D3RD();
                    rotateCube(Z, 2);
                }
                else {
                    rotateCube(Z, 1);
                    R3D3RD();
                    rotateCube(Z, 3);
                }
            }
            else {
                rotateCube(Z, 3);
                R3D3RD();
                rotateCube(Z, 1);
            }
        }
        c = findAngle(TOP, FRONT, RIGHT);
        if (c[1] == 0 && c[2] == 0){
            rotateString("D2");
        }
        else if (c[1] == 0 && c[2] == 2){
            rotateString("D3");
        }
        else if (c[1] == 2 && c[2] == 0){
            rotateString("D");
        }
        while (!(c[0] == 0 && c[1] == 2 && c[2] == 2)){
            R3D3RD();
            c = findAngle(TOP, FRONT, RIGHT);
        }
    }

    void solveEdge(std::vector<int> c) {
        if (c[0] == 1) {
            if (c[1] == 0 && c[2] == 0){
                rotateCube(Z, 2);
                URU3R3U3F3UF();
                rotateCube(Z, 2);
            }
            if (c[1] == 0 && c[2] == 2){
                rotateCube(Z, 1);
                URU3R3U3F3UF();
                rotateCube(Z, 3);
            }
            if (c[1] == 2 && c[2] == 0){
                rotateCube(Z, 3);
                URU3R3U3F3UF();
                rotateCube(Z, 1);
            }
        }
        c = findEdge(FRONT, RIGHT);

        if (c[0] == 0){
            if (c[1] == 0 && c[2] == 1){
                rotateString("U2");
            }
            if (c[1] == 1 && c[2] == 0){
                rotateString("U3");
            }
            if (c[1] == 1 && c[2] == 2){
                rotateString("U");
            }
        }
        c = findEdge(FRONT, RIGHT);

        URU3R3U3F3UF();
        if (cube[1][2][2][FRONT] == color[RIGHT] && cube[1][2][2][RIGHT] == color[FRONT]) {
            URU3R3U3F3UF();
            rotateString("U2");
            URU3R3U3F3UF();
        }
    }

    void RUR3URU2R3U(){
        rotateString("R U R3 U R U2 R3 U");
    }

    void URU3L3UR3U3L(){
        rotateString("U R U3 L3 U R3 U3 L");
    }

    void FRUR3U3F3(){
        rotateString("F R U R3 U3 F3");
    }

    void URU3R3U3F3UF(){
        rotateString("U R U3 R3 U3 F3 U F");
    }

    void R3D3RD(){
        rotateString("R3 D3 R D");
    }

    void FU3RU(){
        rotateString("U3 R3 U F3");
    }

    bool checkCrossDown() {
        return  cube[0][0][1][TOP] == color[TOP] &&
                cube[0][1][0][TOP] == color[TOP] &&
                cube[0][1][2][TOP] == color[TOP] &&
                cube[0][2][1][TOP] == color[TOP];
    }

    bool checkCross() {
        return  cube[0][0][1][TOP] == color[TOP] && cube[0][0][1][BACK] == color[BACK] &&
                cube[0][1][0][TOP] == color[TOP] && cube[0][1][0][LEFT] == color[LEFT] &&
                cube[0][1][2][TOP] == color[TOP] && cube[0][1][2][RIGHT] == color[RIGHT] &&
                cube[0][2][1][TOP] == color[TOP] && cube[0][2][1][FRONT] == color[FRONT];
    }

    bool checkAngle() {
        return  cube[0][0][0][TOP] == color[TOP] && cube[0][0][1][TOP] == color[TOP] && cube[0][0][2][TOP] == color[TOP] &&
                cube[0][1][0][TOP] == color[TOP] && cube[0][1][1][TOP] == color[TOP] && cube[0][1][2][TOP] == color[TOP] &&
                cube[0][2][0][TOP] == color[TOP] && cube[0][2][1][TOP] == color[TOP] && cube[0][2][2][TOP] == color[TOP] &&
                cube[0][0][0][BACK] == color[BACK] && cube[0][0][1][BACK] == color[BACK] && cube[0][0][2][BACK] == color[BACK] &&
                cube[0][0][0][LEFT] == color[LEFT] && cube[0][1][0][LEFT] == color[LEFT] && cube[0][2][0][LEFT] == color[LEFT] &&
                cube[0][0][2][RIGHT] == color[RIGHT] && cube[0][1][2][RIGHT] == color[RIGHT] && cube[0][2][2][RIGHT] == color[RIGHT] &&
                cube[0][2][0][FRONT] == color[FRONT] && cube[0][2][1][FRONT] == color[FRONT] && cube[0][2][2][FRONT] == color[FRONT];
    }

    bool checkAngleDown() {
        int a = 0;
        auto c1 = findAngle(TOP, FRONT, RIGHT);
        auto c2 = findAngle(TOP, FRONT, LEFT);
        auto c3 = findAngle(TOP, BACK, LEFT);
        auto c4 = findAngle(TOP, BACK, RIGHT);
        if (c1[1] == 2 && c1[2] == 2)
            a++;
        if (c2[1] == 2 && c2[2] == 0)
            a++;
        if (c3[1] == 0 && c3[2] == 0)
            a++;
        if (c4[1] == 0 && c4[2] == 2)
            a++;

        return a != 0;
    }

    bool checkEdge() {
        return  cube[2][0][0][BOTTOM] == color[BOTTOM] && cube[2][0][1][BOTTOM] == color[BOTTOM] && cube[2][0][2][BOTTOM] == color[BOTTOM] &&
                cube[2][1][0][BOTTOM] == color[BOTTOM] && cube[2][1][1][BOTTOM] == color[BOTTOM] && cube[2][1][2][BOTTOM] == color[BOTTOM] &&
                cube[2][2][0][BOTTOM] == color[BOTTOM] && cube[2][2][1][BOTTOM] == color[BOTTOM] && cube[2][2][2][BOTTOM] == color[BOTTOM] &&
                cube[2][0][0][BACK] == color[BACK] && cube[2][0][1][BACK] == color[BACK] && cube[2][0][2][BACK] == color[BACK] &&
                cube[2][0][0][LEFT] == color[LEFT] && cube[2][1][0][LEFT] == color[LEFT] && cube[2][2][0][LEFT] == color[LEFT] &&
                cube[2][0][2][RIGHT] == color[RIGHT] && cube[2][1][2][RIGHT] == color[RIGHT] && cube[2][2][2][RIGHT] == color[RIGHT] &&
                cube[2][2][0][FRONT] == color[FRONT] && cube[2][2][1][FRONT] == color[FRONT] && cube[2][2][2][FRONT] == color[FRONT] &&
                cube[1][0][0][BACK] == color[BACK] && cube[1][0][0][LEFT] == color[LEFT] &&
                cube[1][0][2][BACK] == color[BACK] && cube[1][0][2][RIGHT] == color[RIGHT] &&
                cube[1][2][0][FRONT] == color[FRONT] && cube[1][2][0][LEFT] == color[LEFT] &&
                cube[1][2][2][FRONT] == color[FRONT] && cube[1][2][2][RIGHT] == color[RIGHT];
    }

    bool checkF2L() {
        int a = 0;
        if (cube[0][0][1][BACK] == color[BACK]){
            a++;
        }
        if (cube[0][1][0][LEFT] == color[LEFT]){
            a++;
        }
        if (cube[0][1][2][RIGHT] == color[RIGHT]){
            a++;
        }
        if (cube[0][2][1][FRONT] == color[FRONT]){
            a++;
        }
        return a > 1;
    }

    bool checkCube() {
        bool c = checkAngle();
        for (int i = 0; i < 4; i++) {
            rotateCube(Z, 1);
            c = c && checkAngle();
        }
        rotateCube(X, 1);
        c = c && checkAngle();
        rotateCube(X, 2);
        c = c && checkAngle();
        return c;
    }

    std::vector<int> findCentre(unsigned color1){
        if (cube[0][1][1][TOP] == color[color1]){
            return {0, 1, 1};
        }
        if (cube[2][1][1][BOTTOM] == color[color1]){
            return {2, 1, 1};
        }
        if (cube[1][1][0][LEFT] == color[color1]){
            return {1, 1, 0};
        }
        if (cube[1][1][2][RIGHT] == color[color1]){
            return {1, 1, 2};
        }
        if (cube[1][2][1][FRONT] == color[color1]){
            return {1, 2, 1};
        }
        if (cube[1][0][1][BACK] == color[color1]){
            return {1, 0, 1};
        }

        return {-1};
    }

    std::vector<int> findEdge(unsigned color1, unsigned color2){
        if ((cube[0][0][1][TOP] == color[color1] && cube[0][0][1][BACK] == color[color2]) ||
            (cube[0][0][1][TOP] == color[color2] && cube[0][0][1][BACK] == color[color1]))
            return {0, 0, 1};
        if ((cube[0][1][0][TOP] == color[color1] && cube[0][1][0][LEFT] == color[color2]) ||
            (cube[0][1][0][TOP] == color[color2] && cube[0][1][0][LEFT] == color[color1]))
            return {0, 1, 0};
        if ((cube[0][1][2][TOP] == color[color1] && cube[0][1][2][RIGHT] == color[color2]) ||
            (cube[0][1][2][TOP] == color[color2] && cube[0][1][2][RIGHT] == color[color1]))
            return {0, 1, 2};
        if ((cube[0][2][1][TOP] == color[color1] && cube[0][2][1][FRONT] == color[color2]) ||
            (cube[0][2][1][TOP] == color[color2] && cube[0][2][1][FRONT] == color[color1]))
            return {0, 2, 1};

        if ((cube[1][0][0][LEFT] == color[color1] && cube[1][0][0][BACK] == color[color2]) ||
            (cube[1][0][0][LEFT] == color[color2] && cube[1][0][0][BACK] == color[color1]))
            return {1, 0, 0};
        if ((cube[1][0][2][RIGHT] == color[color1] && cube[1][0][2][BACK] == color[color2]) ||
            (cube[1][0][2][RIGHT] == color[color2] && cube[1][0][2][BACK] == color[color1]))
            return {1, 0, 2};
        if ((cube[1][2][0][LEFT] == color[color1] && cube[1][2][0][FRONT] == color[color2]) ||
            (cube[1][2][0][LEFT] == color[color2] && cube[1][2][0][FRONT] == color[color1]))
            return {1, 2, 0};
        if ((cube[1][2][2][RIGHT] == color[color1] && cube[1][2][2][FRONT] == color[color2]) ||
            (cube[1][2][2][RIGHT] == color[color2] && cube[1][2][2][FRONT] == color[color1]))
            return {1, 2, 2};

        if ((cube[2][0][1][BOTTOM] == color[color1] && cube[2][0][1][BACK] == color[color2]) ||
            (cube[2][0][1][BOTTOM] == color[color2] && cube[2][0][1][BACK] == color[color1]))
            return {2, 0, 1};
        if ((cube[2][1][0][BOTTOM] == color[color1] && cube[2][1][0][LEFT] == color[color2]) ||
            (cube[2][1][0][BOTTOM] == color[color2] && cube[2][1][0][LEFT] == color[color1]))
            return {2, 1, 0};
        if ((cube[2][1][2][BOTTOM] == color[color1] && cube[2][1][2][RIGHT] == color[color2]) ||
            (cube[2][1][2][BOTTOM] == color[color2] && cube[2][1][2][RIGHT] == color[color1]))
            return {2, 1, 2};
        if ((cube[2][2][1][BOTTOM] == color[color1] && cube[2][2][1][FRONT] == color[color2]) ||
            (cube[2][2][1][BOTTOM] == color[color2] && cube[2][2][1][FRONT] == color[color1]))
            return {2, 2, 1};

        return {-1};
    }

    std::vector<int> findAngle(unsigned color1, unsigned color2, unsigned color3){
        if ((cube[0][0][0][LEFT] == color[color1] && cube[0][0][0][BACK] == color[color2] && cube[0][0][0][TOP] == color[color3]) ||
            (cube[0][0][0][LEFT] == color[color1] && cube[0][0][0][TOP] == color[color2] && cube[0][0][0][BACK] == color[color3]) ||
            (cube[0][0][0][TOP] == color[color1] && cube[0][0][0][LEFT] == color[color2] && cube[0][0][0][BACK] == color[color3]) ||
            (cube[0][0][0][TOP] == color[color1] && cube[0][0][0][BACK] == color[color2] && cube[0][0][0][LEFT] == color[color3]) ||
            (cube[0][0][0][BACK] == color[color1] && cube[0][0][0][TOP] == color[color2] && cube[0][0][0][LEFT] == color[color3]) ||
            (cube[0][0][0][BACK] == color[color1] && cube[0][0][0][LEFT] == color[color2] && cube[0][0][0][TOP] == color[color3]))
            return {0, 0, 0};
        if ((cube[0][0][2][RIGHT] == color[color1] && cube[0][0][2][BACK] == color[color2] && cube[0][0][2][TOP] == color[color3]) ||
            (cube[0][0][2][RIGHT] == color[color1] && cube[0][0][2][TOP] == color[color2] && cube[0][0][2][BACK] == color[color3]) ||
            (cube[0][0][2][TOP] == color[color1] && cube[0][0][2][RIGHT] == color[color2] && cube[0][0][2][BACK] == color[color3]) ||
            (cube[0][0][2][TOP] == color[color1] && cube[0][0][2][BACK] == color[color2] && cube[0][0][2][RIGHT] == color[color3]) ||
            (cube[0][0][2][BACK] == color[color1] && cube[0][0][2][TOP] == color[color2] && cube[0][0][2][RIGHT] == color[color3]) ||
            (cube[0][0][2][BACK] == color[color1] && cube[0][0][2][RIGHT] == color[color2] && cube[0][0][2][TOP] == color[color3]))
            return {0, 0, 2};
        if ((cube[0][2][0][LEFT] == color[color1] && cube[0][2][0][FRONT] == color[color2] && cube[0][2][0][TOP] == color[color3]) ||
            (cube[0][2][0][LEFT] == color[color1] && cube[0][2][0][TOP] == color[color2] && cube[0][2][0][FRONT] == color[color3]) ||
            (cube[0][2][0][TOP] == color[color1] && cube[0][2][0][LEFT] == color[color2] && cube[0][2][0][FRONT] == color[color3]) ||
            (cube[0][2][0][TOP] == color[color1] && cube[0][2][0][FRONT] == color[color2] && cube[0][2][0][LEFT] == color[color3]) ||
            (cube[0][2][0][FRONT] == color[color1] && cube[0][2][0][TOP] == color[color2] && cube[0][2][0][LEFT] == color[color3]) ||
            (cube[0][2][0][FRONT] == color[color1] && cube[0][2][0][LEFT] == color[color2] && cube[0][2][0][TOP] == color[color3]))
            return {0, 2, 0};
        if ((cube[0][2][2][RIGHT] == color[color1] && cube[0][2][2][FRONT] == color[color2] && cube[0][2][2][TOP] == color[color3]) ||
            (cube[0][2][2][RIGHT] == color[color1] && cube[0][2][2][TOP] == color[color2] && cube[0][2][2][FRONT] == color[color3]) ||
            (cube[0][2][2][TOP] == color[color1] && cube[0][2][2][RIGHT] == color[color2] && cube[0][2][2][FRONT] == color[color3]) ||
            (cube[0][2][2][TOP] == color[color1] && cube[0][2][2][FRONT] == color[color2] && cube[0][2][2][RIGHT] == color[color3]) ||
            (cube[0][2][2][FRONT] == color[color1] && cube[0][2][2][TOP] == color[color2] && cube[0][2][2][RIGHT] == color[color3]) ||
            (cube[0][2][2][FRONT] == color[color1] && cube[0][2][2][RIGHT] == color[color2] && cube[0][2][2][TOP] == color[color3]))
            return {0, 2, 2};

        if ((cube[2][0][0][LEFT] == color[color1] && cube[2][0][0][BACK] == color[color2] && cube[2][0][0][BOTTOM] == color[color3]) ||
            (cube[2][0][0][LEFT] == color[color1] && cube[2][0][0][BOTTOM] == color[color2] && cube[2][0][0][BACK] == color[color3]) ||
            (cube[2][0][0][BOTTOM] == color[color1] && cube[2][0][0][LEFT] == color[color2] && cube[2][0][0][BACK] == color[color3]) ||
            (cube[2][0][0][BOTTOM] == color[color1] && cube[2][0][0][BACK] == color[color2] && cube[2][0][0][LEFT] == color[color3]) ||
            (cube[2][0][0][BACK] == color[color1] && cube[2][0][0][BOTTOM] == color[color2] && cube[2][0][0][LEFT] == color[color3]) ||
            (cube[2][0][0][BACK] == color[color1] && cube[2][0][0][LEFT] == color[color2] && cube[2][0][0][BOTTOM] == color[color3]))
            return {2, 0, 0};
        if ((cube[2][0][2][RIGHT] == color[color1] && cube[2][0][2][BACK] == color[color2] && cube[2][0][2][BOTTOM] == color[color3]) ||
            (cube[2][0][2][RIGHT] == color[color1] && cube[2][0][2][BOTTOM] == color[color2] && cube[2][0][2][BACK] == color[color3]) ||
            (cube[2][0][2][BOTTOM] == color[color1] && cube[2][0][2][RIGHT] == color[color2] && cube[2][0][2][BACK] == color[color3]) ||
            (cube[2][0][2][BOTTOM] == color[color1] && cube[2][0][2][BACK] == color[color2] && cube[2][0][2][RIGHT] == color[color3]) ||
            (cube[2][0][2][BACK] == color[color1] && cube[2][0][2][BOTTOM] == color[color2] && cube[2][0][2][RIGHT] == color[color3]) ||
            (cube[2][0][2][BACK] == color[color1] && cube[2][0][2][RIGHT] == color[color2] && cube[2][0][2][BOTTOM] == color[color3]))
            return {2, 0, 2};
        if ((cube[2][2][0][LEFT] == color[color1] && cube[2][2][0][FRONT] == color[color2] && cube[2][2][0][BOTTOM] == color[color3]) ||
            (cube[2][2][0][LEFT] == color[color1] && cube[2][2][0][BOTTOM] == color[color2] && cube[2][2][0][FRONT] == color[color3]) ||
            (cube[2][2][0][BOTTOM] == color[color1] && cube[2][2][0][LEFT] == color[color2] && cube[2][2][0][FRONT] == color[color3]) ||
            (cube[2][2][0][BOTTOM] == color[color1] && cube[2][2][0][FRONT] == color[color2] && cube[2][2][0][LEFT] == color[color3]) ||
            (cube[2][2][0][FRONT] == color[color1] && cube[2][2][0][BOTTOM] == color[color2] && cube[2][2][0][LEFT] == color[color3]) ||
            (cube[2][2][0][FRONT] == color[color1] && cube[2][2][0][LEFT] == color[color2] && cube[2][2][0][BOTTOM] == color[color3]))
            return {2, 2, 0};
        if ((cube[2][2][2][RIGHT] == color[color1] && cube[2][2][2][FRONT] == color[color2] && cube[2][2][2][BOTTOM] == color[color3]) ||
            (cube[2][2][2][RIGHT] == color[color1] && cube[2][2][2][BOTTOM] == color[color2] && cube[2][2][2][FRONT] == color[color3]) ||
            (cube[2][2][2][BOTTOM] == color[color1] && cube[2][2][2][RIGHT] == color[color2] && cube[2][2][2][FRONT] == color[color3]) ||
            (cube[2][2][2][BOTTOM] == color[color1] && cube[2][2][2][FRONT] == color[color2] && cube[2][2][2][RIGHT] == color[color3]) ||
            (cube[2][2][2][FRONT] == color[color1] && cube[2][2][2][BOTTOM] == color[color2] && cube[2][2][2][RIGHT] == color[color3]) ||
            (cube[2][2][2][FRONT] == color[color1] && cube[2][2][2][RIGHT] == color[color2] && cube[2][2][2][BOTTOM] == color[color3]))
            return {2, 2, 2};

        return {-1};
    }
};

std::ostream &operator<<(std::ostream& out, const Cube &other) {
    int i, j;

    for (i = 0; i < 3; i++){
        for (j = 0; j < 3 ; j++){
            out << "  ";
        }
        out << "  ";
        for (j = 0; j < 3 ; j++){
            out << parse(other.cube[0][i][j][POS::TOP]) << " ";
        }
        out << "\n";
    }
    out << "\n";

    for (i = 0; i < 3; i++){
        for (j = 0; j < 3 ; j++){
            out << parse(other.cube[i][j][0][POS::LEFT]) << " ";
        }
        out << "  ";
        for (j = 0; j < 3 ; j++){
            out << parse(other.cube[i][2][j][POS::FRONT]) << " ";
        }
        out << "  ";
        for (j = 0; j < 3 ; j++){
            out << parse(other.cube[i][2 - j][2][POS::RIGHT]) << " ";
        }
        out << "  ";
        for (j = 0; j < 3 ; j++){
            out << parse(other.cube[i][0][2 - j][POS::BACK]) << " ";
        }
        out << "\n";
    }
    out << "\n";

    for (i = 0; i < 3; i++){
        for (j = 0; j < 3 ; j++){
            out << "  ";
        }
        out << "  ";
        for (j = 0; j < 3 ; j++){
            out << parse(other.cube[2][2 - i][j][POS::BOTTOM]) << " ";
        }
        out << "\n";
    }
    return out;
}

int main() {
    Cube f;
    //f.fileRead("cube.in");
    f.randomCube();
    f.solved();

    std::cout << f;

    return 0;
}
