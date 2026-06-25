// 2025 by Sugrobov A.A. under Unlicense

#include <jaggeda/jaggeda.hpp>

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <utility>

// RowProxy implementation

JaggedA::RowProxy::RowProxy(std::int32_t* row, std::size_t size) noexcept
    : row_(row)
    , size_(size) {
}

std::int32_t& JaggedA::RowProxy::operator[](std::size_t col) noexcept {
    return row_[col];
}

const std::int32_t& JaggedA::RowProxy::operator[](std::size_t col) const noexcept {
    return row_[col];
}

std::size_t JaggedA::RowProxy::size() const noexcept {
    return size_;
}

// JaggedA implementation

JaggedA::~JaggedA() {
    clear();
}

JaggedA::JaggedA(std::size_t rows, std::size_t default_row_size)
    : data_(rows > 0 ? new std::int32_t*[rows] : nullptr)
    , row_sizes_(rows > 0 ? new std::size_t[rows]() : nullptr)
    , row_count_(rows)
    , row_capacity_(rows) {
    if (rows == 0) {
        return;
    }
    std::size_t i = 0;
    try {
        for (; i < rows; ++i) {
            if (default_row_size > 0) {
                data_[i] = new std::int32_t[default_row_size]();
            }
            else {
                data_[i] = nullptr;
            }
            row_sizes_[i] = default_row_size;
        }
    }
    catch (...) {
        for (std::size_t j = 0; j < i; ++j) {
            delete[] data_[j];
        }
        delete[] data_;
        delete[] row_sizes_;
        data_ = nullptr;
        row_sizes_ = nullptr;
        row_count_ = 0;
        row_capacity_ = 0;
        throw;
    }
}

JaggedA::JaggedA(std::initializer_list<std::initializer_list<std::int32_t>> init)
    : data_(init.size() > 0 ? new std::int32_t*[init.size()] : nullptr)
    , row_sizes_(init.size() > 0 ? new std::size_t[init.size()]() : nullptr)
    , row_count_(init.size())
    , row_capacity_(init.size()) {
    if (init.size() == 0) {
        return;
    }
    std::size_t i = 0;
    try {
        for (const auto& row : init) {
            row_sizes_[i] = row.size();
            if (row_sizes_[i] > 0) {
                data_[i] = new std::int32_t[row_sizes_[i]];
                std::size_t j = 0;
                for (const auto& val : row) {
                    data_[i][j++] = val;
                }
            }
            else {
                data_[i] = nullptr;
            }
            ++i;
        }
    }
    catch (...) {
        for (std::size_t j = 0; j < i; ++j) {
            delete[] data_[j];
        }
        delete[] data_;
        delete[] row_sizes_;
        data_ = nullptr;
        row_sizes_ = nullptr;
        row_count_ = 0;
        row_capacity_ = 0;
        throw;
    }
}

JaggedA::JaggedA(const JaggedA& src)
    : data_(src.row_count_ > 0 ? new std::int32_t*[src.row_count_] : nullptr)
    , row_sizes_(src.row_count_ > 0 ? new std::size_t[src.row_count_] : nullptr)
    , row_count_(src.row_count_)
    , row_capacity_(src.row_count_) {
    std::size_t i = 0;
    try {
        for (; i < row_count_; ++i) {
            row_sizes_[i] = src.row_sizes_[i];
            if (row_sizes_[i] > 0) {
                data_[i] = new std::int32_t[row_sizes_[i]];
                std::memcpy(data_[i], src.data_[i], row_sizes_[i] * sizeof(std::int32_t));
            }
            else {
                data_[i] = nullptr;
            }
        }
    }
    catch (...) {
        for (std::size_t j = 0; j < i; ++j) {
            delete[] data_[j];
        }
        delete[] data_;
        delete[] row_sizes_;
        data_ = nullptr;
        row_sizes_ = nullptr;
        row_count_ = 0;
        row_capacity_ = 0;
        throw;
    }
}

JaggedA& JaggedA::operator=(const JaggedA& rhs) {
    if (this != &rhs) {
        JaggedA tmp(rhs);
        swap(tmp);
    }
    return *this;
}

JaggedA::JaggedA(JaggedA&& other) noexcept
    : data_(other.data_)
    , row_sizes_(other.row_sizes_)
    , row_count_(other.row_count_)
    , row_capacity_(other.row_capacity_) {
    other.data_ = nullptr;
    other.row_sizes_ = nullptr;
    other.row_count_ = 0;
    other.row_capacity_ = 0;
}

JaggedA& JaggedA::operator=(JaggedA&& other) noexcept {
    if (this != &other) {
        clear();
        delete[] data_;
        delete[] row_sizes_;
        data_ = other.data_;
        row_sizes_ = other.row_sizes_;
        row_count_ = other.row_count_;
        row_capacity_ = other.row_capacity_;
        other.data_ = nullptr;
        other.row_sizes_ = nullptr;
        other.row_count_ = 0;
        other.row_capacity_ = 0;
    }
    return *this;
}

std::size_t JaggedA::size() const noexcept {
    return row_count_;
}

std::size_t JaggedA::size(std::size_t row) const {
    if (row >= row_count_) {
        throw std::invalid_argument("JaggedA::size - row out of range");
    }
    return row_sizes_[row];
}

std::size_t JaggedA::row_size(std::size_t row) const {
    if (row >= row_count_) {
        throw std::invalid_argument("JaggedA::row_size - row out of range");
    }
    return row_sizes_[row];
}

bool JaggedA::empty() const noexcept {
    return row_count_ == 0;
}

void JaggedA::resize_rows(std::size_t new_row_count) {
    if (new_row_count == row_count_) {
        return;
    }
    if (new_row_count < row_count_) {
        for (std::size_t i = new_row_count; i < row_count_; ++i) {
            delete[] data_[i];
        }
        row_count_ = new_row_count;
        if (new_row_count == 0) {
            delete[] data_;
            delete[] row_sizes_;
            data_ = nullptr;
            row_sizes_ = nullptr;
            row_capacity_ = 0;
        }
        return;
    }
    if (new_row_count > row_capacity_) {
        std::size_t new_cap = new_row_count;
        auto new_data = new std::int32_t*[new_cap];
        auto new_sizes = new std::size_t[new_cap];
        for (std::size_t i = 0; i < row_count_; ++i) {
            new_data[i] = data_[i];
            new_sizes[i] = row_sizes_[i];
        }
        for (std::size_t i = row_count_; i < new_row_count; ++i) {
            new_data[i] = nullptr;
            new_sizes[i] = 0;
        }
        delete[] data_;
        delete[] row_sizes_;
        data_ = new_data;
        row_sizes_ = new_sizes;
        row_capacity_ = new_cap;
    }
    else {
        for (std::size_t i = row_count_; i < new_row_count; ++i) {
            data_[i] = nullptr;
            row_sizes_[i] = 0;
        }
    }
    row_count_ = new_row_count;
}

void JaggedA::resize_row(std::size_t row, std::size_t new_size) {
    if (row >= row_count_) {
        throw std::invalid_argument("JaggedA::resize_row - row out of range");
    }
    if (new_size == row_sizes_[row]) {
        return;
    }
    if (new_size == 0) {
        delete[] data_[row];
        data_[row] = nullptr;
        row_sizes_[row] = 0;
        return;
    }
    auto new_row = new std::int32_t[new_size];
    std::size_t copy_count = std::min(new_size, row_sizes_[row]);
    if (copy_count > 0 && data_[row] != nullptr) {
        std::memcpy(new_row, data_[row], copy_count * sizeof(std::int32_t));
    }
    if (new_size > row_sizes_[row]) {
        std::memset(new_row + copy_count, 0, (new_size - copy_count) * sizeof(std::int32_t));
    }
    delete[] data_[row];
    data_[row] = new_row;
    row_sizes_[row] = new_size;
}

void JaggedA::clear() {
    for (std::size_t i = 0; i < row_count_; ++i) {
        delete[] data_[i];
    }
    delete[] data_;
    delete[] row_sizes_;
    data_ = nullptr;
    row_sizes_ = nullptr;
    row_count_ = 0;
    row_capacity_ = 0;
}

void JaggedA::push_back_row(std::size_t row_size) {
    if (row_count_ == row_capacity_) {
        std::size_t new_cap = (row_capacity_ == 0) ? 1 : row_capacity_ * 2;
        auto new_data = new std::int32_t*[new_cap];
        auto new_sizes = new std::size_t[new_cap];
        for (std::size_t i = 0; i < row_count_; ++i) {
            new_data[i] = data_[i];
            new_sizes[i] = row_sizes_[i];
        }
        delete[] data_;
        delete[] row_sizes_;
        data_ = new_data;
        row_sizes_ = new_sizes;
        row_capacity_ = new_cap;
    }
    if (row_size > 0) {
        data_[row_count_] = new std::int32_t[row_size]();
    }
    else {
        data_[row_count_] = nullptr;
    }
    row_sizes_[row_count_] = row_size;
    ++row_count_;
}

void JaggedA::pop_back_row() {
    if (row_count_ == 0) {
        throw std::out_of_range("JaggedA::pop_back_row - empty");
    }
    delete[] data_[row_count_ - 1];
    data_[row_count_ - 1] = nullptr;
    row_sizes_[row_count_ - 1] = 0;
    --row_count_;
}

void JaggedA::insert_row(std::size_t pos, std::size_t row_size) {
    if (pos > row_count_) {
        throw std::invalid_argument("JaggedA::insert_row - pos out of range");
    }
    if (row_count_ == row_capacity_) {
        std::size_t new_cap = (row_capacity_ == 0) ? 1 : row_capacity_ * 2;
        auto new_data = new std::int32_t*[new_cap];
        auto new_sizes = new std::size_t[new_cap];
        for (std::size_t i = 0; i < row_count_; ++i) {
            new_data[i] = data_[i];
            new_sizes[i] = row_sizes_[i];
        }
        delete[] data_;
        delete[] row_sizes_;
        data_ = new_data;
        row_sizes_ = new_sizes;
        row_capacity_ = new_cap;
    }
    std::int32_t* new_row = nullptr;
    if (row_size > 0) {
        new_row = new std::int32_t[row_size]();
    }
    for (std::size_t i = row_count_; i > pos; --i) {
        data_[i] = data_[i - 1];
        row_sizes_[i] = row_sizes_[i - 1];
    }
    data_[pos] = new_row;
    row_sizes_[pos] = row_size;
    ++row_count_;
}

void JaggedA::erase_row(std::size_t pos) {
    if (pos >= row_count_) {
        throw std::invalid_argument("JaggedA::erase_row - pos out of range");
    }
    delete[] data_[pos];
    for (std::size_t i = pos; i < row_count_ - 1; ++i) {
        data_[i] = data_[i + 1];
        row_sizes_[i] = row_sizes_[i + 1];
    }
    --row_count_;
    data_[row_count_] = nullptr;
    row_sizes_[row_count_] = 0;
}

JaggedA::RowProxy JaggedA::operator[](std::size_t row) {
    if (row >= row_count_) {
        throw std::invalid_argument("JaggedA::operator[] - row out of range");
    }
    return RowProxy(data_[row], row_sizes_[row]);
}

const JaggedA::RowProxy JaggedA::operator[](std::size_t row) const {
    if (row >= row_count_) {
        throw std::invalid_argument("JaggedA::operator[] - row out of range");
    }
    return RowProxy(data_[row], row_sizes_[row]);
}

std::int32_t& JaggedA::at(std::size_t row, std::size_t col) {
    if (row >= row_count_) {
        throw std::out_of_range("JaggedA::at - row out of range");
    }
    if (col >= row_sizes_[row]) {
        throw std::out_of_range("JaggedA::at - col out of range");
    }
    return data_[row][col];
}

const std::int32_t& JaggedA::at(std::size_t row, std::size_t col) const {
    if (row >= row_count_) {
        throw std::out_of_range("JaggedA::at - row out of range");
    }
    if (col >= row_sizes_[row]) {
        throw std::out_of_range("JaggedA::at - col out of range");
    }
    return data_[row][col];
}

std::int32_t** JaggedA::data() noexcept {
    return data_;
}

const std::int32_t* const* JaggedA::data() const noexcept {
    return reinterpret_cast<const std::int32_t* const*>(data_);
}

void JaggedA::swap(JaggedA& other) noexcept {
    using std::swap;
    swap(data_, other.data_);
    swap(row_sizes_, other.row_sizes_);
    swap(row_count_, other.row_count_);
    swap(row_capacity_, other.row_capacity_);
}

bool JaggedA::operator==(const JaggedA& other) const {
    if (row_count_ != other.row_count_) {
        return false;
    }
    for (std::size_t i = 0; i < row_count_; ++i) {
        if (row_sizes_[i] != other.row_sizes_[i]) {
            return false;
        }
        for (std::size_t j = 0; j < row_sizes_[i]; ++j) {
            if (data_[i][j] != other.data_[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool JaggedA::operator!=(const JaggedA& other) const {
    return !(*this == other);
}
