#include "complex.hpp"
#include <stdexcept>


Complex::Complex(const double real)
    : re(real)
    , im(0.0)
{
}


Complex::Complex(const double real, const double imaginary)
    : re(real)
    , im(imaginary)
{
}


Complex& Complex::operator+=(const Complex& rhs) {
    re += rhs.re;
    im += rhs.im;
    return *this;
}

Complex& Complex::operator+=(const double rhs) {
    re += rhs;
    return *this;
}

Complex& Complex::operator-=(const Complex& rhs) {
    re -= rhs.re;
    im -= rhs.im;
    return *this;
}

Complex& Complex::operator-=(const double rhs) {
    re -= rhs;
    return *this;
}

Complex& Complex::operator*=(const Complex& rhs) {
    const double oldRe = re;
    const double oldIm = im;
    re = oldRe * rhs.re - oldIm * rhs.im;
    im = oldRe * rhs.im + oldIm * rhs.re;
    return *this;
}

Complex& Complex::operator*=(const double rhs) {
    re *= rhs;
    im *= rhs;
    return *this;
}


Complex& Complex::operator/=(const Complex& rhs) {
    const double denominator = rhs.re * rhs.re + rhs.im * rhs.im;
    if (denominator == 0.0) {
        throw std::runtime_error("division by zero");
    }
    const double oldRe = re;
    const double oldIm = im;
    re = (oldRe * rhs.re + oldIm * rhs.im) / denominator;
    im = (oldIm * rhs.re - oldRe * rhs.im) / denominator;
    return *this;
}

Complex& Complex::operator/=(const double rhs) {
    if (rhs == 0.0) {
        throw std::runtime_error("division by zero");
    }
    re /= rhs;
    im /= rhs;
    return *this;
}

std::ostream& Complex::writeTo(std::ostream& ostrm) const {
    ostrm << leftBrace << re << separator << im << rightBrace;
    return ostrm;
}

std::istream& Complex::readFrom(std::istream& istrm) {
    std::istream::sentry sentry(istrm);
    if (!sentry) {
        return istrm;
    }

    char lb = 0;
    char rb = 0;
    char comma = 0;
    double r = 0.0;
    double i = 0.0;

    if (!(istrm >> lb) || lb != leftBrace) {
        istrm.setstate(std::ios_base::failbit);
    }
    else if (!(istrm >> r)) {
        istrm.setstate(std::ios_base::failbit);
    }
    else if (!(istrm >> comma) || comma != separator) {
        istrm.setstate(std::ios_base::failbit);
    }
    else if (!(istrm >> i)) {
        istrm.setstate(std::ios_base::failbit);
    }
    else if (!(istrm >> rb) || rb != rightBrace) {
        istrm.setstate(std::ios_base::failbit);
    }
    else {
        re = r;
        im = i;
    }

    return istrm;
}

// ������� �����
Complex Complex::operator-() const noexcept {
    return Complex(-re, -im);
}

// �������� ���������

Complex operator+(const Complex& lhs, const Complex& rhs) {
    Complex res(lhs);
    res += rhs;
    return res;
}

Complex operator+(const Complex& lhs, const double rhs) {
    Complex res(lhs);
    res += rhs;
    return res;
}

Complex operator+(const double lhs, const Complex& rhs) {
    Complex res(lhs);
    res += rhs;
    return res;
}

Complex operator-(const Complex& lhs, const Complex& rhs) {
    Complex res(lhs);
    res -= rhs;
    return res;
}

Complex operator-(const Complex& lhs, const double rhs) {
    Complex res(lhs);
    res -= rhs;
    return res;
}

Complex operator-(const double lhs, const Complex& rhs) {
    Complex res(lhs);
    res -= rhs;
    return res;
}

Complex operator*(const Complex& lhs, const Complex& rhs) {
    Complex res(lhs);
    res *= rhs;
    return res;
}

Complex operator*(const Complex& lhs, const double rhs) {
    Complex res(lhs);
    res *= rhs;
    return res;
}

Complex operator*(const double lhs, const Complex& rhs) {
    Complex res(rhs);
    res *= lhs;
    return res;
}

Complex operator/(const Complex& lhs, const Complex& rhs) {
    Complex res(lhs);
    res /= rhs;
    return res;
}

Complex operator/(const Complex& lhs, const double rhs) {
    Complex res(lhs);
    res /= rhs;
    return res;
}

Complex operator/(const double lhs, const Complex& rhs) {
    Complex res(lhs);
    res /= rhs;
    return res;
}
