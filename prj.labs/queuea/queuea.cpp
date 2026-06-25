#include </Users/lenar/Desktop/OOP/misis2026s-25-04-ganeev-l-r/prj.labs/queuea/queuea.hpp>

#include <utility>

QueueA::QueueA()
    : data_(new std::int64_t[8])
    , head_(0)
    , size_(0)
    , capacity_(8)
{
}

QueueA::QueueA(const std::ptrdiff_t capacity)
    : data_(new std::int64_t[capacity > 0 ? capacity : 1])
    , head_(0)
    , size_(0)
    , capacity_(capacity > 0 ? capacity : 1)
{
}

QueueA::QueueA(const double capacity)
    : QueueA(static_cast<std::ptrdiff_t>(capacity))
{
}

QueueA::QueueA(const QueueA& src)
    : data_(new std::int64_t[src.capacity_])
    , head_(0)
    , size_(src.size_)
    , capacity_(src.capacity_)
{
    for (std::ptrdiff_t i = 0; i < src.size_; ++i) {
        const std::ptrdiff_t idx = (src.head_ + i) % src.capacity_;
        data_[i] = src.data_[idx];
    }
}

QueueA& QueueA::operator=(const QueueA& src) {
    if (this != &src) {
        if (capacity_ < src.size_) {
            delete[] data_;
            data_ = new std::int64_t[src.capacity_];
            capacity_ = src.capacity_;
        }
        head_ = 0;
        size_ = src.size_;
        for (std::ptrdiff_t i = 0; i < src.size_; ++i) {
            const std::ptrdiff_t idx = (src.head_ + i) % src.capacity_;
            data_[i] = src.data_[idx];
        }
    }
    return *this;
}

QueueA::~QueueA() noexcept {
    delete[] data_;
}

bool QueueA::is_empty() const noexcept {
    return size_ == 0;
}

void QueueA::push(const std::int64_t val) {
    if (size_ == capacity_) {
        const std::ptrdiff_t new_cap = capacity_ * 2;
        std::int64_t* new_data = new std::int64_t[new_cap];
        for (std::ptrdiff_t i = 0; i < size_; ++i) {
            const std::ptrdiff_t idx = (head_ + i) % capacity_;
            new_data[i] = data_[idx];
        }
        std::swap(data_, new_data);
        delete[] new_data;
        head_ = 0;
        capacity_ = new_cap;
    }
    const std::ptrdiff_t pos = (head_ + size_) % capacity_;
    data_[pos] = val;
    ++size_;
}

void QueueA::pop() {
    if (is_empty()) {
        throw std::invalid_argument("QueueA::pop - queue is empty");
    }
    head_ = (head_ + 1) % capacity_;
    --size_;
}

std::int64_t& QueueA::front() {
    if (is_empty()) {
        throw std::invalid_argument("QueueA::front - queue is empty");
    }
    return data_[head_];
}

std::int64_t QueueA::front() const {
    if (is_empty()) {
        throw std::invalid_argument("QueueA::front - queue is empty");
    }
    return data_[head_];
}

void QueueA::clear() noexcept {
    head_ = 0;
    size_ = 0;
}
