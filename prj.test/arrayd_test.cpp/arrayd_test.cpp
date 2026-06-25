#ifdef _MSC_VER
#endif

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <arrayd/arrayd.hpp>


TEST_CASE("ArrayD ctor(size) ok") {
    ArrayD a(3);
    CHECK(a.size() == 3);
}

TEST_CASE("ArrayD ctor(size) throws on non-positive size") {
    CHECK_THROWS_AS(ArrayD(0), std::invalid_argument);
    CHECK_THROWS_AS(ArrayD(-5), std::invalid_argument);
}


TEST_CASE("ArrayD copy ctor copies data") {
    ArrayD a(2);
    a[0] = 1.0f;
    a[1] = 2.0f;

    ArrayD b(a);
    CHECK(b.size() == 2);
    CHECK(b[0] == doctest::Approx(1.0f));
    CHECK(b[1] == doctest::Approx(2.0f));
    
}


TEST_CASE("ArrayD assignment copies data") {
    ArrayD a(3);
    a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
    ArrayD b(1);
    b = a;
    CHECK(b.size() == 3);
    CHECK(b[0] == doctest::Approx(1.0f));
    CHECK(b[1] == doctest::Approx(2.0f));
    CHECK(b[2] == doctest::Approx(3.0f));
}

TEST_CASE("ArrayD assignment self-assignment does not crash") {
    ArrayD a(2);
    a[0] = 10.0f; a[1] = 20.0f;
    a = a;
    CHECK(a.size() == 2);
    CHECK(a[0] == doctest::Approx(10.0f));
    CHECK(a[1] == doctest::Approx(20.0f));
}


TEST_CASE("ArrayD resize throws on negative size") {
    ArrayD a(1);
    CHECK_THROWS_AS(a.resize(-1), std::invalid_argument);
}

TEST_CASE("ArrayD resize grows with reallocation (capacity_ < size)") {
    ArrayD a(2);
    a[0] = 1.0f; a[1] = 2.0f;
    a.resize(4);
    CHECK(a.size() == 4);
    CHECK(a[0] == doctest::Approx(1.0f));
    CHECK(a[1] == doctest::Approx(2.0f));
    CHECK(a[2] == doctest::Approx(0.0f));
    CHECK(a[3] == doctest::Approx(0.0f));
}

TEST_CASE("ArrayD resize grows inside capacity (size_ < size <= capacity_)") {
    ArrayD a(4);
    a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f; a[3] = 4.0f;
    a.resize(2);   // capacity_ = 4, size_ = 2
    a.resize(3);   // size_ < size <= capacity_
    CHECK(a.size() == 3);
    CHECK(a[0] == doctest::Approx(1.0f));
    CHECK(a[1] == doctest::Approx(2.0f));
    CHECK(a[2] == doctest::Approx(0.0f));
}

TEST_CASE("ArrayD resize shrinks size (size < size_)") {
    ArrayD a(3);
    a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
    a.resize(1);
    CHECK(a.size() == 1);
    CHECK(a[0] == doctest::Approx(1.0f));
    CHECK_THROWS(a[1]);
}


TEST_CASE("ArrayD non-const operator[] read/write") {
    ArrayD a(2);
    a[0] = 5.0f;
    a[1] = 6.0f;
    CHECK(a[0] == doctest::Approx(5.0f));
    CHECK(a[1] == doctest::Approx(6.0f));
}

TEST_CASE("ArrayD non-const operator[] throws on invalid index") {
    ArrayD a(2);
    CHECK_THROWS_AS(a[-1], std::invalid_argument);
    CHECK_THROWS_AS(a[2], std::invalid_argument);
}

TEST_CASE("ArrayD const operator[] read") {
    ArrayD a(2);
    a[0] = 7.0f;
    a[1] = 8.0f;
    const ArrayD& ca = a;
    CHECK(ca[0] == doctest::Approx(7.0f));
    CHECK(ca[1] == doctest::Approx(8.0f));
}

TEST_CASE("ArrayD const operator[] throws on invalid index") {
    ArrayD a(2);
    const ArrayD& ca = a;
    CHECK_THROWS_AS(ca[-1], std::invalid_argument);
    CHECK_THROWS_AS(ca[2], std::invalid_argument);
}


TEST_CASE("ArrayD insert in middle") {
    ArrayD a(3);
    a[0] = 1.0f; a[1] = 3.0f; a[2] = 4.0f;
    a.insert(1, 2.0f); // [1,2,3,4]
    CHECK(a.size() == 4);
    CHECK(a[0] == doctest::Approx(1.0f));
    CHECK(a[1] == doctest::Approx(2.0f));
    CHECK(a[2] == doctest::Approx(3.0f));
    CHECK(a[3] == doctest::Approx(4.0f));
}

TEST_CASE("ArrayD insert at end") {
    ArrayD a(2);
    a[0] = 1.0f; a[1] = 2.0f;
    a.insert(2, 3.0f); // [1,2,3]
    CHECK(a.size() == 3);
    CHECK(a[0] == doctest::Approx(1.0f));
    CHECK(a[1] == doctest::Approx(2.0f));
    CHECK(a[2] == doctest::Approx(3.0f));
}

TEST_CASE("ArrayD insert throws on invalid index") {
    ArrayD a(2);
    CHECK_THROWS_AS(a.insert(-1, 10.0f), std::invalid_argument);
    CHECK_THROWS_AS(a.insert(3, 10.0f), std::invalid_argument);
}


TEST_CASE("ArrayD remove from middle") {
    ArrayD a(4);
    a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f; a[3] = 4.0f;
    a.remove(1); // [1,3,4]
    CHECK(a.size() == 3);
    CHECK(a[0] == doctest::Approx(1.0f));
    CHECK(a[1] == doctest::Approx(3.0f));
    CHECK(a[2] == doctest::Approx(4.0f));
}

TEST_CASE("ArrayD remove last element") {
    ArrayD a(3);
    a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
    a.remove(2); // [1,2]
    CHECK(a.size() == 2);
    CHECK(a[0] == doctest::Approx(1.0f));
    CHECK(a[1] == doctest::Approx(2.0f));
    CHECK_THROWS_AS(a[2], std::invalid_argument);
}

TEST_CASE("ArrayD remove throws on invalid index") {
    ArrayD a(3);
    CHECK_THROWS_AS(a.remove(-1), std::invalid_argument);
    CHECK_THROWS_AS(a.remove(3), std::invalid_argument);
}
#ifdef _MSC_VER
#endif
