#include "BitsetD.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <type_traits>

template <std::int32_t N>
void test_default_and_fill() {
  BitsetD b(N, false);
  assert(b.size() == N);
  for (std::int32_t i = 0; i < N; ++i) {
    assert(b.get(i) == false);
  }

  BitsetD c(N, true);
  assert(c.size() == N);
  for (std::int32_t i = 0; i < N; ++i) {
    assert(c.get(i) == true);
  }
}

template <std::int32_t N>
void test_mask_ctor() {
  BitsetD b(UINT64_C(0xA5), N);
  assert(b.size() == N);
  assert(b.get(0) == true);
  assert(b.get(2) == true);
  assert(b.get(5) == true);
  assert(b.get(7) == true);
}

template <std::int32_t N>
void test_get_set_and_index() {
  BitsetD b(N, false);
  b.set(0, true);
  b.set(N - 1, true);
  assert(b.get(0) == true);
  assert(b.get(N - 1) == true);
  assert(static_cast<bool>(b[0]) == true);
  assert(static_cast<bool>(b[N - 1]) == true);

  b[0] = false;
  b[N - 1] = false;
  assert(b.get(0) == false);
  assert(b.get(N - 1) == false);
}

template <std::int32_t N>
void test_equality() {
  BitsetD a(N, false);
  BitsetD b(N, false);
  assert(a == b);

  a.set(1, true);
  assert(!(a == b));
  b.set(1, true);
  assert(a == b);
}

template <std::int32_t N>
void test_resize_grow_and_shrink() {
  BitsetD b(N, false);
  b.set(0, true);
  if (N > 2) b.set(N - 1, true);

  b.resize(N + 5, false);
  assert(b.size() == N + 5);
  assert(b.get(0) == true);
  if (N > 2) assert(b.get(N - 1) == true);

  b.resize(N, false);
  assert(b.size() == N);
  assert(b.get(0) == true);
}

template <std::int32_t N>
void test_invert() {
  BitsetD b(N, false);
  b.set(0, true);
  b.invert();
  assert(b.get(0) == false);
  for (std::int32_t i = 1; i < N; ++i) {
    assert(b.get(i) == true);
  }
}

template <std::int32_t N>
void test_shift_lr() {
  BitsetD b(N, false);
  if (N >= 4) {
    b.set(1, true);
    b.set(3, true);

    BitsetD l = b;
    l <<= 1;
    assert(l.get(2) == true);
    assert(l.get(4) == true || (N <= 4 && true));

    BitsetD r = b;
    r >>= 1;
    assert(r.get(0) == true);
    assert(r.get(2) == true);
  }
}

template <std::int32_t N>
void test_bitwise_ops() {
  BitsetD a(N, false), b(N, false);
  a.set(0, true);
  a.set(1, true);
  b.set(1, true);
  b.set(2, true);

  BitsetD c = a;
  c &= b;
  assert(c.get(0) == false);
  assert(c.get(1) == true);
  assert(c.get(2) == false);

  c = a;
  c |= b;
  assert(c.get(0) == true);
  assert(c.get(1) == true);
  assert(c.get(2) == true);

  c = a;
  c ^= b;
  assert(c.get(0) == true);
  assert(c.get(1) == false);
  assert(c.get(2) == true);
}

template <std::int32_t N>
void test_to_string() {
  BitsetD b(N, false);
  b.set(0, true);
  b.set(N - 1, true);

  std::string s = b.to_string(BitsetD::StrFormat::BinNoPreSep);
  assert(!s.empty());

  std::string h = b.to_string(BitsetD::StrFormat::Hex);
  assert(!h.empty());

  std::string o = b.to_string(BitsetD::StrFormat::Oct);
  assert(!o.empty());
}

template <typename T>
struct TypeTag {};

template <typename T>
void test_template_type_behavior() {
  static_assert(std::is_same_v<T, std::uint32_t> || std::is_same_v<T, std::uint64_t>);
  T x = static_cast<T>(1);
  assert(x == 1);
}

template <typename T, std::int32_t N>
void run_all_for_type_and_size() {
  test_default_and_fill<N>();
  test_mask_ctor<N>();
  test_get_set_and_index<N>();
  test_equality<N>();
  test_resize_grow_and_shrink<N>();
  test_invert<N>();
  test_shift_lr<N>();
  test_bitwise_ops<N>();
  test_to_string<N>();
  test_template_type_behavior<T>();
}

int main() {
  run_all_for_type_and_size<std::uint32_t, 8>();
  run_all_for_type_and_size<std::uint32_t, 33>();
  run_all_for_type_and_size<std::uint64_t, 64>();

  {
    BitsetD b(8, false);
    bool thrown = false;
    try { b.get(8); } catch (const std::out_of_range&) { thrown = true; }
    assert(thrown);
  }

  {
    bool thrown = false;
    try { BitsetD bad(0, false); } catch (const std::invalid_argument&) { thrown = true; }
    assert(thrown);
  }

  std::cout << "All tests passed\n";
  return 0;
}
