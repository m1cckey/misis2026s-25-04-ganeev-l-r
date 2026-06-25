#include <queuel/queuel.hpp>

QueueL::QueueL(const std::ptrdiff_t /*capacity*/) {
}

QueueL::QueueL(const double capacity)
    : QueueL(static_cast<std::ptrdiff_t>(capacity))
{
}

QueueL::QueueL(const QueueL& src) {
    if (!src.is_empty()) {
        head_ = new Node{src.head_->val};
        Node* src_curr = src.head_;
        Node* dst_curr = head_;
        while (src_curr->next) {
            dst_curr->next = new Node{src_curr->next->val};
            src_curr = src_curr->next;
            dst_curr = dst_curr->next;
        }
        tail_ = dst_curr;
    }
}

QueueL& QueueL::operator=(const QueueL& src) {
    if (this != &src) {
        clear();
        if (!src.is_empty()) {
            head_ = new Node{src.head_->val};
            Node* src_curr = src.head_;
            Node* dst_curr = head_;
            while (src_curr->next) {
                dst_curr->next = new Node{src_curr->next->val};
                src_curr = src_curr->next;
                dst_curr = dst_curr->next;
            }
            tail_ = dst_curr;
        }
    }
    return *this;
}

QueueL::~QueueL() {
    clear();
}

bool QueueL::is_empty() const noexcept {
    return head_ == nullptr;
}

void QueueL::push(const std::int64_t val) {
    Node* new_node = new Node{val};
    if (tail_) {
        tail_->next = new_node;
    } else {
        head_ = new_node;
    }
    tail_ = new_node;
}

void QueueL::pop() {
    if (!head_) {
        throw std::invalid_argument("QueueL::pop - queue is empty");
    }
    Node* old = head_;
    head_ = head_->next;
    if (!head_) {
        tail_ = nullptr;
    }
    delete old;
}

std::int64_t& QueueL::front() {
    if (is_empty()) {
        throw std::invalid_argument("QueueL::front - queue is empty");
    }
    return head_->val;
}

std::int64_t QueueL::front() const {
    if (is_empty()) {
        throw std::invalid_argument("QueueL::front - queue is empty");
    }
    return head_->val;
}

void QueueL::clear() noexcept {
    while (!is_empty()) {
        pop();
    }
}
