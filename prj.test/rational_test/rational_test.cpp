#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <rational/rational.hpp>
#include <sstream>

TEST_CASE("[rational] - ctor default") {
    Rational r0;
    CHECK(r0.num() == 0);
    CHECK(r0.den() == 1);
}

TEST_CASE("[rational] - ctors and normalize") {
    Rational r1(3);
    CHECK(r1.num() == 3);
    CHECK(r1.den() == 1);

    Rational r2(-6, 2);
    CHECK(r2.num() == -3);
    CHECK(r2.den() == 1);

    Rational r3(2, -6);
    CHECK(r3.num() == -1);
    CHECK(r3.den() == 3);

    CHECK_THROWS(Rational(1, 0));
}

TEST_CASE("[rational] - comparison operators") {
    Rational a(1, 2);
    Rational b(2, 4);
    Rational c(3, 4);

    CHECK(a == b);
    CHECK(a != c);
    CHECK(a < c);
    CHECK(a <= c);
    CHECK(c > a);
    CHECK(c >= a);
}

TEST_CASE("[rational] - arithmetic with rationals") {
    Rational a(1, 2);
    Rational b(1, 3);

    CHECK(a + b == Rational(5, 6));
    CHECK(a - b == Rational(1, 6));
    CHECK(a * b == Rational(1, 6));
    CHECK(a / b == Rational(3, 2));
}

TEST_CASE("[rational] - arithmetic with ints (member)") {
    Rational a(1, 2);

    CHECK(a + 1 == Rational(3, 2));
    CHECK(a - 1 == Rational(-1, 2));
    CHECK(a * 2 == Rational(1, 1));
    CHECK(a / 2 == Rational(1, 4));
}

TEST_CASE("[rational] - arithmetic with ints (non-member)") {
    Rational a(1, 2);

    CHECK(1 + a == Rational(3, 2));
    CHECK(1 - a == Rational(1, 2));
    CHECK(2 * a == Rational(1, 1));
    CHECK(1 / a == Rational(2, 1));
}

TEST_CASE("[rational] - compound assignment") {
    Rational a(1, 2);
    Rational b(1, 3);

    a += b;
    CHECK(a == Rational(5, 6));

    a -= b;
    CHECK(a == Rational(1, 2));

    a *= b;
    CHECK(a == Rational(1, 6));

    a /= b;
    CHECK(a == Rational(1, 2));
}

TEST_CASE("[rational] - compound assignment with ints") {
    Rational a(1, 2);

    a += 1;
    CHECK(a == Rational(3, 2));

    a -= 1;
    CHECK(a == Rational(1, 2));

    a *= 4;
    CHECK(a == Rational(2, 1));

    a /= 2;
    CHECK(a == Rational(1, 1));
}

TEST_CASE("[rational] - unary minus") {
    Rational a(1, 2);
    Rational b = -a;
    CHECK(b.num() == -1);
    CHECK(b.den() == 2);
}
TEST_CASE("rational - valid format") {
    std::istringstream iss("6/-4");
    Rational a;

    iss >> a;                              
    CHECK_FALSE(iss.fail());              
    CHECK(a == Rational(-3, 2));            
}

TEST_CASE("rational - zero denominator") {
    std::istringstream iss("3/0");
    Rational a;

    CHECK_THROWS_AS(iss >> a, std::invalid_argument);
}

TEST_CASE("rational - invalid format") {
    std::istringstream iss("3����/4");
    Rational a;

    iss >> a;
    CHECK(iss.fail());
}
TEST_CASE("rational - invalid format") {
    std::istringstream iss("3555778875000/4");
    Rational a;

    iss >> a;
    CHECK(iss.fail());
}


TEST_CASE("rational - INT32_MIN normalization") {

    CHECK_NOTHROW(static_cast<void>(Rational(std::numeric_limits<std::int32_t>::min(), 1)));
}

TEST_CASE("rational - overflow in addition") {

    Rational a(std::numeric_limits<std::int32_t>::max(), 1);
    Rational b(std::numeric_limits<std::int32_t>::max(), 1);
    CHECK_NOTHROW(static_cast<void>(a + b));
}

