#ifndef COMPLEX_H
#define COMPLEX_H

class Complex {
private:
    double real;
    double imag;

public:
    Complex(double r = 0.0, double i = 0.0);

    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex operator*(double scalar) const;

    double abs() const;

    double getReal() const {
      return real;
    }
    double getImag() const {
      return imag;
    }
};

#endif // COMPLEX_H
