#pragma once

#ifndef BITSETD_BITSETD_HPP
#define BITSETD_BITSETD_HPP

#include <cstdint>
#include <string>
#include <vector>

class BitsetD {
public:
    class BitR {
        friend class BitsetD;
    public:
        ~BitR() = default;
        operator bool() const noexcept { return val_; }
        BitR(const BitR&) = default;
        BitR(BitR&&) = default;
    private:
        BitR() = delete;
        BitR(const BitsetD& bs, const std::int32_t idx) : val_(bs.get(idx)) {}
        BitR& operator=(const BitR&) = delete;
    private:
        bool val_ = false;
    };

    class BitW {
        friend class BitsetD;
    public:
        ~BitW() = default;
        operator bool() const { return bs_.get(idx_); }
        BitW(const BitW&) = default;
        BitW(BitW&&) = default;

        BitW& operator=(const bool val) noexcept {
            bs_.set(idx_, val);
            return *this;
        }

        BitW& operator=(const BitW& rhs) {
            return operator=(rhs.operator bool());
        }

        BitW& operator=(BitW&& rhs) {
            return operator=(rhs.operator bool());
        }

    private:
        BitW(BitsetD& bs, const std::int32_t idx) : bs_(bs), idx_(idx) {}
        BitW() = delete;
    private:
        BitsetD& bs_;
        const std::int32_t idx_ = 0;
    };

public:
    BitsetD() = default;
    BitsetD(const BitsetD& src) = default;
    BitsetD(BitsetD&& src) noexcept;


    BitsetD(const std::uint64_t mask, const std::int32_t size);

    explicit BitsetD(const std::int32_t size, const bool val = false);

    ~BitsetD() = default;

    BitsetD& operator=(const BitsetD& rhs) = default;
    BitsetD& operator=(BitsetD&& rhs) noexcept;

    explicit operator std::uint64_t() const;
    explicit operator std::uint32_t() const { return chunks_.empty() ? 0 : chunks_[0]; }

    std::int32_t size() const noexcept { return size_; }
    void resize(const std::int32_t new_size, const bool val = false);

    bool get(const std::int32_t idx) const;
    void set(const std::int32_t idx, const bool val);

    BitW operator[](const std::int32_t idx) { return BitW(*this, idx); }
    BitR operator[](const std::int32_t idx) const { return BitR(*this, idx); }

    bool operator==(const BitsetD& rhs) const noexcept;
    bool operator!=(const BitsetD& rhs) const noexcept { return !operator==(rhs); }

    BitsetD& invert() noexcept;
    void fill(const bool val) noexcept;
    BitsetD& shift(const std::int32_t s) noexcept;

    BitsetD& operator<<=(const std::int32_t shift);
    BitsetD& operator>>=(const std::int32_t shift);
    BitsetD& operator&=(const BitsetD& rhs);
    BitsetD& operator|=(const BitsetD& rhs);
    BitsetD& operator^=(const BitsetD& rhs);

    enum class StrFormat : std::int32_t {
        Bin = 1,
        BinNoPreSep,
        Oct,
        Hex,
        Def = Bin,
        Default = Def
    };

    std::string to_string(const StrFormat fmt = StrFormat::Def, const std::int32_t len = 0) const;

private:
    std::int32_t                size_ = 0;// знаковый,чтобы можно было обрабатывать ошибки
    std::vector<std::uint32_t>  chunks_;

    static const std::int32_t chunk_bi_s = 32;
    static const std::int32_t chunk_by_s = 4;

    std::int32_t chunks_count() const {
        return (size_ + chunk_bi_s - 1) / chunk_bi_s;
    }

    void clean_tail() noexcept;
};


inline BitsetD::operator std::uint64_t() const {
    if (size_ == 0) return 0;
    std::uint64_t val = (size_ > 32 && chunks_.size() > 1) ? chunks_[1] : 0;
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
