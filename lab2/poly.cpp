#include "poly.h"

using namespace std;

Polynomial::Polynomial()=default;

void Polynomial::Check() {
    for (unsigned i = num() - 1; i >= 0; i--) {
        if (coefficients[i] != 0)
            break;
        coefficients.pop_back();
    }
}

Polynomial::Polynomial(const vector<double>& x){
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
        coefficients(other.coefficients){
}

Polynomial &Polynomial::operator=(const Polynomial &other){
    coefficients = other.coefficients;
    return *this;
}

bool Polynomial::operator==(const Polynomial &other) const {
    if (num() != other.num() || coefficients != other.coefficients)
        return false;
    return true;
}

bool Polynomial::operator!=(const Polynomial &other) const {
    if (num() == other.num() || coefficients == other.coefficients)
        return true;
    return false;
}

Polynomial Polynomial::operator+() const {
    return *this;
}

Polynomial Polynomial::operator-() const {
    return *this * -1;
}

Polynomial Polynomial::operator+(const Polynomial &other) const {
    Polynomial Copy = *this;
    Copy += other;
    return Copy;
}

Polynomial Polynomial::operator-(const Polynomial &other) const {
    Polynomial Copy = *this;
    Copy -= other;
    return Copy;
}

Polynomial &Polynomial::operator+=(const Polynomial &other) {
    for (int i = 0; i < num(); i++){
        coefficients[i] += ((other.num() > i) ? other.coefficients[i] : 0);
    }
    for (int i = num(); i < other.num(); i++){
        coefficients.push_back(other.coefficients[i]);
    }
    Check();
    return *this;
}

Polynomial &Polynomial::operator-=(const Polynomial &other) {
    for (int i = 0; i < num(); i++){
        coefficients[i] -= ((other.num() > i) ? other.coefficients[i] : 0);
    }
    for (int i = num(); i < other.num(); i++){
        coefficients.push_back(-other.coefficients[i]);
    }
    Check();
    return *this;
}

Polynomial Polynomial::operator*(double k) const {
    Polynomial k1({k});
    return *this * k1;
}

Polynomial Polynomial::operator*(const Polynomial &other) const {
    Polynomial Copy = *this;
    Copy *= other;
    return Copy;
}

Polynomial Polynomial::operator/(double k) const {
    return *this * (1/k);
}

Polynomial Polynomial::operator*=(double k) const {
    return *this * (k);
}

Polynomial &Polynomial::operator*=(const Polynomial &other) {
    int n = num() + other.num() - 1;
    vector<double> answer(n);
    for (int i = 0; i < num(); i++){
        for (int j = 0; j < other.num(); j++) {
            answer[i+j] += coefficients[i] * other.coefficients[j];
        }
    }
    *this = Polynomial(answer);
    return *this;
}

Polynomial Polynomial::operator/=(double k) const {
    return *this / k;
}

ostream &operator<<(ostream& out, const Polynomial &other) {
    out << other.num() - 1 << endl;
    for (int i = other.num() - 1; i >= 0; i--) {
        if (other.coefficients[i] == 0){
            out << "";
        }
        else{
            out << ((other.coefficients[i] < 0) ? "- " : (i != other.num() - 1)? "+ " : "");
            if (abs(other.coefficients[i]) != 1 || i == 0) {
                out << abs(other.coefficients[i]);
            }
            if (i == 0) {
                continue;
            }
            out << "x" << ((i != 1) ? "^" : " ");
            if (i != 1)
                out << abs(i) << " ";
        }
    }
    out << endl;
    for (auto i : other.coefficients){
        cout << i << " ";
    }
    return out;
}

istream &operator>>(istream& in, Polynomial &other) {
    other.coefficients.clear();
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

int Polynomial::num() const{
    return coefficients.size();
}
