#ifndef LAB2_POLY_H
#define LAB2_POLY_H
#include "vector"
#include <iostream>
#include <string>
#include <exception>

using namespace std;

class Polynomial{
protected:
    mutable vector<double> coefficients;
    mutable int num = 0;

    void Check();
public:
    Polynomial();

    Polynomial(const vector<double>&);

    ~Polynomial();

    Polynomial(const Polynomial &);

    Polynomial &operator=(const Polynomial &);

    bool operator==(const Polynomial &)const;

    bool operator!=(const Polynomial &)const;

    Polynomial operator+();

    Polynomial operator-();

    Polynomial operator+(const Polynomial &) const;

    Polynomial operator-(const Polynomial &);

    Polynomial &operator+=(const Polynomial &);

    Polynomial &operator-=(const Polynomial &);

    Polynomial operator*(double);

    Polynomial operator*(const Polynomial &) const;

    Polynomial operator/(double);

    Polynomial &operator*=(double);

    Polynomial &operator*=(const Polynomial &);

    Polynomial &operator/=(double);

    friend ostream &operator<<(ostream &out, const Polynomial &other);

    friend istream &operator>>(istream &in, Polynomial &other);

    double operator[](int) const;

    double &operator[](int);
};

vector<double> Check_(vector<double>);

#endif //LAB2_POLY_H
