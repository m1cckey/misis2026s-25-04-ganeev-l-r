// DiostRB.hpp
#pragma once

#include <string>
#include <iostream>
#include <vector>      // Добавлен недостающий заголовок
#include <cstring>     // Для size_t (хотя он обычно доступен через другие заголовки)

class DiostRB {
public:
    using byte_type = unsigned char;

    DiostRB() = default;

    explicit DiostRB(const std::string& data) : str_(data) {}

    explicit DiostRB(const std::vector<byte_type>& data)
        : str_(reinterpret_cast<const char*>(data.data()), data.size())
    {}

    explicit DiostRB(const std::vector<char>& data)  // Добавлен explicit для консистентности
        : str_(data.data(), data.size())
    {}

    // Правила пяти
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

    DiostRB& operator=(const std::vector<char>& data) {  // Добавлен оператор присваивания
        str_.assign(data.data(), data.size());
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
        // Используем временную строку для безопасного чтения
        std::string temp;
        temp.resize(size);
        if (size > 0) {
            is.read(temp.data(), static_cast<std::streamsize>(size));  // Убрано const_cast
        }
        if (is) {
            obj = temp;  // Присваиваем только при успешном чтении
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
        os.write(s.data(), static_cast<std::streamsize>(size));
    }
    return os;
}