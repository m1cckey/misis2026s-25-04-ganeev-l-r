#ifndef COMPLEX_COMPLEX_HPP
#define COMPLEX_COMPLEX_HPP

#include <iostream>
#include <sstream>
constexpr double EPS = 2 * 1e-9;
struct Complex {
    Complex() {}
    explicit Complex(const double real);
    Complex(const double real, const double imaginary);

    ~Complex() = default;
    Complex& operator=(const Complex&) = default;

    bool operator==(const Complex& rhs) const {
        return std::abs(re - rhs.re) <= EPS &&
            std::abs(im - rhs.im) <= EPS;
    }
    bool operator!=(const Complex& rhs) const { return !operator==(rhs); }

    Complex& operator+=(const Complex& rhs);
    Complex& operator+=(const double rhs);  

    Complex& operator-=(const Complex& rhs);
    Complex& operator-=(const double rhs); 

    Complex& operator*=(const Complex& rhs);
    Complex& operator*=(const double rhs);  

    Complex& operator/=(const Complex& rhs);
    Complex& operator/=(const double rhs); 

    std::ostream& writeTo(std::ostream& ostrm) const;
    std::istream& readFrom(std::istream& istrm);

    double re{ 0.0 };
    double im{ 0.0 };

    Complex operator-() const noexcept;

    static const char leftBrace{ '{' };
    static const char separator{ ',' };
    static const char rightBrace{ '}' };
};

Complex operator+(const Complex& lhs, const Complex& rhs);
Complex operator+(const Complex& lhs, const double rhs);
Complex operator+(const double lhs, const Complex& rhs);

Complex operator-(const Complex& lhs, const Complex& rhs);
Complex operator-(const Complex& lhs, const double rhs);
Complex operator-(const double lhs, const Complex& rhs);

Complex operator*(const Complex& lhs, const Complex& rhs);
Complex operator*(const Complex& lhs, const double rhs);
Complex operator*(const double lhs, const Complex& rhs);

Complex operator/(const Complex& lhs, const Complex& rhs);
Complex operator/(const Complex& lhs, const double rhs);
Complex operator/(const double lhs, const Complex& rhs);

inline std::ostream& operator<<(std::ostream& ostrm, const Complex& rhs)
{
    return rhs.writeTo(ostrm);
}

inline std::istream& operator>>(std::istream& istrm, Complex& rhs)
{
    return rhs.readFrom(istrm);
}

#endif
