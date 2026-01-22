#ifndef GAPBUFFER_H
#define GAPBUFFER_H

#include <cstddef>
#include <cstring>
#include <cwchar>

// Generic gap_buffer for character‑like T
template <typename T>
class gap_buffer {
public:
    gap_buffer(T* data, std::size_t capacity, std::size_t gap_size)
        : buf_(data),
          cap_(capacity),
          gap_begin_(0),
          gap_end_(gap_size <= capacity ? gap_size : capacity)
    {
        // buffer is pre‑initialized by caller (spaces / zeros)
    }

    // Insert a single character at cursor (at gap_begin)
    void insert(T ch) {
        if (gap_begin_ >= gap_end_) return; // gap full, ignore in this task set
        buf_[gap_begin_++] = ch;
    }

    // Move cursor one position to the right
    void right() {
        if (gap_end_ >= cap_) return;
        buf_[gap_begin_] = buf_[gap_end_];
        ++gap_begin_;
        ++gap_end_;
    }

    // Move cursor one position to the left
    void left() {
        if (gap_begin_ == 0) return;
        --gap_begin_;
        --gap_end_;
        buf_[gap_end_] = buf_[gap_begin_];
    }

    // Erase character before cursor (backspace)
    void erase() {
        if (gap_begin_ == 0) return;
        --gap_begin_;
        // That position becomes part of the gap; nothing else to do
    }

    // at() – logical element access, hides the gap
    T& at(std::size_t idx) {
        std::size_t real = physical_index(idx);
        return buf_[real];
    }

    const T& at(std::size_t idx) const {
        std::size_t real = physical_index(idx);
        return buf_[real];
    }

    // operator[] – same as at(), but no bounds checks
    T operator[](std::size_t idx) const {
        return at(idx);
    }

    // Append C‑string
    gap_buffer& operator+=(const T* str) {
        // Move gap to end of logical text
        move_gap_to_end();
        // Insert characters until terminator
        std::size_t i = 0;
        while (str[i] != T(0) && gap_begin_ < gap_end_) {
            insert(str[i]);
            ++i;
        }
        return *this;
    }

private:
    T* buf_;
    std::size_t cap_;
    std::size_t gap_begin_;
    std::size_t gap_end_;

    // Compute physical index for logical idx
    std::size_t physical_index(std::size_t idx) const {
        if (idx < gap_begin_) {
            return idx;
        }
        return idx + (gap_end_ - gap_begin_);
    }

    // Move the gap to the end of current logical text
    void move_gap_to_end() {
        // Logical length without gap
        std::size_t logical_len = cap_ - (gap_end_ - gap_begin_);
        while (gap_begin_ < logical_len && gap_end_ < cap_) {
            buf_[gap_end_] = buf_[gap_begin_];
            ++gap_begin_;
            ++gap_end_;
        }
    }
};

#endif // GAPBUFFER_H


