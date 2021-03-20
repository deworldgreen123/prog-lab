#include "poly.h"

using namespace std;

Polynomial::Polynomial()=default;

void Polynomial::Check() {
    for (unsigned i = num - 1; i >= 0; i--) {
        if (coefficients[i] != 0)
            break;
        coefficients.pop_back();
    }
    num = coefficients.size();
}

Polynomial::Polynomial(const vector<double>& x){
    num = x.size();
    for (double i : x){
        coefficients.push_back(i);
    }
    this->Check();
}

Polynomial::~Polynomial() {
    coefficients.clear();
    coefficients.shrink_to_fit();
}

Polynomial::Polynomial(const Polynomial &other):
        num(other.num), coefficients(other.coefficients){
}

Polynomial &Polynomial::operator=(const Polynomial &other){
    coefficients = other.coefficients;
    num = other.num;
    return *this;
}

bool Polynomial::operator==(const Polynomial &other) const {
    if (num != other.num || coefficients != other.coefficients)
        return false;
    return true;
}

bool Polynomial::operator!=(const Polynomial &other) const {
    if (num == other.num || coefficients == other.coefficients)
        return true;
    return false;
}

Polynomial Polynomial::operator+() {
    return *this;
}

Polynomial Polynomial::operator-() {
    *this *= -1;
    return *this;
}

Polynomial Polynomial::operator+(const Polynomial &other) const {
    int num_answer = (num > other.num) ? other.num : num;
    vector<double> answer(num_answer);
    for (int i = 0; i < num_answer; i++){
        answer[i] = ((num > i) ? coefficients[i] : 0) + ((other.num > i) ? other.coefficients[i] : 0);
    }
    answer = Check_(answer);
    return Polynomial(answer);
}

Polynomial Polynomial::operator-(const Polynomial &other) {
    *this = -((-*this) + other);
    return *this;
}

Polynomial &Polynomial::operator+=(const Polynomial &other) {
    *this = *this + other;
    return *this;
}

Polynomial &Polynomial::operator-=(const Polynomial &other) {
    *this = *this - other;
    return *this;
}

Polynomial Polynomial::operator*(double k) {
    vector<double> k1{k};
    *this = *this * k1;
    return *this;
}

Polynomial Polynomial::operator*(const Polynomial &other) const {
    int n = num + other.num - 1;
    vector<double> answer(n);
    for (int i = 0; i < num; i++){
        for (int j = 0; j < other.num; j++){
            answer[i + j] += coefficients[i] * other.coefficients[j];
        }
    }
    return Polynomial(answer);
}

Polynomial Polynomial::operator/(double k) {
    *this = *this * (1/k);
    return *this;
}

Polynomial &Polynomial::operator*=(double k) {
    *this = *this * k;
    return *this;
}

Polynomial &Polynomial::operator*=(const Polynomial &other) {
    *this = *this * other;
    return *this;
}

Polynomial &Polynomial::operator/=(double k) {
    *this = *this / k;
    return *this;
}

ostream &operator<<(ostream& out, const Polynomial &other) {
    out << other.num - 1 << endl;
    for (int i = other.num - 1; i >= 0; i--) {
        if (other.coefficients[i] == 0){
            out << "";
        }
        else{
            if (abs(other.coefficients[i]) == 1 && i != 0){
                out << ((other.coefficients[i] == -1) ? "-" : "");
            }
            else{
                if (i == other.num - 1 && other.coefficients[i] < 0)
                    out << "-";
                out << abs(other.coefficients[i]);
            }
            out << ((i == 0) ? "" : ((i == 1) ? "x" : "x^"));
            if (i != 0 && i != 1)
                out << i;
        }
        out << ((i == 0 || other.coefficients[i] == 0) ? "" : ((other.coefficients[i - 1] < 0) ? " - " : " + "));
    }
    out << endl;
    return out;
}

istream &operator>>(istream& in, Polynomial &other) {
    string str_in;
    string str1;
    vector<string> str_monomials;
    getline(in, str_in); //123x + 432x^2 - 435x^3 + 13452
    vector<double> arr_size;

    if (str_in[0] != '-' && str_in[0] != '+')
        str1 += '+';

    for (int i = 0; i < str_in.size(); i++){
        if (str_in[i] == ' ')
            continue;

        if (i != 0 &&(str_in[i] == '+' || str_in[i] == '-')) {
            str_monomials.push_back(str1);
            str1.clear();
        }

        str1 += str_in[i];

        if (i == str_in.size() - 1) {
            str_monomials.push_back(str1);
            str1.clear();
        }
    }

    for (const auto& str_monomial : str_monomials){
        signed sign = (str_monomial[0] == '+')? +1 : -1;
        std::string buff;
        int count = 1;

        for (; count < str_monomial.size() && str_monomial[count] != 'x'; count++)
            buff.push_back(str_monomial[count]);

        int coef = 1;

        if (!buff.empty())
            coef = stoi(buff);

        int power;

        if (count == str_monomial.size())
            power = 0;
        else if (++count == str_monomial.size())
            power = 1;
        else {
            count++;
            buff.clear();
            for (int i = count; i < str_monomial.size(); i++)
                buff.push_back(str_monomial[i]);
            power = stoi(buff);
        }

        if (power >= other.coefficients.size())
            other.coefficients.resize(power + 1, 0);
        other.coefficients[power] += coef * sign;
        other.num = other.coefficients.size();
    }
    other.Check();

    return in;
}

double Polynomial::operator[](int i) const {
    return coefficients[i];
}

double &Polynomial::operator[](int i) {
    return coefficients[i];
}

vector<double> Check_(vector<double> x){
    for (unsigned i = x.size() - 1; i >= 0; i--) {
        if (x[i] != 0)
            break;
        x.pop_back();
    }
    return x;
}