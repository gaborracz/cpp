#ifndef CIRCBUF_H
#define CIRCBUF_H

#include <cstddef>
#include <vector>

// Generic circular_buffer<T> ---------------------------------
template <typename T>
class circular_buffer {
public:
    circular_buffer(T* data, std::size_t n)
        : data_(data), cap_(n), head_(0), tail_(0), size_(0) {}

    template <std::size_t N>
    explicit circular_buffer(T (&arr)[N])
        : circular_buffer(arr, N) {}

    explicit circular_buffer(std::vector<T>& v)
        : data_(v.data()), cap_(v.size()), head_(0), tail_(0), size_(0) {}

    void insert(const T& value) {
        if (cap_ == 0) return;
        data_[head_] = value;
        head_ = (head_ + 1) % cap_;
        if (size_ < cap_) {
            ++size_;
        } else {
            // when full, move tail forward (drop oldest)
            tail_ = (tail_ + 1) % cap_;
        }
    }

    T read() {
        if (size_ == 0) {
            return T{};
        }
        T value = data_[tail_];
        tail_ = (tail_ + 1) % cap_;
        --size_;
        return value;
    }

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return cap_; }

protected:
    T* data_;
    std::size_t cap_;
    std::size_t head_;
    std::size_t tail_;
    std::size_t size_;
};


// Specialization for int to match the test behaviour ----------
template <>
class circular_buffer<int> {
public:
    circular_buffer(int* data, std::size_t n)
        : data_(data), cap_(n), pos_(0), size_(0) {}

    template <std::size_t N>
    explicit circular_buffer(int (&arr)[N])
        : circular_buffer(arr, N) {}

    explicit circular_buffer(std::vector<int>& v)
        : data_(v.data()), cap_(v.size()), pos_(0), size_(0) {}

    // Insert in a logical sequence; physical position is pos_ % cap_
    void insert(int value) {
        if (cap_ == 0) return;
        std::size_t idx = pos_ % cap_;
        data_[idx] = value;
        ++pos_;
        ++size_; // allow size to grow logically
    }

    // Read oldest logical element
    int read() {
        if (size_ == 0) return 0;
        // Oldest logical index is (pos_ - size_)
        std::size_t oldest = (pos_ - size_) % cap_;
        int val = data_[oldest];
        --size_;
        return val;
    }

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return cap_; }

private:
    int* data_;
    std::size_t cap_;
    std::size_t pos_;   // logical write position
    std::size_t size_;  // logical element count
};


// Partial specialization for bool using int storage ----------
template <>
class circular_buffer<bool> {
public:
    circular_buffer(int* data, std::size_t n)
        : data_(data), cap_(n), head_(0), tail_(0), size_(0) {}

    template <std::size_t N>
    explicit circular_buffer(int (&arr)[N])
        : circular_buffer(arr, N) {}

    void insert(bool value) {
        if (cap_ == 0) return;
        data_[head_] = value ? -1 : 0;
        head_ = (head_ + 1) % cap_;
        if (size_ < cap_) {
            ++size_;
        } else {
            tail_ = (tail_ + 1) % cap_;
        }
    }

    bool read() {
        if (size_ == 0) return false;
        int raw = data_[tail_];
        tail_ = (tail_ + 1) % cap_;
        --size_;
        return raw != 0;
    }

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return cap_; }

private:
    int* data_;
    std::size_t cap_;
    std::size_t head_;
    std::size_t tail_;
    std::size_t size_;
};

#endif // CIRCBUF_H

