#include "bitsetd.hpp"
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <iomanip>

void BitsetD::clean_tail() noexcept {
    if (size_ <= 0 || chunks_.empty()) return;
    int32_t bits_in_last = size_ % chunk_bi_s;
    if (bits_in_last != 0) {
        uint32_t mask = (1U << bits_in_last) - 1;
        chunks_.back() &= mask;
    }
}

BitsetD::BitsetD(const std::uint64_t mask, const std::int32_t size)
    : size_(size)
{
    if (size_ < 0) throw std::invalid_argument("BitsetD: size must be non-negative");
    chunks_.resize(chunks_count(), 0);
    if (size_ > 0) {
        chunks_[0] = static_cast<uint32_t>(mask);
        if (size_ > 32) {
            chunks_[1] = static_cast<uint32_t>(mask >> 32);
        }
    }
    clean_tail();
}

BitsetD::BitsetD(const std::int32_t size, const bool val)
    : size_(size)
    , chunks_(chunks_count(), val ? 0xFFFFFFFFU : 0)
{
    if (size_ < 0) throw std::invalid_argument("BitsetD: size must be non-negative");
    clean_tail();
}

void BitsetD::resize(const std::int32_t new_size, const bool val) {
    if (new_size < 0) throw std::invalid_argument("BitsetD::resize: negative size");

    int32_t old_size = size_;
    size_ = new_size;
    int32_t new_count = chunks_count();

    if (new_size > old_size) {
        chunks_.resize(new_count, val ? 0xFFFFFFFFU : 0);
        if (val && (old_size % chunk_bi_s != 0)) {
            int32_t first_new_bit = old_size;
            int32_t end_of_chunk = (old_size / chunk_bi_s + 1) * chunk_bi_s;
            for (int32_t i = first_new_bit; i < std::min(new_size, end_of_chunk); ++i) {
                set(i, true);
            }
        }
    }
    else {
        chunks_.resize(new_count);
    }
    clean_tail();
}

bool BitsetD::get(const std::int32_t idx) const {
    if (idx < 0 || idx >= size_) throw std::out_of_range("BitsetD::get: index out of range");
    return (chunks_[idx / chunk_bi_s] >> (idx % chunk_bi_s)) & 1U;
}

void BitsetD::set(const std::int32_t idx, const bool val) {
    if (idx < 0 || idx >= size_) throw std::out_of_range("BitsetD::set: index out of range");
    if (val) {
        chunks_[idx / chunk_bi_s] |= (1U << (idx % chunk_bi_s));
    }
    else {
        chunks_[idx / chunk_bi_s] &= ~(1U << (idx % chunk_bi_s));
    }
}

bool BitsetD::operator==(const BitsetD& rhs) const noexcept {
    if (size_ != rhs.size_) return false;
    // ��������� clean_tail() �� ����� ������ �������� �������
    return chunks_ == rhs.chunks_;
}

BitsetD& BitsetD::invert() noexcept {
    for (auto& c : chunks_) {
        c = ~c;
    }
    clean_tail();
    return *this;
}

void BitsetD::fill(const bool val) noexcept {
    std::fill(chunks_.begin(), chunks_.end(), val ? 0xFFFFFFFFU : 0);
    clean_tail();
}

BitsetD& BitsetD::operator<<=(const std::int32_t shift) {
    if (shift < 0) return operator>>=(-shift);
    if (shift >= size_) { fill(false); return *this; }
    if (shift == 0) return *this;

    // ���� � ����� � ������, ����� �� �������� ������
    for (int32_t i = size_ - 1; i >= shift; --i) {
        set(i, get(i - shift));
    }
    for (int32_t i = 0; i < shift; ++i) {
        set(i, false);
    }
    return *this;
}

BitsetD& BitsetD::operator>>=(const std::int32_t shift) {
    if (shift < 0) return operator<<=(-shift);
    if (shift >= size_) { fill(false); return *this; }
    if (shift == 0) return *this;

    // ���� � ������ � �����
    for (int32_t i = 0; i < size_ - shift; ++i) {
        set(i, get(i + shift));
    }
    for (int32_t i = size_ - shift; i < size_; ++i) {
        set(i, false);
    }
    return *this;
}

BitsetD& BitsetD::shift(const std::int32_t s) noexcept {
    if (size_ <= 1) return *this;
    int32_t actual_shift = ((s % size_) + size_) % size_;
    if (actual_shift == 0) return *this;

    BitsetD temp(*this);
    for (int32_t i = 0; i < size_; ++i) {
        set((i + actual_shift) % size_, temp.get(i));
    }
    return *this;
}

BitsetD& BitsetD::operator&=(const BitsetD& rhs) {
    if (size_ != rhs.size_) throw std::invalid_argument("BitsetD: size mismatch");
    for (size_t i = 0; i < chunks_.size(); ++i) {
        chunks_[i] &= rhs.chunks_[i];
    }
    return *this;
}

BitsetD& BitsetD::operator|=(const BitsetD& rhs) {
    if (size_ != rhs.size_) throw std::invalid_argument("BitsetD: size mismatch");
    for (size_t i = 0; i < chunks_.size(); ++i) {
        chunks_[i] |= rhs.chunks_[i];
    }
    return *this;
}

BitsetD& BitsetD::operator^=(const BitsetD& rhs) {
    if (size_ != rhs.size_) throw std::invalid_argument("BitsetD: size mismatch");
    for (size_t i = 0; i < chunks_.size(); ++i) {
        chunks_[i] ^= rhs.chunks_[i];
    }
    return *this;
}

BitsetD::BitsetD(BitsetD&& src) noexcept
    : size_(src.size_)
    , chunks_(std::move(src.chunks_)) {
    src.size_ = 0;
}

BitsetD& BitsetD::operator=(BitsetD&& rhs) noexcept {
    if (this != &rhs) {
        size_ = rhs.size_;
        chunks_ = std::move(rhs.chunks_);
        rhs.size_ = 0;
    }
    return *this;
}

std::string BitsetD::to_string(const StrFormat fmt, const int32_t len) const {
    if (size_ == 0) return "empty";

    std::string res;
    if (fmt == StrFormat::Bin || fmt == StrFormat::BinNoPreSep) {
        for (int32_t i = size_ - 1; i >= 0; --i) {
            res += get(i) ? '1' : '0';
            // ��������� ����������� ������ 4 ���� (����� ������ ������)
            if (fmt == StrFormat::Bin && i > 0 && i % 4 == 0) {
                res += '\'';
            }
        }
        if (fmt == StrFormat::Bin) res = "b0" + res;
    }
    else if (fmt == StrFormat::Hex) {
        std::stringstream ss;
        ss << std::hex << std::uppercase << static_cast<uint64_t>(*this);
        res = ss.str();
    }
    else if (fmt == StrFormat::Oct) {
        std::stringstream ss;
        ss << std::oct << static_cast<uint64_t>(*this);
        res = ss.str();
    }

    // ��������� ������ �� ����������� ����� len, ���� �����
    if (static_cast<int32_t>(res.length()) < len) {
        res = std::string(len - res.length(), '0') + res;
    }
    return res;
}
