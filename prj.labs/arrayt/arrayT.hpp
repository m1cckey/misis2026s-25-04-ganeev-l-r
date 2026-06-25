#pragma once
#ifndef ARRAYT_ARRAYT_HPP
#define ARRAYT_ARRAYT_HPP

#include <cstddef>
#include <algorithm>
#include <stdexcept>
#include <utility>

template<
    typename T,                 
    typename S = std::ptrdiff_t 
>
class ArrayT {
public:
    ArrayT() = default;

    ArrayT(const ArrayT&);

    //! \param size - начальный размер
    ArrayT(const S size);

    ~ArrayT();

    ArrayT& operator=(const ArrayT&);

    [[nodiscard]] S size() const noexcept { return size_; }

    //! \param size - новый размер
    void resize(const S size);

    //! \param idx - индекс  элемента
    [[nodiscard]] T& operator[](const S idx);
    [[nodiscard]] T operator[](const S idx) const;

    //! \param idx - индекс вставляемого элемента
    void insert(const S idx, const T val);

    //! \param idx - индекс удаляемого элемента
    void remove(const S idx);

private:
    S capacity_ = 0;  
    S size_ = 0;      
    T* data_ = nullptr;
};

template<typename T, typename S>
ArrayT<T, S>::ArrayT(const ArrayT& src)
    : capacity_(src.size_)
    , size_(capacity_)
    , data_(size_ > 0 ? new T[size_] : nullptr) {
    for (S i = 0; i < size_; ++i) {
        data_[i] = src.data_[i];
    }
}

template<typename T, typename S>
ArrayT<T, S>::ArrayT(const S size)
    : capacity_(size)
    , size_(size) {
    if (size_ <= 0) {
        throw std::invalid_argument("ArrayT::ArrayT - non positive size");
    }
    data_ = new T[capacity_]{};
}

template<typename T, typename S>
ArrayT<T, S>::~ArrayT() {
    delete[] data_;
}

template<typename T, typename S>
ArrayT<T, S>& ArrayT<T, S>::operator=(const ArrayT& rhs) {
    if (this != &rhs) {
        T* new_data = nullptr;
        if (rhs.size_ > 0) {
            new_data = new T[rhs.size_];
            for (S i = 0; i < rhs.size_; ++i) {
                new_data[i] = rhs.data_[i];
            }
        }
        delete[] data_;
        data_ = new_data;
        size_ = rhs.size_;
        capacity_ = rhs.size_;
    }
    return *this;
}

template<typename T, typename S>
void ArrayT<T, S>::resize(const S size) {
    if (size < 0) {
        throw std::invalid_argument("ArrayT::resize - non positive size");
    }
    if (capacity_ < size) {
        S new_capacity = (capacity_ > 0) ? capacity_ * 2 : size;
        if (new_capacity < size) new_capacity = size;
        T* new_data = new T[new_capacity]{};
        for (S i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }
    else {
        if (size_ < size) {
            std::fill(data_ + size_, data_ + size, T{});
        }
    }
    size_ = size;
}

template<typename T, typename S>
T& ArrayT<T, S>::operator[](const S idx) {
    if (idx < 0 || size_ <= idx) {
        throw std::invalid_argument("ArrayT::operator[] - invalid index");
    }
    return *(data_ + idx);
}

template<typename T, typename S>
T ArrayT<T, S>::operator[](const S idx) const {
    if (idx < 0 || size_ <= idx) {
        throw std::invalid_argument("ArrayT::operator[] - invalid index");
    }
    return data_[idx];
}

template<typename T, typename S>
void ArrayT<T, S>::insert(const S idx, const T val) {
    if (idx < 0 || size_ < idx) {
        throw std::invalid_argument("ArrayT::Insert - invalid index");
    }
    resize(size_ + 1);
    for (S i = size_ - 1; i > idx; --i) {
        data_[i] = std::move(data_[i - 1]);
    }
    data_[idx] = val;
}

template<typename T, typename S>
void ArrayT<T, S>::remove(const S idx) {
    if (idx < 0 || size_ <= idx) {
        throw std::invalid_argument("ArrayT::operator[] - invalid index");
    }
    for (S i = idx; i < size_ - 1; ++i) {
        data_[i] = std::move(data_[i + 1]);
    }
    resize(size_ - 1);
}

#endif
