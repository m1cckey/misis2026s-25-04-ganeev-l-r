#include "BitsetD.hpp"
#include <algorithm>
#include <stdexcept>

static void mask_unused(int32_t size, std::vector<uint32_t>& chunks) {
    if (size > 0 && size % 32 != 0) {
        chunks.back() &= (1U << (size % 32)) - 1;
    }
}

BitsetD::BitsetD(const std::uint64_t mask, const int32_t size) : size_(size) {
    chunks_.resize(chunks_count(), 0);
    if (size > 0) {
        chunks_[0] = static_cast<std::uint32_t>(mask & 0xFFFFFFFF);
        if (size > 32) {
            chunks_[1] = static_cast<std::uint32_t>(mask >> 32);
        }
    }
    mask_unused(size_, chunks_);
}

BitsetD::BitsetD(const int32_t size, const bool val) : size_(size) {
    chunks_.resize(chunks_count(), val ? 0xFFFFFFFF : 0);
    mask_unused(size_, chunks_);
}

void BitsetD::resize(const std::int32_t new_size, const bool val) {
    int32_t old_size = size_;
    int32_t new_chunks_count = (new_size + 31) / 32;

    if (val && new_size > old_size && old_size % 32 != 0) {
        chunks_[old_size / 32] |= ~((1U << (old_size % 32)) - 1);
    }

    chunks_.resize(new_chunks_count, val ? 0xFFFFFFFF : 0);
    size_ = new_size;
    mask_unused(size_, chunks_);
}

bool BitsetD::get(const std::int32_t idx) const {
    if (idx < 0 || idx >= size_) throw std::out_of_range("BitsetD: index out of range");
    return (chunks_[idx / 32] & (1U << (idx % 32))) != 0;
}

void BitsetD::set(const std::int32_t idx, const bool val) {
    if (idx < 0 || idx >= size_) throw std::out_of_range("BitsetD: index out of range");
    if (val) chunks_[idx / 32] |= (1U << (idx % 32));
    else     chunks_[idx / 32] &= ~(1U << (idx % 32));
}

bool BitsetD::operator==(const BitsetD& rhs) const noexcept {
    return size_ == rhs.size_ && chunks_ == rhs.chunks_;
}

BitsetD& BitsetD::invert() noexcept {
    for (auto& block : chunks_) block = ~block;
    mask_unused(size_, chunks_);
    return *this;
}

void BitsetD::fill(const bool val) noexcept {
    std::fill(chunks_.begin(), chunks_.end(), val ? 0xFFFFFFFF : 0);
    mask_unused(size_, chunks_);
}

BitsetD& BitsetD::shift(const std::int32_t s) noexcept {
    if (size_ <= 1) return *this;
    int32_t real_shift = (s % size_ + size_) % size_;
    if (real_shift == 0) return *this;

    BitsetD temp(*this);
    for (int32_t i = 0; i < size_; ++i) {
        this->set((i + real_shift) % size_, temp.get(i));
    }
    return *this;
}

BitsetD& BitsetD::operator<<=(const std::int32_t shift) {
    if (shift <= 0) return *this;
    for (int32_t i = size_ - 1; i >= 0; --i) {
        set(i, (i >= shift) ? get(i - shift) : false);
    }
    return *this;
}

BitsetD& BitsetD::operator>>=(const std::int32_t shift) {
    if (shift <= 0) return *this;
    for (int32_t i = 0; i < size_; ++i) {
        set(i, (i + shift < size_) ? get(i + shift) : false);
    }
    return *this;
}

BitsetD& BitsetD::operator&=(const BitsetD& rhs) {
    size_t n = std::min(chunks_.size(), rhs.chunks_.size());
    for (size_t i = 0; i < n; ++i) chunks_[i] &= rhs.chunks_[i];
    return *this;
}

BitsetD& BitsetD::operator|=(const BitsetD& rhs) {
    size_t n = std::min(chunks_.size(), rhs.chunks_.size());
    for (size_t i = 0; i < n; ++i) chunks_[i] |= rhs.chunks_[i];
    return *this;
}

BitsetD& BitsetD::operator^=(const BitsetD& rhs) {
    size_t n = std::min(chunks_.size(), rhs.chunks_.size());
    for (size_t i = 0; i < n; ++i) chunks_[i] ^= rhs.chunks_[i];
    return *this;
}

std::string BitsetD::to_string(const StrFormat fmt, const int32_t len) const {
    return "";
}

void BitsetD::write_to(std::ostream& out) const {
    out.put('b');
    
    out.put(static_cast<char>(size_));

    std::uint8_t current_byte = 0;
    int bit_count = 0;
    std::uint8_t hash = 0;
    
    for (int32_t i = 0; i < size_; ++i) {
        if (get(i)) {
            current_byte |= (1 << bit_count);
        }
        bit_count++;
        
        if (bit_count == 8 || i == size_ - 1) {
            out.put(static_cast<char>(current_byte));
        }
      }