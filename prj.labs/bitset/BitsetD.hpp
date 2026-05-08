#pragma once
#ifndef BITSETD_BITSETD_HPP_20260214
#define BITSETD_BITSETD_HPP_20260214

#include <cstdint>
#include <string>
#include <vector>
#include <istream>
#include <ostream>

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

    BitsetD(const std::uint64_t mask, const int32_t size);

    BitsetD(const int32_t size, const bool val = false);

    ~BitsetD() = default;

    BitsetD& operator=(const BitsetD& rhs) = default;

    BitsetD& operator=(BitsetD&& rhs) = default;

    explicit operator std::uint64_t() const;

    explicit operator std::uint32_t() const { return chunks_[0]; }

    std::int32_t size() const noexcept { return size_; }

    void resize(const std::int32_t new_size, const bool val = false);

    bool get(const std::int32_t idx) const;

    void set(const std::int32_t idx, const bool val);

    BitW operator[](const std::int32_t idx)& { return BitW(*this, idx); }

    BitR operator[](const std::int32_t idx) const& { return BitR(*this, idx); }

    bool operator==(const BitsetD& rhs) const noexcept;

    BitsetD& invert() noexcept;

    void fill(const bool val) noexcept;

    BitsetD& shift(const std::int32_t s) noexcept;

    BitsetD& operator<<=(const std::int32_t shift);

    BitsetD& operator>>=(const std::int32_t shift);

    BitsetD& operator&=(const BitsetD& rhs);

    BitsetD& operator|=(const BitsetD& rhs);

    BitsetD& operator^=(const BitsetD& rhs);

    enum class StrFormat : int32_t {
        Bin = 1,    
        BinNoPreSep, 
        Oct,
        Hex,
        Def = Bin,
        Default = Def
    };

    
    std::string to_string(const StrFormat fmt = StrFormat::Def, const int32_t len = 0) const;
    void write_to(std::ostream& out) const;
    void read_from(std::istream& in);

private:
    std::int32_t                size_ = 0;
    std::vector<std::uint32_t>  chunks_;   
private:
    static const int32_t chunk_bi_s = 32;
    static const int32_t chunk_by_s = 4;
    int32_t chunks_count() const { return (size_ + chunk_bi_s - 1) / chunk_bi_s; }
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