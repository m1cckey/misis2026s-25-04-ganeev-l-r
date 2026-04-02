#include "BitsetD.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>

BitsetD::BitsetD(const std::int32_t size, const bool val)
  : size_(size)
  , chunks_(chunks_count(), val ? UINT32_C(0xFFFFFFFF) : UINT32_C(0))
{
  if (size <= 0) {
    throw std::invalid_argument("BitsetD::BitsetD non positive size");
  }
  if (size_ % chunk_bi_s != 0) {
    chunks_.back() &= ((UINT32_C(1) << (size_ % chunk_bi_s)) - 1);
  }
}

BitsetD::BitsetD(const std::uint64_t mask, const std::int32_t size)
  : size_(size)
  , chunks_(chunks_count(), UINT32_C(0))
{
  if (size <= 0) {
    throw std::invalid_argument("BitsetD::BitsetD non positive size");
  }
  chunks_[0] = static_cast<std::uint32_t>(mask);
  if (chunks_.size() > 1) {
    chunks_[1] = static_cast<std::uint32_t>(mask >> chunk_bi_s);
  }
  if (size_ % chunk_bi_s != 0) {
    chunks_.back() &= ((UINT32_C(1) << (size_ % chunk_bi_s)) - 1);
  }
}

bool BitsetD::operator==(const BitsetD& rhs) const noexcept {
  if (size_ != rhs.size_) {
    return false;
  }
  return chunks_ == rhs.chunks_;
}

void BitsetD::resize(const std::int32_t new_size, const bool val) {
  if (new_size <= 0) {
    throw std::invalid_argument("BitsetD::resize non positive new size");
  }

  if (new_size == size_) {
    return;
  }

  const std::int32_t old_size = size_;
  const std::vector<std::uint32_t> old_chunks = chunks_;

  size_ = new_size;
  chunks_.assign(chunks_count(), val ? UINT32_C(0xFFFFFFFF) : UINT32_C(0));

  const std::int32_t limit = std::min(old_size, new_size);
  for (std::int32_t i = 0; i < limit; ++i) {
    const bool bit = (old_chunks[i / chunk_bi_s] >> (i % chunk_bi_s)) & UINT32_C(1);
    if (bit) {
      chunks_[i / chunk_bi_s] |= (UINT32_C(1) << (i % chunk_bi_s));
    }
  }

  if (new_size % chunk_bi_s != 0) {
    chunks_.back() &= ((UINT32_C(1) << (new_size % chunk_bi_s)) - 1);
  }
}

bool BitsetD::get(const std::int32_t idx) const {
  if (idx < 0 || size_ <= idx) {
    throw std::out_of_range("BitsetD::get invalid index");
  }
  return (chunks_[idx / chunk_bi_s] & (UINT32_C(1) << (idx % chunk_bi_s))) != 0;
}

void BitsetD::set(const std::int32_t idx, const bool val) {
  if (idx < 0 || size_ <= idx) {
    throw std::out_of_range("BitsetD::set invalid index");
  }
  const std::uint32_t mask = (UINT32_C(1) << (idx % chunk_bi_s));
  if (val) {
    chunks_[idx / chunk_bi_s] |= mask;
  } else {
    chunks_[idx / chunk_bi_s] &= ~mask;
  }
}

BitsetD& BitsetD::invert() noexcept {
  for (auto& bits : chunks_) {
    bits = ~bits;
  }
  if (size_ % chunk_bi_s != 0) {
    chunks_.back() &= ((UINT32_C(1) << (size_ % chunk_bi_s)) - 1);
  }
  return *this;
}

BitsetD& BitsetD::shift(const std::int32_t shift) noexcept {
  if (shift == 0 || size_ <= 0) {
    return *this;
  }

  BitsetD copy(size_, false);

  for (std::int32_t i = 0; i < size_; ++i) {
    const std::int32_t j = i - shift;
    if (0 <= j && j < size_ && get(i)) {
      copy.set(j, true);
    }
  }

  std::swap(chunks_, copy.chunks_);
  return *this;
}

BitsetD& BitsetD::operator<<=(const std::int32_t shift) {
  if (shift <= 0) {
    return operator>>=(-shift);
  }
  if (shift >= size_) {
    std::fill(chunks_.begin(), chunks_.end(), UINT32_C(0));
    return *this;
  }

  for (std::int32_t i = size_ - 1; i >= shift; --i) {
    set(i, get(i - shift));
  }
  for (std::int32_t i = 0; i < shift; ++i) {
    set(i, false);
  }
  return *this;
}

BitsetD& BitsetD::operator>>=(const std::int32_t shift) {
  if (shift <= 0) {
    return operator<<=(-shift);
  }
  if (shift >= size_) {
    std::fill(chunks_.begin(), chunks_.end(), UINT32_C(0));
    return *this;
  }

  for (std::int32_t i = 0; i < size_ - shift; ++i) {
    set(i, get(i + shift));
  }
  for (std::int32_t i = size_ - shift; i < size_; ++i) {
    set(i, false);
  }
  return *this;
}

BitsetD& BitsetD::operator&=(const BitsetD& rhs) {
  if (size_ != rhs.size_) {
    throw std::invalid_argument("BitsetD::operator&= different size");
  }
  for (std::int32_t i = 0; i < chunks_count(); ++i) {
    chunks_[i] &= rhs.chunks_[i];
  }
  return *this;
}

BitsetD& BitsetD::operator|=(const BitsetD& rhs) {
  if (size_ != rhs.size_) {
    throw std::invalid_argument("BitsetD::operator|= different size");
  }
  for (std::int32_t i = 0; i < chunks_count(); ++i) {
    chunks_[i] |= rhs.chunks_[i];
  }
  return *this;
}

BitsetD& BitsetD::operator^=(const BitsetD& rhs) {
  if (size_ != rhs.size_) {
    throw std::invalid_argument("BitsetD::operator^= different size");
  }
  for (std::int32_t i = 0; i < chunks_count(); ++i) {
    chunks_[i] ^= rhs.chunks_[i];
  }
  return *this;
}

std::string BitsetD::to_string(const BitsetD::StrFormat fmt, const int32_t len) const {
  if (size_ <= 0) {
    return "empty";
  }

  auto bin_string = [&]() {
    std::string s;
    s.reserve(size_ + size_ / 4 + 2);
    for (std::int32_t i = size_ - 1; i >= 0; --i) {
      s.push_back(get(i) ? '1' : '0');
      if (fmt == StrFormat::Bin && i > 0 && (i % 4 == 0)) {
        s.push_back('\'');
      }
    }
    if (len > 0 && static_cast<std::int32_t>(s.size()) < len) {
      s.insert(s.begin(), len - static_cast<std::int32_t>(s.size()), '0');
    }
    if (fmt == StrFormat::Bin) {
      s.insert(0, "b0");
    }
    return s;
  };

  if (fmt == StrFormat::Bin || fmt == StrFormat::BinNoPreSep) {
    return bin_string();
  }

  std::uint64_t v = static_cast<std::uint64_t>(*this);

  if (fmt == StrFormat::Hex) {
    std::string s;
    do {
      const int d = static_cast<int>(v & 0xFu);
      s.push_back("0123456789abcdef"[d]);
      v >>= 4;
    } while (v != 0);
    while (static_cast<std::int32_t>(s.size()) < len) {
      s.push_back('0');
    }
    std::reverse(s.begin(), s.end());
    return "0x" + s;
  }

  if (fmt == StrFormat::Oct) {
    std::string s;
    do {
      s.push_back(static_cast<char>('0' + (v & 0x7u)));
      v >>= 3;
    } while (v != 0);
    while (static_cast<std::int32_t>(s.size()) < len) {
      s.push_back('0');
    }
    std::reverse(s.begin(), s.end());
    return "0o" + s;
  }

  return "empty";
}
