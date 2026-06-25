#pragma once
#ifndef QUEUEL_QUEUEL_HPP
#define QUEUEL_QUEUEL_HPP

#include <cstdint>
#include <stdexcept>

class QueueL {
public:
    QueueL() = default;
    explicit QueueL(const std::ptrdiff_t capacity);
    QueueL(const double capacity);
    QueueL(const int capacity) : QueueL(static_cast<std::ptrdiff_t>(capacity)) {}
    QueueL(const unsigned int capacity) : QueueL(static_cast<std::ptrdiff_t>(capacity)) {}
    QueueL(const QueueL& src);
    QueueL& operator=(const QueueL& src);
    ~QueueL();

    [[nodiscard]] bool is_empty() const noexcept;

    void push(const std::int64_t val);
    void push(const double val) { push(static_cast<std::int64_t>(val)); }
    void push(const int val) { push(static_cast<std::int64_t>(val)); }
    void push(const unsigned int val) { push(static_cast<std::int64_t>(val)); }
    void pop();

    [[nodiscard]] std::int64_t& front();
    [[nodiscard]] std::int64_t front() const;

    // compatibility aliases
    [[nodiscard]] std::int64_t& top() { return front(); }
    [[nodiscard]] std::int64_t top() const { return front(); }

    void clear() noexcept;

private:
    struct Node {
        std::int64_t val = 0;
        Node* next = nullptr;
    };

    Node* head_ = nullptr;
    Node* tail_ = nullptr;
};

#endif
