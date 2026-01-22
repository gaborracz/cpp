#ifndef ARVIOPS_H
#define ARVIOPS_H

#include <cstddef>
#include <vector>
#include <algorithm>

template <typename T>
class array_view_operations {
public:
    // Pointer + size constructor
    array_view_operations(T* data, std::size_t n)
        : data_(data), n_(n), original_(n)
    {
        for (std::size_t i = 0; i < n_; ++i) {
            original_[i] = data_[i];
        }
    }

    // C-array constructor with size deduction
    template <std::size_t N>
    explicit array_view_operations(T (&arr)[N])
        : array_view_operations(arr, N) {}

    // Reverse elements in the view
    void reverse() {
        if (!data_ || n_ == 0) return;
        std::size_t i = 0, j = n_ - 1;
        while (i < j) {
            std::swap(data_[i], data_[j]);
            ++i;
            --j;
        }
    }

    // Circular shift by k positions (right for k>0, left for k<0)
    void shift(int k) {
        if (!data_ || n_ == 0) return;
        std::size_t len = n_;
        int r = k % static_cast<int>(len);
        if (r < 0) r += static_cast<int>(len); // normalize to [0, len)
        if (r == 0) return;

        std::vector<T> temp(len);
        for (std::size_t i = 0; i < len; ++i) {
            std::size_t new_idx = (i + r) % len;
            temp[new_idx] = data_[i];
        }
        for (std::size_t i = 0; i < len; ++i) {
            data_[i] = temp[i];
        }
    }

    // Reset to original contents
    void reset() {
        if (!data_) return;
        for (std::size_t i = 0; i < n_; ++i) {
            data_[i] = original_[i];
        }
    }

    // Default sort using operator<
    void sort() {
        if (!data_) return;
        std::sort(data_, data_ + n_);
    }

    // Sort with custom comparator
    template <typename Comp>
    void sort(Comp comp) {
        if (!data_) return;
        std::sort(data_, data_ + n_, comp);
    }

    // Return first index where predicate is true, or -1
    template <typename Pred>
    int get_first_index_if(Pred p) const {
        if (!data_) return -1;
        for (std::size_t i = 0; i < n_; ++i) {
            if (p(data_[i])) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

private:
    T* data_;
    std::size_t n_;
    std::vector<T> original_;
};

#endif // ARVIOPS_H

