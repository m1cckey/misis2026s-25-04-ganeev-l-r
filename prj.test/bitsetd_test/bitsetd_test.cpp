#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <bitsetd/bitsetd.hpp>

TEST_CASE("BitsetD default ctor creates empty bitset") {
    BitsetD bs;
    CHECK(bs.size() == 0);
}

TEST_CASE("BitsetD ctor(size, val) basic") {
    BitsetD bs(10, false);
    CHECK(bs.size() == 10);
    for (int i = 0; i < 10; ++i) {
        CHECK(bs.get(i) == false);
    }
}

TEST_CASE("BitsetD ctor(size, true) sets all bits") {
    BitsetD bs(5, true);
    CHECK(bs.size() == 5);
    for (int i = 0; i < 5; ++i) {
        CHECK(bs.get(i) == true);
    }
}

TEST_CASE("BitsetD ctor throws on negative size") {
    CHECK_THROWS_AS(BitsetD(-1, false), std::invalid_argument);
    CHECK_THROWS_AS(BitsetD(-5, true), std::invalid_argument);
}

TEST_CASE("BitsetD ctor from mask") {
    BitsetD bs(0b1010u, 4);
    CHECK(bs.size() == 4);
    CHECK(bs.get(0) == false);
    CHECK(bs.get(1) == true);
    CHECK(bs.get(2) == false);
    CHECK(bs.get(3) == true);
}

TEST_CASE("BitsetD get/set basic") {
    BitsetD bs(8, false);
    bs.set(3, true);
    bs.set(7, true);
    CHECK(bs.get(3) == true);
    CHECK(bs.get(7) == true);
    CHECK(bs.get(0) == false);
    bs.set(3, false);
    CHECK(bs.get(3) == false);
}

TEST_CASE("BitsetD get/set throws on out of range") {
    BitsetD bs(5, false);
    CHECK_THROWS_AS(bs.get(-1), std::out_of_range);
    CHECK_THROWS_AS(bs.get(5), std::out_of_range);
    CHECK_THROWS_AS(bs.set(-1, true), std::out_of_range);
    CHECK_THROWS_AS(bs.set(5, true), std::out_of_range);
}

TEST_CASE("BitsetD operator[] read/write") {
    BitsetD bs(5, false);
    bs[2] = true;
    CHECK(bs[2] == true);
    bs[2] = false;
    CHECK(bs[2] == false);
}

TEST_CASE("BitsetD const operator[] read") {
    BitsetD bs(5, false);
    bs.set(2, true);
    const BitsetD& cbs = bs;
    CHECK(cbs[2] == true);
    CHECK(cbs[0] == false);
}

TEST_CASE("BitsetD copy ctor") {
    BitsetD bs(5, false);
    bs.set(1, true);
    bs.set(3, true);
    BitsetD copy(bs);
    CHECK(copy.size() == 5);
    CHECK(copy.get(1) == true);
    CHECK(copy.get(3) == true);
    copy.set(1, false);
    CHECK(bs.get(1) == true);
}

TEST_CASE("BitsetD copy assignment") {
    BitsetD a(3, false);
    a.set(0, true);
    BitsetD b(5, true);
    b = a;
    CHECK(b.size() == 3);
    CHECK(b.get(0) == true);
    CHECK(b.get(1) == false);
}

TEST_CASE("BitsetD move ctor") {
    BitsetD a(4, true);
    BitsetD b(std::move(a));
    CHECK(b.size() == 4);
    CHECK(b.get(0) == true);
    CHECK(a.size() == 0);
}

TEST_CASE("BitsetD move assignment") {
    BitsetD a(4, true);
    BitsetD b;
    b = std::move(a);
    CHECK(b.size() == 4);
    CHECK(b.get(0) == true);
    CHECK(a.size() == 0);
}

TEST_CASE("BitsetD resize grow with val=false") {
    BitsetD bs(3, true);
    bs.resize(5, false);
    CHECK(bs.size() == 5);
    CHECK(bs.get(0) == true);
    CHECK(bs.get(1) == true);
    CHECK(bs.get(2) == true);
    CHECK(bs.get(3) == false);
    CHECK(bs.get(4) == false);
}

TEST_CASE("BitsetD resize grow with val=true") {
    BitsetD bs(30, false);
    bs.set(0, true);
    bs.resize(35, true);
    CHECK(bs.size() == 35);
    CHECK(bs.get(0) == true);
    CHECK(bs.get(1) == false); // старые биты сохранились
    for (int i = 30; i < 35; ++i) {
        CHECK(bs.get(i) == true);
    }
}

TEST_CASE("BitsetD resize shrink") {
    BitsetD bs(5, true);
    bs.resize(2);
    CHECK(bs.size() == 2);
    CHECK(bs.get(0) == true);
    CHECK(bs.get(1) == true);
}

TEST_CASE("BitsetD resize throws on negative") {
    BitsetD bs(5, false);
    CHECK_THROWS_AS(bs.resize(-1), std::invalid_argument);
}

TEST_CASE("BitsetD == and !=") {
    BitsetD a(4, false);
    a.set(1, true);
    BitsetD b(4, false);
    b.set(1, true);
    CHECK(a == b);
    b.set(2, true);
    CHECK(!(a == b));
    CHECK(a != b); // через operator==
}

TEST_CASE("BitsetD == different sizes") {
    BitsetD a(3, false);
    BitsetD b(4, false);
    CHECK(!(a == b));
}

TEST_CASE("BitsetD fill") {
    BitsetD bs(7, false);
    bs.fill(true);
    for (int i = 0; i < 7; ++i) {
        CHECK(bs.get(i) == true);
    }
    bs.fill(false);
    for (int i = 0; i < 7; ++i) {
        CHECK(bs.get(i) == false);
    }
}

TEST_CASE("BitsetD invert and operator~") {
    BitsetD bs(5, false);
    bs.set(0, true);
    bs.set(3, true);
    bs.invert();
    CHECK(bs.get(0) == false);
    CHECK(bs.get(1) == true);
    CHECK(bs.get(3) == false);
    CHECK(bs.get(4) == true);

    BitsetD neg = ~BitsetD(3, false);
    CHECK(neg.get(0) == true);
    CHECK(neg.get(1) == true);
    CHECK(neg.get(2) == true);
}

TEST_CASE("BitsetD shift (cyclic)") {
    BitsetD bs(5, false);
    bs.set(0, true);
    bs.set(2, true);
    bs.shift(1);
    CHECK(bs.get(1) == true);
    CHECK(bs.get(3) == true);
    CHECK(bs.get(0) == false);
    CHECK(bs.get(2) == false);
}

TEST_CASE("BitsetD shift negative") {
    BitsetD bs(4, false);
    bs.set(1, true);
    bs.shift(-1);
    CHECK(bs.get(0) == true);
    CHECK(bs.get(1) == false);
}

TEST_CASE("BitsetD operator<<=") {
    BitsetD bs(5, false);
    bs.set(0, true);
    bs.set(1, true);
    bs <<= 2;
    CHECK(bs.get(2) == true);
    CHECK(bs.get(3) == true);
    CHECK(bs.get(0) == false);
    CHECK(bs.get(1) == false);
}

TEST_CASE("BitsetD operator<<=") {
    BitsetD bs(4, false);
    bs.set(0, true);
    bs <<= 4;
    for (int i = 0; i < 4; ++i) {
        CHECK(bs.get(i) == false);
    }
}

TEST_CASE("BitsetD operator<<") {
    BitsetD bs(4, false);
    bs.set(1, true);
    BitsetD shifted = bs << 1;
    CHECK(shifted.get(2) == true);
    CHECK(bs.get(1) == true); // оригинал не изменился
}

TEST_CASE("BitsetD operator>>=") {
    BitsetD bs(5, false);
    bs.set(3, true);
    bs.set(4, true);
    bs >>= 2;
    CHECK(bs.get(1) == true);
    CHECK(bs.get(2) == true);
    CHECK(bs.get(3) == false);
}

TEST_CASE("BitsetD operator>>") {
    BitsetD bs(4, false);
    bs.set(2, true);
    BitsetD shifted = bs >> 1;
    CHECK(shifted.get(1) == true);
    CHECK(bs.get(2) == true);
}

TEST_CASE("BitsetD operator&=") {
    BitsetD a(4, false); a.set(0, true); a.set(1, true);
    BitsetD b(4, false); b.set(1, true); b.set(2, true);
    a &= b;
    CHECK(a.get(0) == false);
    CHECK(a.get(1) == true);
    CHECK(a.get(2) == false);
}

TEST_CASE("BitsetD operator&") {
    BitsetD a(3, false); a.set(0, true);
    BitsetD b(3, false); b.set(0, true); b.set(1, true);
    BitsetD c = a & b;
    CHECK(c.get(0) == true);
    CHECK(c.get(1) == false);
}

TEST_CASE("BitsetD operator|=") {
    BitsetD a(4, false); a.set(0, true);
    BitsetD b(4, false); b.set(1, true);
    a |= b;
    CHECK(a.get(0) == true);
    CHECK(a.get(1) == true);
    CHECK(a.get(2) == false);
}

TEST_CASE("BitsetD operator|") {
    BitsetD a(3, false); a.set(0, true);
    BitsetD b(3, false); b.set(1, true);
    BitsetD c = a | b;
    CHECK(c.get(0) == true);
    CHECK(c.get(1) == true);
    CHECK(c.get(2) == false);
}

TEST_CASE("BitsetD operator^=") {
    BitsetD a(4, false); a.set(0, true); a.set(1, true);
    BitsetD b(4, false); b.set(1, true); b.set(2, true);
    a ^= b;
    CHECK(a.get(0) == true);
    CHECK(a.get(1) == false);
    CHECK(a.get(2) == true);
}

TEST_CASE("BitsetD operator^") {
    BitsetD a(3, false); a.set(0, true); a.set(1, true);
    BitsetD b(3, false); b.set(1, true); b.set(2, true);
    BitsetD c = a ^ b;
    CHECK(c.get(0) == true);
    CHECK(c.get(1) == false);
    CHECK(c.get(2) == true);
}

TEST_CASE("BitsetD bitwise operators throw on size mismatch") {
    BitsetD a(3, false);
    BitsetD b(4, false);
    CHECK_THROWS_AS(a &= b, std::invalid_argument);
    CHECK_THROWS_AS(a |= b, std::invalid_argument);
    CHECK_THROWS_AS(a ^= b, std::invalid_argument);
}

TEST_CASE("BitsetD to_string binary default") {
    BitsetD bs(8, false);
    bs.set(7, true);
    bs.set(5, true);
    CHECK(bs.to_string(BitsetD::StrFormat::Bin) == "b01010'0000");
}

TEST_CASE("BitsetD to_string binary no prefix/separator") {
    BitsetD bs(4, false);
    bs.set(3, true);
    bs.set(1, true);
    CHECK(bs.to_string(BitsetD::StrFormat::BinNoPreSep) == "1010");
}

TEST_CASE("BitsetD to_string hex") {
    BitsetD bs(8, false);
    bs.set(7, true);
    bs.set(4, true);
    CHECK(bs.to_string(BitsetD::StrFormat::Hex) == "90");
}

TEST_CASE("BitsetD to_string oct") {
    BitsetD bs(6, false);
    bs.set(5, true);
    bs.set(3, true);
    CHECK(bs.to_string(BitsetD::StrFormat::Oct) == "50");
}

TEST_CASE("BitsetD to_string empty") {
    BitsetD bs;
    CHECK(bs.to_string() == "empty");
}

TEST_CASE("BitsetD to_string with len padding") {
    BitsetD bs(4, false);
    bs.set(3, true);
    CHECK(bs.to_string(BitsetD::StrFormat::BinNoPreSep, 8) == "00001000");
}

TEST_CASE("BitsetD operator uint64_t") {
    BitsetD bs(8, false);
    bs.set(0, true);
    bs.set(3, true);
    CHECK(static_cast<std::uint64_t>(bs) == 9u);
}

TEST_CASE("BitsetD operator uint32_t") {
    BitsetD bs(4, false);
    bs.set(1, true);
    CHECK(static_cast<std::uint32_t>(bs) == 2u);
}

TEST_CASE("BitsetD large size cross chunk boundary") {
    BitsetD bs(40, false);
    bs.set(31, true);
    bs.set(32, true);
    CHECK(bs.get(31) == true);
    CHECK(bs.get(32) == true);
    bs.set(31, false);
    CHECK(bs.get(31) == false);
}

// --- �����-����� ---
TEST_CASE("BitsetD resize from zero with true") {
    BitsetD bs;
    bs.resize(32, true);
    for (int i = 0; i < 32; ++i) {
        CHECK(bs.get(i) == true);
    }
}

TEST_CASE("BitsetD resize across chunk boundary with true") {
    BitsetD bs(30, false);
    bs.set(0, true);
    bs.resize(35, true);
    CHECK(bs.get(0) == true);    // ������ ��� ����������
    CHECK(bs.get(1) == false);   // ������� ������ ��������
    for (int i = 30; i < 35; ++i) {
        CHECK(bs.get(i) == true); // ����� ���� ������� true
    }
}

TEST_CASE("BitsetD invert on multi-chunk") {
    BitsetD bs(33, false);
    bs.set(0, true);
    bs.set(32, true);
    bs.invert();
    CHECK(bs.get(0) == false);
    CHECK(bs.get(32) == false);
    for (int i = 1; i < 32; ++i) {
        CHECK(bs.get(i) == true);
    }
    // 33-� ��� (������ ������ 33-������� ������) ������ ���� 0
    CHECK(bs.get(32) == false);
}

TEST_CASE("BitsetD shift by zero does nothing") {
    BitsetD bs(5, false);
    bs.set(2, true);
    bs.shift(0);
    CHECK(bs.get(2) == true);
}

TEST_CASE("BitsetD operator[] on empty throws on use") {
    BitsetD bs;
    auto bw = bs[0]; // BitW ������� �� ������, �� operator bool() ������ ������
    CHECK_THROWS_AS(bw.operator bool(), std::out_of_range);
}
