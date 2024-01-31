#pragma once

#include <cstring>
#include <initializer_list>
#include <algorithm>
#include <deque>
#include <iterator>
#include <memory>

class Deque {
public:
    Deque() {
        data_ = nullptr;
        buffers_number_ = 2;
        buffer_begin_ = 0;
        buffer_end_ = 0;
        data_size_ = 0;
        data_begin_ = 0;
        data_end_ = 0;

        Allocator();
    };

    Deque(const Deque& rhs) : Deque() {
        buffers_number_ =
            std::max(static_cast<size_t>(1), (rhs.data_size_ + kBlockSize - 1) / kBlockSize);
        Allocator();
        rhs.Duplicator(data_);
        buffer_end_ = data_size_ / kBlockSize;
        data_size_ = rhs.data_size_;
        data_end_ = data_size_ % kBlockSize;
        if (buffers_number_ * kBlockSize == data_size_) {
            buffer_end_ = 0;
        }
    };

    Deque(Deque&& rhs) : Deque() {
        Swap(rhs);
    };

    explicit Deque(size_t size) : Deque() {
        buffers_number_ = std::max(static_cast<size_t>(1), (size + kBlockSize - 1 / kBlockSize));
        Allocator();
        for (size_t i = 0; i < size; ++i) {
            PushBack(0);
        }
    };

    Deque(std::initializer_list<int> list) : Deque() {
        buffers_number_ =
            std::max(static_cast<size_t>(1), (list.size() + kBlockSize - 1 / kBlockSize));
        Allocator();
        for (const auto& i : list) {
            PushBack(i);
        }
    };

    Deque& operator=(Deque rhs) {
        Swap(rhs);
        return *this;
    }

    void Swap(Deque& rhs) {
        data_.swap(rhs.data_);
        // buffer
        std::swap(buffers_number_, rhs.buffers_number_);
        std::swap(buffer_end_, rhs.buffer_end_);
        std::swap(buffer_begin_, rhs.buffer_begin_);
        // current block
        std::swap(data_size_, rhs.data_size_);
        std::swap(data_begin_, rhs.data_begin_);
        std::swap(data_end_, rhs.data_end_);
    }

    void PushBack(int value) {
        Rellocator();

        data_[buffer_end_][data_end_] = value;
        RightTransition(data_end_, buffer_end_);
        ++data_size_;
    }

    void PopBack() {
        LeftTransition(data_end_, buffer_end_);
        --data_size_;
    }

    void PushFront(int value) {
        Rellocator();

        LeftTransition(data_begin_, buffer_begin_);
        data_[buffer_begin_][data_begin_] = value;
        ++data_size_;
    }

    void PopFront() {
        RightTransition(data_begin_, buffer_begin_);
        --data_size_;
    }

    int& operator[](size_t ind) {
        return data_[(buffer_begin_ + (data_begin_ + ind) / kBlockSize) % buffers_number_]
                    [(data_begin_ + ind) % kBlockSize];
    }

    int operator[](size_t ind) const {
        return data_[(buffer_begin_ + (data_begin_ + ind) / kBlockSize) % buffers_number_]
                    [(data_begin_ + ind) % kBlockSize];
    }

    size_t Size() const {
        return data_size_;
    }

    void Clear() {
        data_size_ = 0;
        data_begin_ = data_end_;
        buffer_begin_ = buffer_end_;
    }

private:
    void Allocator() {
        data_ =
            std::shared_ptr<std::shared_ptr<int[]>[]>(new std::shared_ptr<int[]>[buffers_number_]);

        for (size_t i = 0; i < buffers_number_; ++i) {
            data_[i] = std::shared_ptr<int[]>(new int[kBlockSize]);
            for (size_t j = 0; j < kBlockSize; ++j) {
                data_[i][j] = 0;
            }
        }
    };

    void Rellocator() {
        if (data_size_ < kBlockSize * buffers_number_) {
            return;
        }

        std::shared_ptr<std::shared_ptr<int[]>[]> new_data(
            new std::shared_ptr<int[]>[2 * buffers_number_]);

        size_t new_pos = 0;
        size_t old_pos = buffer_begin_;

        for (size_t i = 0; i < buffers_number_ * 2; ++i) {
            new_data[i] = std::shared_ptr<int[]>(new int[kBlockSize]);
        }

        do {
            if (new_data[new_pos] != nullptr && data_[old_pos] != nullptr) {
                new_data[new_pos++].swap(data_[old_pos++]);
            }
            if (old_pos == buffers_number_) {
                old_pos = 0;
            }
        } while (old_pos != buffer_end_);

        buffer_begin_ = data_begin_ = 0;
        data_end_ = data_size_ % kBlockSize;
        buffer_end_ = buffers_number_;
        buffers_number_ *= 2;
        data_.swap(new_data);
    };

    void Duplicator(const std::shared_ptr<std::shared_ptr<int[]>[]>& rhs) const {
        for (size_t i = 0, rhs_buffer_begin = 0, rhs_data_begin = 0,
                    buffer_begin_copy = buffer_begin_, data_begin_copy = data_begin_;
             i < data_size_; ++i) {
            rhs[rhs_buffer_begin][rhs_data_begin] = data_[buffer_begin_copy][data_begin_copy];
            RightTransition(data_begin_copy, buffer_begin_copy);
            RightTransition(rhs_data_begin, rhs_buffer_begin);
        }
    };

    void LeftTransition(size_t& ind, size_t& block_id) const {
        if (ind > 0) {
            --ind;
            return;
        }
        ind = kBlockSize - 1;
        if (block_id > 0) {
            --block_id;
            return;
        }
        block_id = buffers_number_ - 1;
    };

    void RightTransition(size_t& ind, size_t& block_id) const {
        if (ind + 1 < kBlockSize) {
            ++ind;
            return;
        }
        ind = 0;
        if (block_id + 1 < buffers_number_) {
            ++block_id;
            return;
        }
        block_id = 0;
    };

private:
    std::shared_ptr<std::shared_ptr<int[]>[]> data_;

    static constexpr size_t kBlockSize = 128;

    size_t buffers_number_;
    size_t data_size_;
    size_t buffer_begin_;
    size_t data_begin_;
    size_t buffer_end_;
    size_t data_end_;
};
