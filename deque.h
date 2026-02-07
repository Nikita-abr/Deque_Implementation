#pragma once

#include <cstddef>
#include <initializer_list>
#include <span>
#include <utility>

class Deque {
   public:
    Deque() : backets_(new int*[1]), size_(1), begin_({0, 0}), end_({0, 1}), count_(1) {
        std::span(backets_, 1)[0] = new int[kChankSize];  // NOLINT(cppcoreguidelines-owning-memory)
        // backets_[0] = new int[kChankSize];
    }

    Deque(const Deque& other) {
        this->Resize(other.size_);
        for (size_t i = 0; i < size_; ++i) {
            for (size_t j = 0; j < kChankSize; ++j) {
                std::span(backets_, size_)[i][j] = std::span(other.backets_, size_)[i][j];
                // backets_[i][j] = other.backets_[i][j];
            }
        }

        begin_ = other.begin_;
        end_ = other.end_;
        count_ = other.count_;
    }

    Deque(Deque&& other) noexcept
        : backets_(other.backets_)
        , size_(other.size_)
        , begin_(other.begin_)
        , end_(other.end_)
        , count_(other.count_) {
        other.backets_ = nullptr;
        other.size_ = 0;
    }

    explicit Deque(size_t size) : count_(1) {
        this->Resize(1);
        begin_ = {0, 0};
        end_ = {0, 1};
        for (size_t i = 0; i < size; ++i) {
            this->PushBack(0);
        }
    }

    Deque(std::initializer_list<int> list) : count_(1) {
        this->Resize(1);
        begin_ = {0, 0};
        end_ = {0, 1};
        for (const auto& el : list) {
            this->PushBack(el);
        }
    }

    void Resize(size_t size) {
        for (size_t i = 0; i < size_; ++i) {
            delete[] std::span(backets_, size_)[i];  // NOLINT(cppcoreguidelines-owning-memory)
        }
        delete[] backets_;
        backets_ = new int*[size];                   // NOLINT(cppcoreguidelines-owning-memory)
        size_ = size;
        for (size_t i = 0; i < size; ++i) {
            std::span(backets_, size_)[i] =          // NOLINT(cppcoreguidelines-owning-memory)
                new int[kChankSize];                 // NOLINT(cppcoreguidelines-owning-memory)
            // backets_[i] = new int[kChankSize];
        }
    }

    Deque& operator=(const Deque& other) {
        if (this == &other) {
            return *this;
        }

        this->Resize(other.size_);
        begin_ = other.begin_;
        end_ = other.end_;
        count_ = other.count_;
        for (size_t i = 0; i < size_; ++i) {
            for (size_t j = 0; j < kChankSize; ++j) {
                std::span(backets_, size_)[i][j] = std::span(other.backets_, size_)[i][j];
                // backets_[i][j] = other.backets_[i][j];
            }
        }
        return *this;
    }

    Deque& operator=(Deque&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        for (size_t i = 0; i < size_; ++i) {
            delete[] std::span(backets_, size_)[i];  // NOLINT(cppcoreguidelines-owning-memory)
        }
        delete[] backets_;

        backets_ = other.backets_;
        size_ = other.size_;
        begin_ = other.begin_;
        end_ = other.end_;
        count_ = other.count_;

        other.backets_ = nullptr;
        other.size_ = 0;

        return *this;
    }

    ~Deque() {
        for (size_t i = 0; i < size_; ++i) {
            delete[] std::span(backets_, size_)[i];  // NOLINT(cppcoreguidelines-owning-memory)
        }
        delete[] backets_;
    }

    void Swap(Deque& other) {
        std::swap(backets_, other.backets_);
        std::swap(begin_, other.begin_);
        std::swap(end_, other.end_);
        std::swap(size_, other.size_);
        std::swap(count_, other.count_);
    }

    void PushBack(int value) {
        std::span(backets_, size_)[end_.first][end_.second] = value;
        // backets_[end_.first][end_.second] = value;
        count_++;

        if (end_.second == kChankSize - 1) {
            int** new_backets = new int*[size_ + 1];  // NOLINT(cppcoreguidelines-owning-memory)
            for (size_t i = 0; i < size_; ++i) {
                std::span(new_backets, size_)[i] = std::span(backets_, size_)[i];
            }
            std::span(new_backets, size_ + 1)[size_] =  // NOLINT(cppcoreguidelines-owning-memory)
                new int[kChankSize];                    // NOLINT(cppcoreguidelines-owning-memory)
            delete[] backets_;
            backets_ = new_backets;
            ++size_;
            end_ = {size_ - 1, 0};
        } else {
            end_.second++;
        }
    }

    void PopBack() {
        if (end_.second != 0) {
            end_.second--;
        } else {
            delete[] std::span(                // NOLINT(cppcoreguidelines-owning-memory)
                backets_, size_)[end_.first];  // NOLINT(cppcoreguidelines-owning-memory)
            --size_;
            --end_.first;
            end_.second = kChankSize - 1;
        }
        std::span(backets_, size_)[end_.first][end_.second] = 0;
        count_--;
        // backets_[end_.first][end_.second] = 0;
    }

    void PushFront(int value) {
        std::span(backets_, size_)[begin_.first][begin_.second] = value;
        count_++;

        if (begin_.second == 0) {
            int** new_backets = new int*[size_ + 1];  // NOLINT(cppcoreguidelines-owning-memory)
            for (size_t i = 1; i <= size_; ++i) {
                std::span(new_backets, size_)[i] = std::span(backets_, size_)[i - 1];
            }
            std::span(new_backets, size_)[0] =  // NOLINT(cppcoreguidelines-owning-memory)
                new int[kChankSize];            // NOLINT(cppcoreguidelines-owning-memory)
            delete[] backets_;
            backets_ = new_backets;
            ++size_;
            begin_ = {0, kChankSize - 1};
            end_.first++;
        } else {
            begin_.second--;
        }
    }

    void PopFront() {
        if (begin_.second != kChankSize - 1) {
            begin_.second++;
        } else {
            int** new_backets = new int*[size_ - 1];  // NOLINT(cppcoreguidelines-owning-memory)
            for (size_t i = 1; i < size_; ++i) {
                std::span(new_backets, size_)[i - 1] = std::span(backets_, size_)[i];
                // new_backets[i - 1] = backets_[i];
            }
            delete[] std::span(backets_, size_)[0];  // NOLINT(cppcoreguidelines-owning-memory)
            delete[] backets_;
            backets_ = new_backets;
            --size_;
            begin_ = {0, 0};
            end_.first--;
        }
        std::span(backets_, size_)[begin_.first][begin_.second] = 0;
        count_--;
        // backets_[begin_.first][begin_.second] = 0;
    }

    int& operator[](size_t index) {
        int ind = begin_.first * kChankSize + begin_.second + static_cast<int>(index) + 1;
        ind = ind % (static_cast<int>(size_) * kChankSize);
        return std::span(backets_, size_)[ind / kChankSize][ind % kChankSize];
        // return backets_[ind / kChankSize][ind % kChankSize];
    }

    const int& operator[](size_t index) const {
        int ind = begin_.first * kChankSize + begin_.second + static_cast<int>(index) + 1;
        ind = ind % (static_cast<int>(size_) * kChankSize);
        return std::span(backets_, size_)[ind / kChankSize][ind % kChankSize];
        // return backets_[ind / kChankSize][ind % kChankSize];
    }

    [[nodiscard]] size_t Sizee() const {
        if (begin_ == end_) {
            return 0;
        }
        return end_.first * kChankSize + end_.second - begin_.first * kChankSize - begin_.second -
               1;
    }

    [[nodiscard]] size_t Size() const {  // NOLINT
        return count_ - 1;               // NOLINT
    }  // NOLINT

    [[nodiscard]] size_t Capacity() const {
        return size_ * static_cast<size_t>(kChankSize);
    }

    void Clear() {
        for (size_t i = 1; i < size_; ++i) {
            delete[] std::span(backets_, size_)[i];  // NOLINT(cppcoreguidelines-owning-memory)
        }
        begin_ = {0, 0};
        end_ = {0, 1};
        count_ = 1;
    }

    // void Print() const {
    //     std::cout << "begin: " << begin_.first << " " << begin_.second << "\nend: " << end_.first
    //               << " " << end_.second << "\nsize: " << size_ << "\n";
    //     for (size_t i = 0; i < size_; ++i) {
    //         for (size_t j = 0; j < kChankSize; ++j) {
    //             if (i == begin_.first && j == begin_.second) {
    //                 std::cout << "b ";
    //             } else if (i == end_.first && j == end_.second) {
    //                 std::cout << "e ";
    //             } else {
    //                 std::cout << backets_[i][j] << " ";
    //             }
    //         }
    //         std::cout << "\n";
    //     }
    // }

   private:
    int** backets_ = nullptr;
    size_t size_ = 0;
    // std::vector<int*> backets_;
    std::pair<int, int> begin_;
    std::pair<int, int> end_;
    size_t count_;

    static const int kChankSize = 128;
};
