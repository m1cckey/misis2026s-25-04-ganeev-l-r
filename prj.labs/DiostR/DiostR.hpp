// DiostRB.hpp
#pragma once

#include <string>
#include <iostream>

class DiostRB {
public:
    using byte_type = unsigned char;

    DiostRB() = default;

  
    explicit DiostRB(const std::string& data) : str_(data) {}

    explicit DiostRB(const std::vector<byte_type>& data)
        : str_(reinterpret_cast<const char*>(data.data()), data.size())
    {}

    DiostRB(const std::vector<char>& data)
        : str_(data.data(), data.size())
    {}

    DiostRB(const DiostRB& other) = default;
    DiostRB(DiostRB&& other) noexcept = default;
    DiostRB& operator=(const DiostRB& other) = default;
    DiostRB& operator=(DiostRB&& other) noexcept = default;

    DiostRB& operator=(const std::string& data) {
        str_ = data;
        return *this;
    }
    DiostRB& operator=(const std::vector<byte_type>& data) {
        str_.assign(reinterpret_cast<const char*>(data.data()), data.size());
        return *this;
    }

    const std::string& str() const { return str_; }

private:
    std::string str_;
};



// read
inline std::istream& operator>>(std::istream& is, DiostRB& obj) {
    size_t size = 0;
    is.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (is) {
        obj.str_.resize(size);
        if (size > 0) {
            is.read(const_cast<char*>(obj.str_.data()), size);
        }
    }
    return is;
}

// write
inline std::ostream& operator<<(std::ostream& os, const DiostRB& obj) {
    const std::string& s = obj.str();
    size_t size = s.size();

    os.write(reinterpret_cast<const char*>(&size), sizeof(size));
    if (!s.empty()) {
        os.write(s.data(), size);
    }
    return os;
}