#ifndef ALIGNER_H
#define ALIGNER_H

#include <vector>
#include <cstddef>
#include <algorithm>
#include <numeric>

// Base interface
template <typename T>
class aligner {
public:
    virtual ~aligner() {}

    // Add a row to be aligned
    void add(std::vector<T>& row) {
        rows_.push_back(&row);
    }

    // Set filler value
    void set_filler(const T& f) {
        filler_ = f;
        has_filler_ = true;
    }

    // Number of rows
    std::size_t count() const {
        return rows_.size();
    }

    // Main alignment entry
    void align() {
        if (rows_.empty()) return;
        std::size_t max_len = 0;
        for (auto* r : rows_) {
            if (r)
                max_len = std::max(max_len, r->size());
        }
        if (max_len == 0) return;
        for (auto* r : rows_) {
            if (!r) continue;
            do_align(*r, max_len);
        }
    }

protected:
    std::vector<std::vector<T>*> rows_;
    T filler_{};
    bool has_filler_ = false;

    const T& get_filler() const {
        return has_filler_ ? filler_ : default_filler();
    }

    virtual const T& default_filler() const {
        static T def{}; // 0 for ints, empty for strings, etc.
        return def;
    }

    // To be implemented by derived classes
    virtual void do_align(std::vector<T>& row, std::size_t width) = 0;
};

// Center aligner
template <typename T>
class center_aligner : public aligner<T> {
protected:
    using aligner<T>::get_filler;

    void do_align(std::vector<T>& row, std::size_t width) override {
        if (row.size() >= width) return;
        std::size_t total_pad = width - row.size();
        std::size_t left_pad  = total_pad / 2;
        std::size_t right_pad = total_pad - left_pad;

        const T& fill = get_filler();
        row.insert(row.begin(), left_pad, fill);
        row.insert(row.end(),   right_pad, fill);
    }
};

// Right aligner
template <typename T>
class right_aligner : public aligner<T> {
protected:
    using aligner<T>::get_filler;

    void do_align(std::vector<T>& row, std::size_t width) override {
        if (row.size() >= width) return;
        std::size_t total_pad = width - row.size();
        const T& fill = get_filler();
        // Pad on the left so data is right aligned
        row.insert(row.begin(), total_pad, fill);
    }
};

// Left aligner
template <typename T>
class left_aligner : public aligner<T> {
protected:
    using aligner<T>::get_filler;

    void do_align(std::vector<T>& row, std::size_t width) override {
        if (row.size() >= width) return;
        std::size_t total_pad = width - row.size();
        const T& fill = get_filler();
        // Pad on the right so data is left aligned
        row.insert(row.end(), total_pad, fill);
    }
};

// Justified aligner
template <typename T>
class justified_aligner : public aligner<T> {
protected:
    using aligner<T>::get_filler;

    void do_align(std::vector<T>& row, std::size_t width) override {
        const T& fill = get_filler();
        if (row.empty()) {
            // Just fill with filler
            row.assign(width, fill);
            return;
        }
        if (row.size() >= width) return;

        std::size_t gaps = (row.size() > 1) ? (row.size() - 1) : 1;
        std::size_t total_pad = width - row.size();

        // For 1 element: append all fillers at the end
        if (row.size() == 1) {
            row.insert(row.end(), total_pad, fill);
            return;
        }

        // Distribute fillers between gaps as evenly as possible
        std::vector<std::size_t> pads(gaps, total_pad / gaps);
        std::size_t extra = total_pad % gaps;
        for (std::size_t i = 0; i < extra; ++i) {
            ++pads[i];
        }

        std::vector<T> result;
        result.reserve(width);

        for (std::size_t i = 0; i < row.size(); ++i) {
            result.push_back(row[i]);
            if (i + 1 < row.size()) {
                result.insert(result.end(), pads[i], fill);
            }
        }
        row.swap(result);
    }
};

#endif // ALIGNER_H

