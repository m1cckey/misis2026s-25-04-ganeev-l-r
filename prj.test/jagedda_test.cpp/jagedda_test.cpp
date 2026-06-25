#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4834)
#endif

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <jaggeda/jaggeda.hpp>

TEST_CASE("JaggedA default ctor + empty") {
    JaggedA ja;
    CHECK(ja.empty());
    CHECK(ja.size() == 0);
    CHECK(ja.data() == nullptr);
}

TEST_CASE("JaggedA ctor(rows, default_row_size)") {
    JaggedA ja(3, 2);
    CHECK(ja.size() == 3);
    CHECK(ja.row_size(0) == 2);
    CHECK(ja.row_size(1) == 2);
    CHECK(ja.row_size(2) == 2);
    CHECK(ja[0][0] == 0);
    CHECK(ja[0][1] == 0);
    ja[1][0] = 5;
    CHECK(ja[1][0] == 5);
}

TEST_CASE("JaggedA ctor with zero rows") {
    JaggedA ja(0, 5);
    CHECK(ja.empty());
    CHECK(ja.size() == 0);
}

TEST_CASE("JaggedA initializer_list ctor") {
    JaggedA ja{ {1, 2, 3}, {4, 5}, {6} };
    CHECK(ja.size() == 3);
    CHECK(ja.row_size(0) == 3);
    CHECK(ja.row_size(1) == 2);
    CHECK(ja.row_size(2) == 1);
    CHECK(ja[0][0] == 1);
    CHECK(ja[0][2] == 3);
    CHECK(ja[1][1] == 5);
    CHECK(ja[2][0] == 6);
}

TEST_CASE("JaggedA copy ctor deep copy") {
    JaggedA ja(2, 2);
    ja[0][0] = 1; ja[0][1] = 2;
    ja[1][0] = 3; ja[1][1] = 4;
    JaggedA copy(ja);
    CHECK(copy.size() == 2);
    CHECK(copy[0][0] == 1);
    CHECK(copy[1][1] == 4);
    copy[0][0] = 99;
    CHECK(ja[0][0] == 1);
}

TEST_CASE("JaggedA copy assignment") {
    JaggedA a(2, 2);
    a[0][0] = 1; a[0][1] = 2;
    JaggedA b(1, 1);
    b = a;
    CHECK(b.size() == 2);
    CHECK(b.row_size(0) == 2);
    CHECK(b[0][0] == 1);
    CHECK(b[0][1] == 2);
}

TEST_CASE("JaggedA self-assignment") {
    JaggedA a(2, 2);
    a[0][0] = 10; a[0][1] = 20;
    a = a;
    CHECK(a.size() == 2);
    CHECK(a[0][0] == 10);
    CHECK(a[0][1] == 20);
}

TEST_CASE("JaggedA move ctor leaves source empty") {
    JaggedA a(2, 2);
    a[0][0] = 1; a[0][1] = 2;
    a[1][0] = 3; a[1][1] = 4;
    JaggedA b(std::move(a));
    CHECK(b.size() == 2);
    CHECK(b[0][0] == 1);
#pragma warning(suppress: 26800)
    CHECK(a.empty());
#pragma warning(suppress: 26800)
    CHECK(a.size() == 0);
#pragma warning(suppress: 26800)
    CHECK(a.data() == nullptr);
}

TEST_CASE("JaggedA move assignment") {
    JaggedA a(2, 2);
    a[0][0] = 1;
    JaggedA b;
    b = std::move(a);
    CHECK(b.size() == 2);
    CHECK(b[0][0] == 1);
#pragma warning(suppress: 26800)
    CHECK(a.empty());
}

TEST_CASE("JaggedA at bounds checking") {
    JaggedA ja(2, 2);
    ja[0][0] = 1; ja[0][1] = 2;
    ja[1][0] = 3; ja[1][1] = 4;
    CHECK(ja.at(0, 0) == 1);
    CHECK(ja.at(1, 1) == 4);
#pragma warning(suppress: 6031)
    CHECK_THROWS_AS(ja.at(2, 0), std::out_of_range);
#pragma warning(suppress: 6031)
    CHECK_THROWS_AS(ja.at(0, 2), std::out_of_range);
}

TEST_CASE("JaggedA operator[] row bounds") {
    JaggedA ja(2, 2);
#pragma warning(suppress: 6031)
    CHECK_THROWS_AS(ja[2], std::invalid_argument);
#pragma warning(suppress: 6031)
    CHECK_THROWS_AS(ja[100], std::invalid_argument);
}

TEST_CASE("JaggedA pop_back_row empty throws") {
    JaggedA ja;
    CHECK_THROWS_AS(ja.pop_back_row(), std::out_of_range);
}

TEST_CASE("JaggedA push_back_row and pop_back_row") {
    JaggedA ja;
    ja.push_back_row(3);
    CHECK(ja.size() == 1);
    CHECK(ja.row_size(0) == 3);
    ja[0][0] = 1; ja[0][1] = 2; ja[0][2] = 3;

    ja.push_back_row(2);
    CHECK(ja.size() == 2);
    ja[1][0] = 4; ja[1][1] = 5;

    ja.pop_back_row();
    CHECK(ja.size() == 1);
    CHECK(ja.row_size(0) == 3);
    CHECK(ja[0][2] == 3);
}

TEST_CASE("JaggedA resize_rows grow and shrink") {
    JaggedA ja(2, 2);
    ja[0][0] = 1; ja[0][1] = 2;
    ja[1][0] = 3; ja[1][1] = 4;

    ja.resize_rows(4);
    CHECK(ja.size() == 4);
    CHECK(ja.row_size(0) == 2);
    CHECK(ja.row_size(1) == 2);
    CHECK(ja.row_size(2) == 0);
    CHECK(ja.row_size(3) == 0);
    CHECK(ja[0][0] == 1);

    ja.resize_rows(1);
    CHECK(ja.size() == 1);
    CHECK(ja[0][0] == 1);
    CHECK(ja[0][1] == 2);
}

TEST_CASE("JaggedA resize_row grow and shrink") {
    JaggedA ja(1, 2);
    ja[0][0] = 1; ja[0][1] = 2;

    ja.resize_row(0, 4);
    CHECK(ja.row_size(0) == 4);
    CHECK(ja[0][0] == 1);
    CHECK(ja[0][1] == 2);
    CHECK(ja[0][2] == 0);
    CHECK(ja[0][3] == 0);

    ja.resize_row(0, 1);
    CHECK(ja.row_size(0) == 1);
    CHECK(ja[0][0] == 1);
}

TEST_CASE("JaggedA insert_row") {
    JaggedA ja;
    ja.push_back_row(2);
    ja[0][0] = 1; ja[0][1] = 2;
    ja.insert_row(0, 1);
    CHECK(ja.size() == 2);
    CHECK(ja.row_size(0) == 1);
    CHECK(ja.row_size(1) == 2);
    CHECK(ja[1][0] == 1);
    CHECK(ja[1][1] == 2);

    ja.insert_row(2, 3);
    CHECK(ja.size() == 3);
    CHECK(ja.row_size(2) == 3);
}

TEST_CASE("JaggedA erase_row") {
    JaggedA ja(3, 1);
    ja[0][0] = 1;
    ja[1][0] = 2;
    ja[2][0] = 3;

    ja.erase_row(1);
    CHECK(ja.size() == 2);
    CHECK(ja[0][0] == 1);
    CHECK(ja[1][0] == 3);
}

TEST_CASE("JaggedA clear") {
    JaggedA ja(2, 2);
    ja[0][0] = 1; ja[0][1] = 2;
    ja.clear();
    CHECK(ja.empty());
    CHECK(ja.size() == 0);
}

TEST_CASE("JaggedA operator== and operator!=") {
    JaggedA a(2, 2);
    a[0][0] = 1; a[0][1] = 2;
    a[1][0] = 3; a[1][1] = 4;

    JaggedA b(2, 2);
    b[0][0] = 1; b[0][1] = 2;
    b[1][0] = 3; b[1][1] = 4;

    CHECK(a == b);

    b[1][1] = 99;
    CHECK(a != b);

    JaggedA c(2, 3);
    CHECK(a != c);

    JaggedA d(3, 2);
    CHECK(a != d);
}

TEST_CASE("JaggedA empty rows") {
    JaggedA ja;
    ja.push_back_row(0);
    ja.push_back_row(3);
    CHECK(ja.size() == 2);
    CHECK(ja.row_size(0) == 0);
    CHECK(ja.row_size(1) == 3);
    ja[1][0] = 1; ja[1][1] = 2; ja[1][2] = 3;
}

TEST_CASE("JaggedA swap") {
    JaggedA a(2, 2);
    a[0][0] = 1;
    JaggedA b(1, 1);
    b[0][0] = 99;
    a.swap(b);
    CHECK(a.size() == 1);
    CHECK(a[0][0] == 99);
    CHECK(b.size() == 2);
    CHECK(b[0][0] == 1);
}

TEST_CASE("JaggedA const operator[]") {
    JaggedA ja(2, 2);
    ja[0][0] = 1; ja[0][1] = 2;
    const JaggedA& cja = ja;
    CHECK(cja[0][0] == 1);
    CHECK(cja[0][1] == 2);
}

TEST_CASE("JaggedA sized construction with zero row size") {
    JaggedA ja(3, 0);
    CHECK(ja.size() == 3);
    CHECK(ja.row_size(0) == 0);
    CHECK(ja.row_size(1) == 0);
    CHECK(ja.row_size(2) == 0);
    CHECK(ja.data() != nullptr);
}

TEST_CASE("JaggedA empty initializer_list construction") {
    JaggedA ja{};
    CHECK(ja.size() == 0);
    CHECK(ja.empty());
    CHECK(ja.data() == nullptr);
}

TEST_CASE("JaggedA move self-assignment") {
    JaggedA a(2, 2);
    a[0][0] = 1; a[0][1] = 2;
    a[1][0] = 3; a[1][1] = 4;
    a = std::move(a);
#pragma warning(suppress: 26800)
    CHECK(a.size() == 2);
#pragma warning(suppress: 26800)
    CHECK(a[0][0] == 1);
#pragma warning(suppress: 26800)
    CHECK(a[1][1] == 4);
}

TEST_CASE("JaggedA destructor stress") {
    for (int i = 0; i < 1000; ++i) {
        JaggedA ja(10, 10);
        ja[5][5] = i;
        ja.push_back_row(5);
        ja.pop_back_row();
    }
    CHECK(true);
}

TEST_CASE("JaggedA operator[][] explicit read and write") {
    JaggedA a(3, 5);
    a[1][2] = 42;
    CHECK(a[1][2] == 42);
    a[1][2] = -7;
    CHECK(a.at(1, 2) == -7);
}

TEST_CASE("JaggedA at valid access read and write") {
    JaggedA a(2, 3);
    a.at(0, 0) = 5;
    CHECK(a.at(0, 0) == 5);
    a.at(0, 2) = 10;
    CHECK(a.at(0, 2) == 10);
}

TEST_CASE("JaggedA const at access compiles and returns correct values") {
    JaggedA ja(2, 2);
    ja[0][0] = 7; ja[0][1] = 8;
    ja[1][0] = 9; ja[1][1] = 10;
    const JaggedA& cja = ja;
    CHECK(cja.at(0, 0) == 7);
    CHECK(cja.at(1, 1) == 10);
}

TEST_CASE("JaggedA chained access") {
    JaggedA a(2, 2);
    a[1][1] = 10;
    a[0][0] = a[1][1] + 5;
    CHECK(a[0][0] == 15);
}

TEST_CASE("JaggedA row proxy size") {
    JaggedA a(2, 3);
    CHECK(a[0].size() == 3);
    CHECK(a[1].size() == 3);
    a.resize_row(0, 5);
    CHECK(a[0].size() == 5);
}

TEST_CASE("JaggedA row proxy write modifies original") {
    JaggedA a(2, 2);
    auto r = a[1];
    r[0] = 5;
    r[1] = 6;
    CHECK(a[1][0] == 5);
    CHECK(a[1][1] == 6);
}

TEST_CASE("JaggedA row proxy on empty row") {
    JaggedA a(1, 0);
    CHECK(a[0].size() == 0);
}

TEST_CASE("JaggedA const row proxy returns const reference") {
    JaggedA ja(1, 1);
    ja[0][0] = 99;
    const auto r = ja[0];
    CHECK(r[0] == 99);
}

TEST_CASE("JaggedA resize_rows to zero") {
    JaggedA ja(2, 2);
    ja[0][0] = 1; ja[0][1] = 2;
    ja[1][0] = 3; ja[1][1] = 4;
    ja.resize_rows(0);
    CHECK(ja.size() == 0);
    CHECK(ja.empty());
}

TEST_CASE("JaggedA resize_row on empty array") {
    JaggedA ja;
    ja.resize_rows(1);
    ja.resize_row(0, 5);
    CHECK(ja.row_size(0) == 5);
    CHECK(ja[0][0] == 0);
    CHECK(ja[0][4] == 0);
}

TEST_CASE("JaggedA resize_row strong guarantee on bad_alloc") {
    JaggedA ja(1, 2);
    ja[0][0] = 1; ja[0][1] = 2;
    std::size_t huge = static_cast<std::size_t>(-1) / sizeof(std::int32_t);
    CHECK_THROWS_AS(ja.resize_row(0, huge), std::bad_alloc);
    CHECK(ja.row_size(0) == 2);
    CHECK(ja[0][0] == 1);
    CHECK(ja[0][1] == 2);
}

TEST_CASE("JaggedA resize_rows basic guarantee on bad_alloc") {
    JaggedA ja(2, 2);
    ja[0][0] = 1; ja[0][1] = 2;
    ja[1][0] = 3; ja[1][1] = 4;
    std::size_t huge = static_cast<std::size_t>(-1) / sizeof(std::int32_t);
    CHECK_THROWS_AS(ja.resize_rows(huge), std::bad_alloc);
    CHECK(ja.size() == 2);
    CHECK(ja[0][0] == 1);
    CHECK(ja[1][1] == 4);
}

TEST_CASE("JaggedA push_back_row default creates empty row") {
    JaggedA ja;
    ja.push_back_row();
    CHECK(ja.size() == 1);
    CHECK(ja.row_size(0) == 0);
}

TEST_CASE("JaggedA push_back_row without reallocation strong guarantee") {
    JaggedA ja;
    ja.resize_rows(10);
    ja.resize_rows(0);
    ja.push_back_row(2);
    ja[0][0] = 1; ja[0][1] = 2;
    std::size_t huge = static_cast<std::size_t>(-1) / sizeof(std::int32_t);
    CHECK_THROWS_AS(ja.push_back_row(huge), std::bad_alloc);
    CHECK(ja.size() == 1);
    CHECK(ja[0][0] == 1);
    CHECK(ja[0][1] == 2);
}

TEST_CASE("JaggedA insert_row at middle shifts correctly") {
    JaggedA ja(3, 1);
    ja[0][0] = 1;
    ja[1][0] = 2;
    ja[2][0] = 3;
    ja.insert_row(1, 1);
    ja[1][0] = 99;
    CHECK(ja.size() == 4);
    CHECK(ja[0][0] == 1);
    CHECK(ja[1][0] == 99);
    CHECK(ja[2][0] == 2);
    CHECK(ja[3][0] == 3);
}

TEST_CASE("JaggedA insert_row at end equivalent to push_back_row") {
    JaggedA ja(2, 1);
    ja[0][0] = 1;
    ja[1][0] = 2;
    ja.insert_row(2, 1);
    ja[2][0] = 3;
    CHECK(ja.size() == 3);
    CHECK(ja[2][0] == 3);
}

TEST_CASE("JaggedA erase_row at beginning shifts correctly") {
    JaggedA ja(3, 1);
    ja[0][0] = 1;
    ja[1][0] = 2;
    ja[2][0] = 3;
    ja.erase_row(0);
    CHECK(ja.size() == 2);
    CHECK(ja[0][0] == 2);
    CHECK(ja[1][0] == 3);
}

TEST_CASE("JaggedA erase_row at middle removes correct row") {
    JaggedA ja(3, 1);
    ja[0][0] = 1;
    ja[1][0] = 2;
    ja[2][0] = 3;
    ja.erase_row(1);
    CHECK(ja.size() == 2);
    CHECK(ja[0][0] == 1);
    CHECK(ja[1][0] == 3);
}

TEST_CASE("JaggedA erase_row at end equivalent to pop_back_row") {
    JaggedA ja(3, 1);
    ja[0][0] = 1;
    ja[1][0] = 2;
    ja[2][0] = 3;
    ja.erase_row(2);
    CHECK(ja.size() == 2);
    CHECK(ja[0][0] == 1);
    CHECK(ja[1][0] == 2);
}

TEST_CASE("JaggedA erase_row invalid index throws") {
    JaggedA ja(2, 1);
    CHECK_THROWS_AS(ja.erase_row(2), std::invalid_argument);
    CHECK_THROWS_AS(ja.erase_row(100), std::invalid_argument);
}

TEST_CASE("JaggedA multiple push then multiple pop stress") {
    JaggedA ja;
    for (int i = 0; i < 100; ++i) {
        ja.push_back_row(static_cast<std::size_t>(i % 10));
    }
    CHECK(ja.size() == 100);
    for (int i = 0; i < 100; ++i) {
        ja.pop_back_row();
    }
    CHECK(ja.empty());
    CHECK(ja.size() == 0);
}

TEST_CASE("JaggedA swap with empty object") {
    JaggedA a(2, 2);
    a[0][0] = 1; a[0][1] = 2;
    JaggedA b;
    a.swap(b);
    CHECK(a.empty());
    CHECK(a.data() == nullptr);
    CHECK(b.size() == 2);
    CHECK(b[0][0] == 1);
    CHECK(b[0][1] == 2);
}

TEST_CASE("JaggedA data non-null after non-empty construction") {
    JaggedA ja(1, 1);
    CHECK(ja.data() != nullptr);
}

TEST_CASE("JaggedA data invalidation after resize_rows reallocation") {
    JaggedA ja(2, 2);
    auto old_data = ja.data();
    ja.resize_rows(100);
    CHECK(ja.data() != old_data);
}

TEST_CASE("JaggedA const data returns correct pointer type") {
    JaggedA ja(1, 1);
    const JaggedA& cja = ja;
    const std::int32_t* const* d = cja.data();
    CHECK(d != nullptr);
    CHECK(d[0] != nullptr);
}

TEST_CASE("JaggedA clear already empty is safe") {
    JaggedA ja;
    CHECK_NOTHROW(ja.clear());
    CHECK(ja.empty());
    CHECK(ja.size() == 0);
    CHECK(ja.data() == nullptr);
}

TEST_CASE("JaggedA clear then reuse") {
    JaggedA ja(2, 2);
    ja[0][0] = 1;
    ja.clear();
    CHECK(ja.empty());
    ja.push_back_row(3);
    CHECK(ja.size() == 1);
    CHECK(ja.row_size(0) == 3);
    ja[0][0] = 10; ja[0][1] = 20; ja[0][2] = 30;
    CHECK(ja[0][2] == 30);
}

TEST_CASE("JaggedA insert_row with reallocation basic guarantee") {
    JaggedA ja;
    ja.push_back_row(1);
    ja[0][0] = 42;
    std::size_t huge = static_cast<std::size_t>(-1) / sizeof(std::int32_t);
    CHECK_THROWS_AS(ja.insert_row(0, huge), std::bad_alloc);
    CHECK(ja.size() == 1);
    CHECK(ja[0][0] == 42);
}

TEST_CASE("JaggedA insert_row without reallocation strong guarantee") {
    JaggedA ja;
    ja.resize_rows(10);
    ja.resize_rows(0);
    ja.push_back_row(1);
    ja[0][0] = 5;
    std::size_t huge = static_cast<std::size_t>(-1) / sizeof(std::int32_t);
    CHECK_THROWS_AS(ja.insert_row(0, huge), std::bad_alloc);
    CHECK(ja.size() == 1);
    CHECK(ja[0][0] == 5);
}

TEST_CASE("JaggedA no-throw pop_back and erase on valid data") {
    JaggedA ja(3, 1);
    CHECK_NOTHROW(ja.pop_back_row());
    CHECK_NOTHROW(ja.erase_row(0));
    CHECK(ja.size() == 1);
}

TEST_CASE("JaggedA chain operations integration") {
    JaggedA a{ {1, 2}, {3, 4, 5} };
    CHECK(a.size() == 2);
    a.resize_rows(4);
    a.resize_row(2, 2);
    a.resize_row(3, 3);
    a[2][0] = 6; a[2][1] = 7;
    a[3][0] = 8; a[3][1] = 9; a[3][2] = 10;

    JaggedA b(a);
    b[0][0] = 99;
    CHECK(a[0][0] == 1);

    JaggedA c(std::move(b));
    CHECK(c[0][0] == 99);
#pragma warning(suppress: 26800)
    CHECK(b.empty());
#pragma warning(suppress: 26800)
    CHECK(b.data() == nullptr);

    c.clear();
    c.push_back_row(1);
    c[0][0] = 42;
    CHECK(c[0][0] == 42);
}

TEST_CASE("JaggedA large array stress") {
    JaggedA ja;
    for (int i = 0; i < 1000; ++i) {
        ja.push_back_row(static_cast<std::size_t>(i % 50));
    }
    CHECK(ja.size() == 1000);
    for (int i = 0; i < 1000; ++i) {
        if (ja.row_size(static_cast<std::size_t>(i)) > 0) {
            ja[i][0] = static_cast<std::int32_t>(i);
        }
    }
    for (int i = 0; i < 1000; ++i) {
        if (ja.row_size(static_cast<std::size_t>(i)) > 0) {
            CHECK(ja[i][0] == i);
        }
    }
}

TEST_CASE("JaggedA mixed access and modification") {
    JaggedA ja(5, 0);
    for (std::size_t i = 0; i < ja.size(); ++i) {
        ja.resize_row(i, i + 1);
        for (std::size_t j = 0; j < ja.row_size(i); ++j) {
            ja.at(i, j) = static_cast<std::int32_t>(i * 10 + j);
        }
    }
    for (std::size_t i = 0; i < ja.size(); ++i) {
        for (std::size_t j = 0; j < ja.row_size(i); ++j) {
            CHECK(ja[i][j] == static_cast<std::int32_t>(i * 10 + j));
        }
    }
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
