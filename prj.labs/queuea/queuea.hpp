#pragma once
#ifndef QUEUEA_QUEUEA_HPP
#define QUEUEA_QUEUEA_HPP

#include <cstdint>
#include <stdexcept>

class QueueA {
public:
    QueueA();
    explicit QueueA(const std::ptrdiff_t capacity);
    QueueA(const double capacity);
    QueueA(const int capacity) : QueueA(static_cast<std::ptrdiff_t>(capacity)) {}
    QueueA(const unsigned int capacity) : QueueA(static_cast<std::ptrdiff_t>(capacity)) {}
    QueueA(const QueueA& src);
    QueueA& operator=(const QueueA& src);
    ~QueueA() noexcept;

    [[nodiscard]] bool is_empty() const noexcept;

    void push(const std::int64_t val);
    void push(const double val) { push(static_cast<std::int64_t>(val)); }
    void push(const int val) { push(static_cast<std::int64_t>(val)); }
    void push(const unsigned int val) { push(static_cast<std::int64_t>(val)); }
    void pop();

    [[nodiscard]] std::int64_t& front();
    [[nodiscard]] std::int64_t front() const;


    [[nodiscard]] std::int64_t& top() { return front(); }
    [[nodiscard]] std::int64_t top() const { return front(); }

    void clear() noexcept;

private:
    std::int64_t* data_ = nullptr;
    std::ptrdiff_t head_ = 0;
    std::ptrdiff_t size_ = 0;
    std::ptrdiff_t capacity_ = 0;
};

#endif
