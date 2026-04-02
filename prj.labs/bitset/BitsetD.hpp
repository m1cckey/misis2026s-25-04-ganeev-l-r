#pragma once
#ifndef BITSETD_BITSETD_HPP_20260214
#define BITSETD_BITSETD_HPP_20260214

#include <cstdint>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

class BitsetD {
public:
  class BitR {
    friend class BitsetD;
  public:
    ~BitR() = default;
    operator bool() const noexcept { return val_; }
  private:
    BitR() = delete;
    BitR(const BitR&) = delete;
    BitR(BitR&&) = delete;
    BitR(const BitsetD& bs, const int32_t idx) : val_(bs.get(idx)) {}
    BitR& operator=(const BitR&) = delete;
    BitR& operator=(BitR&&) = delete;
  private:
    bool val_ = false;
  };

  class BitW {
    friend class BitsetD;
  public:
    ~BitW() = default;
    operator bool() const noexcept { return bs_.get(idx_); }
    void operator=(const bool val) noexcept { bs_.set(idx_, val); }
    void operator=(const BitW& rhs) noexcept { operator=(rhs.operator bool()); }
    void operator=(BitW&& rhs) noexcept { operator=(rhs.operator bool()); }
  private:
    BitW(BitsetD& bs, const int32_t idx) : bs_(bs), idx_(idx) {}
    BitW() = delete;
    BitW(const BitW&) = delete;
    BitW(BitW&&) = delete;
  private:
    BitsetD& bs_;
    const int32_t idx_ = 0;
  };

public:
  BitsetD() = default;

  BitsetD(const BitsetD& src) = default;

  BitsetD(BitsetD&& src) = default;

  BitsetD(const std::uint64_t mask, const int32_t size) : size_(size) {
    if (size <= 0) throw std::invalid_argument("BitsetD: size must be positive");
    chunks_.assign(chunks_count(), 0);
    chunks_[0] = static_cast<std::uint32_t>(mask & 0xFFFFFFFFu);
    if (chunks_.size() > 1) chunks_[1] = static_cast<std::uint32_t>(mask >> 32);
    if (chunks_.size() > 2) {
      for (std::size_t i = 2; i < chunks_.size(); ++i) chunks_[i] = 0;
    }
    clear_tail_bits();
  }

  BitsetD(const int32_t size, const bool val = false) : size_(size) {
    if (size <= 0) throw std::invalid_argument("BitsetD: size must be positive");
    chunks_.assign(chunks_count(), val ? 0xFFFFFFFFu : 0u);
    clear_tail_bits();
  }

  ~BitsetD() = default;

  BitsetD& operator=(const BitsetD& rhs) = default;
  BitsetD& operator=(BitsetD&& rhs) = default;

  explicit operator std::uint64_t() const {
    std::uint64_t val = 0;
    if (!chunks_.empty()) val |= chunks_[0];
    if (chunks_.size() > 1) val |= (static_cast<std::uint64_t>(chunks_[1]) << 32);
    return val;
  }

  explicit operator std::uint32_t() const { return chunks_.empty() ? 0u : chunks_[0]; }

  std::int32_t size() const noexcept { return size_; }

  void resize(const std::int32_t new_size, const bool val = false) {
    if (new_size <= 0) throw std::invalid_argument("BitsetD: size must be positive");
    if (new_size == size_) return;

    const int32_t old_size = size_;
    const auto old_chunks = chunks_;
    size_ = new_size;
    chunks_.assign(chunks_count(), val ? 0xFFFFFFFFu : 0u);

    const int32_t copy_bits = std::min(old_size, new_size);
    for (int32_t i = 0; i < copy_bits; ++i) {
      if ((old_chunks[i / chunk_bi_s] >> (i % chunk_bi_s)) & 1u) {
        chunks_[i / chunk_bi_s] |= (1u << (i % chunk_bi_s));
      }
    }

    if (new_size > old_size && val) {
      for (int32_t i = old_size; i < new_size; ++i) set(i, true);
    }
    clear_tail_bits();
  }

  bool get(const std::int32_t idx) const {
    check_idx(idx);
    return (chunks_[idx / chunk_bi_s] >> (idx % chunk_bi_s)) & 1u;
  }

  void set(const std::int32_t idx, const bool val) {
    check_idx(idx);
    auto &chunk = chunks_[idx / chunk_bi_s];
    const auto mask = (1u << (idx % chunk_bi_s));
    if (val) chunk |= mask;
    else chunk &= ~mask;
  }

  bool operator==(const BitsetD& rhs) const noexcept {
    return size_ == rhs.size_ && chunks_ == rhs.chunks_;
  }

  BitsetD& invert() noexcept {
    for (auto& c : chunks_) c = ~c;
    clear_tail_bits();
    return *this;
  }

  void fill(const bool val) noexcept {
    std::fill(chunks_.begin(), chunks_.end(), val ? 0xFFFFFFFFu : 0u);
    clear_tail_bits();
  }

  BitsetD& shift(const std::int32_t s) noexcept {
    if (s == 0 || size_ <= 0) return *this;
    if (s >= size_ || s <= -size_) {
      fill(false);
      return *this;
    }
    if (s > 0) {
      *this <<= s;
    } else {
      *this >>= -s;
    }
    return *this;
  }

  BitsetD& operator<<=(const std::int32_t shift) {
    if (shift <= 0) return operator>>=(-shift);
    if (shift >= size_) {
      fill(false);
      return *this;
    }
    for (int32_t i = size_ - 1; i >= shift; --i) set(i, get(i - shift));
    for (int32_t i = 0; i < shift; ++i) set(i, false);
    return *this;
  }

  BitsetD& operator>>=(const std::int32_t shift) {
    if (shift <= 0) return operator<<=(-shift);
    if (shift >= size_) {
      fill(false);
      return *this;
    }
    for (int32_t i = 0; i < size_ - shift; ++i) set(i, get(i + shift));
    for (int32_t i = size_ - shift; i < size_; ++i) set(i, false);
    return *this;
  }

  BitsetD& operator&=(const BitsetD& rhs) {
    const int32_t n = std::min(size_, rhs.size_);
    for (int32_t i = 0; i < n; ++i) set(i, get(i) & rhs.get(i));
    for (int32_t i = n; i < size_; ++i) set(i, false);
    return *this;
  }

  BitsetD& operator|=(const BitsetD& rhs) {
    const int32_t n = std::min(size_, rhs.size_);
    for (int32_t i = 0; i < n; ++i) set(i, get(i) | rhs.get(i));
    return *this;
  }

  BitsetD& operator^=(const BitsetD& rhs) {
    const int32_t n = std::min(size_, rhs.size_);
    for (int32_t i = 0; i < n; ++i) set(i, get(i) ^ rhs.get(i));
    return *this;
  }

  enum class StrFormat : int32_t {
    Bin = 1,
    BinNoPreSep,
    Oct,
    Hex,
    Def = Bin,
    Default = Def
  };

  std::string to_string(const StrFormat fmt = StrFormat::Def, const int32_t len = 0) const {
    if (size_ <= 0) return "empty";
    if (fmt == StrFormat::Bin || fmt == StrFormat::BinNoPreSep) {
      std::string s;
      for (int32_t i = size_ - 1; i >= 0; --i) {
        s.push_back(get(i) ? '1' : '0');
        if (fmt == StrFormat::Bin && i > 0 && (i % 4 == 0)) s.push_back('\'');
      }
      if (len > 0 && static_cast<int32_t>(s.size()) < len) s.insert(s.begin(), len - s.size(), '0');
      if (fmt == StrFormat::Bin) s.insert(0, "b");
      return s;
    }

    std::uint64_t v = static_cast<std::uint64_t>(*this);
    std::string s;
    if (fmt == StrFormat::Hex) {
      const char* dig = "0123456789abcdef";
      do { s.push_back(dig[v & 0xFu]); v >>= 4; } while (v);
      while (static_cast<int32_t>(s.size()) < len) s.push_back('0');
      if (s.empty()) s = "0";
      std::reverse(s.begin(), s.end());
      return "0x" + s;
    }

    if (fmt == StrFormat::Oct) {
      do { s.push_back(char('0' + (v & 0x7u))); v >>= 3; } while (v);
      while (static_cast<int32_t>(s.size()) < len) s.push_back('0');
      if (s.empty()) s = "0";
      std::reverse(s.begin(), s.end());
      return "0o" + s;
    }

    return "empty";
  }

private:
  std::int32_t                size_ = 0;
  std::vector<std::uint32_t>  chunks_;

private:
  static const int32_t chunk_bi_s = 32;
  static const int32_t chunk_by_s = 4;
  int32_t chunks_count() const { return (size_ + chunk_bi_s - 1) / chunk_bi_s; }

  void check_idx(const int32_t idx) const {
    if (idx < 0 || idx >= size_) throw std::out_of_range("BitsetD: index out of range");
  }

  void clear_tail_bits() {
    if (chunks_.empty()) return;
    const int32_t tail = size_ % chunk_bi_s;
    if (tail == 0) return;
    chunks_.back() &= ((1u << tail) - 1u);
  }
};

inline BitsetD::operator std::uint64_t() const {
  std::uint64_t val = chunk_bi_s < size_ ? chunks_[1] : 0;
  val <<= 32;
  val |= chunks_[0];
  return val;
}

inline BitsetD operator~(const BitsetD& val) noexcept {
  return BitsetD(val).invert();
}

inline BitsetD operator<<(const BitsetD& lhs, const std::int32_t shift) {
  return BitsetD(lhs).operator<<=(shift); 
}

inline BitsetD operator>>(const BitsetD& lhs, const std::int32_t shift) {
  return BitsetD(lhs).operator>>=(shift);
}

inline BitsetD operator&(const BitsetD& lhs, const BitsetD& rhs) {
  return BitsetD(lhs).operator&=(rhs);
}

inline BitsetD operator|(const BitsetD& lhs, const BitsetD& rhs) {
  return BitsetD(lhs).operator|=(rhs);
}

inline BitsetD operator^(const BitsetD& lhs, const BitsetD& rhs) {
  return BitsetD(lhs).operator^=(rhs);
}

#endif
