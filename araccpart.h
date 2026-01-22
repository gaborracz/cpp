#ifndef ARACCPART_H
#define ARACCPART_H

#include <vector>
#include <functional>
#include <cstddef>

template<typename T, typename Op = std::plus<T>>
class array_accumulate_partition {
public:
    // Constructors
    array_accumulate_partition(T* data, std::size_t n)
        : orig_(data), n_(n), acc_(n), op_(Op())
    {
        // Initially one partition: [0, n)
        bounds_.push_back(0);
        bounds_.push_back(n_);
        recompute_all();
    }

    template<std::size_t N>
    explicit array_accumulate_partition(T (&arr)[N])
        : array_accumulate_partition(arr, N) {}

    // Partition index insertion (task 2)
    void set_partition_idx(std::size_t idx) {
        if (idx > n_) return;
        // Insert boundary if not already present
        for (std::size_t b : bounds_) {
            if (b == idx) {
                // nothing to do
                return;
            }
        }
        bounds_.push_back(idx);
        // Keep bounds sorted
        std::sort(bounds_.begin(), bounds_.end());
        recompute_all();
    }

    // Iterators over accumulated array (task 4)
    T* abegin() { return acc_.data(); }
    T* aend()   { return acc_.data() + n_; }

    const T* abegin() const { return acc_.data(); }
    const T* aend()   const { return acc_.data() + n_; }

    // Iterators over original array (task 5)
    T* begin() { return orig_; }
    T* end()   { return orig_ + n_; }

    const T* begin() const { return orig_; }
    const T* end()   const { return orig_ + n_; }

    // Helper: number of partitions
    std::size_t partition_count() const {
        return bounds_.size() > 0 ? bounds_.size() - 1 : 0;
    }

    // Access accumulated value of partition p
    T partition_value(std::size_t p) const {
        if (p >= partition_count()) return T{};
        std::size_t left  = bounds_[p];
        std::size_t right = bounds_[p + 1];
        if (left >= right || right > n_) return T{};
        return acc_[left]; // all elements in partition have same value
    }

    // Write accumulated value of partition p into its last element
    // of the original array (used by operators, tasks 3 & 5)
    void apply_partition_to_original(std::size_t p) {
        if (p >= partition_count()) return;
        std::size_t left  = bounds_[p];
        std::size_t right = bounds_[p + 1];
        if (left >= right || right > n_) return;
        orig_[right - 1] = acc_[left];
    }

private:
    T* orig_;
    std::size_t n_;
    std::vector<T> acc_;
    std::vector<std::size_t> bounds_;
    Op op_;

    void recompute_all() {
        if (n_ == 0 || bounds_.size() < 2) return;
        // For every partition, compute accumulated value and fill acc_
        for (std::size_t pi = 0; pi + 1 < bounds_.size(); ++pi) {
            std::size_t left  = bounds_[pi];
            std::size_t right = bounds_[pi + 1];
            if (left >= right || right > n_) continue;
            T value{};
            bool first = true;
            for (std::size_t i = left; i < right; ++i) {
                if (first) {
                    value = orig_[i];
                    first = false;
                } else {
                    value = op_(value, orig_[i]);
                }
            }
            for (std::size_t i = left; i < right; ++i) {
                acc_[i] = value;
            }
        }
    }
};

// Shift operators (tasks 3 & 5)

// ap >> idx  : use partition idx, write to original
template<typename T, typename Op>
array_accumulate_partition<T, Op>&
operator>>(array_accumulate_partition<T, Op>& ap, std::size_t idx) {
    ap.apply_partition_to_original(idx);
    return ap;
}

// idx >> ap : same semantics, but index on left
template<typename T, typename Op>
array_accumulate_partition<T, Op>&
operator>>(std::size_t idx, array_accumulate_partition<T, Op>& ap) {
    ap.apply_partition_to_original(idx);
    return ap;
}

// val << ap : use val as partition index for ap
template<typename T, typename Op>
array_accumulate_partition<T, Op>&
operator<<(std::size_t idx, array_accumulate_partition<T, Op>& ap) {
    ap.apply_partition_to_original(idx);
    return ap;
}

#endif // ARACCPART_H

