#include "image.h"

Image::Image(int32_t width, int32_t height) : width_(width), height_(height) {
    color_array_.resize(height, std::vector<Color>(width));
}

Color& Image::GetBitmapPix(int32_t i, int32_t j) {
    return color_array_[i][j];
}

Color Image::GetBitmapPix(int32_t i, int32_t j) const {
    return color_array_[i][j];
}

void Image::SetHeightWidth(int32_t new_height, int32_t new_width) {
    width_ = new_width;
    height_ = new_height;
}

int32_t Image::GetWidth() const {
    return width_;
}

int32_t Image::GetHeight() const {
    return height_;
}

std::vector<std::vector<Color>>& Image::GetColorArray() {
    return color_array_;
}

std::vector<std::vector<Color>> Image::GetColorArray() const {
    return color_array_;
}
