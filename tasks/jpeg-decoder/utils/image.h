#pragma once

#include <vector>
#include <cstddef>
#include <string>

struct RGB {
    int r, g, b;
};

class Image {
public:
    Image() {
    }
    Image(size_t width, size_t height) {
        SetSize(width, height);
    }

    void SetSize(size_t width, size_t height) {
        data_.assign(height, std::vector<RGB>(width));
    }

    size_t Width() const {
        if (data_.empty()) {
            return 0;
        }
        return data_[0].size();
    }

    size_t Height() const {
        return data_.size();
    }

    void SetPixel(int y, int x, const RGB& pixel) {
        data_[y][x] = pixel;
    }

    RGB GetPixel(int y, int x) const {
        return data_[y][x];
    }

    RGB& GetPixel(int y, int x) {
        return data_[y][x];
    }

    void SetComment(const std::string& comment) {
        comment_ = comment;
    }

    const std::string& GetComment() const {
        return comment_;
    }

private:
    std::vector<std::vector<RGB>> data_;
    std::string comment_;
};
