#ifndef LAB2_POLY_H
#define LAB2_POLY_H
#include "vector"
#include <iostream>
#include <string>
#include <exception>

using namespace std;

class Polynomial{
protected:
    vector<double> coefficients;

    void Check();
public:
    Polynomial();

    Polynomial(const vector<double>&);

    ~Polynomial();

    Polynomial(const Polynomial &);

    Polynomial &operator=(const Polynomial &);

    bool operator==(const Polynomial &)const;

    bool operator!=(const Polynomial &)const;

    Polynomial operator+() const;

    Polynomial operator-() const;

    Polynomial operator+(const Polynomial &) const;

    Polynomial operator-(const Polynomial &) const;

    Polynomial &operator+=(const Polynomial &);

    Polynomial &operator-=(const Polynomial &);

    Polynomial operator*(double) const;

    Polynomial operator*(const Polynomial &) const;

    Polynomial operator/(double) const;

    Polynomial operator*=(double) const;

    Polynomial &operator*=(const Polynomial &);

    Polynomial operator/=(double) const;

    friend ostream &operator<<(ostream &out, const Polynomial &other);

    friend istream &operator>>(istream &in, Polynomial &other);

    friend istream &operator>>(istream &in, Polynomial &other);

    double operator[](int) const;

    double &operator[](int);

    [[nodiscard]] int num() const;
};

#endif //LAB2_POLY_H

