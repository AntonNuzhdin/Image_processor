#pragma once

#include <vector>

struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};

class Image {
public:
    Image(int32_t width, int32_t height);

    Color GetBitmapPix(int32_t i, int32_t j) const;

    Color& GetBitmapPix(int32_t i, int32_t j);

    void SetHeightWidth(int32_t new_height, int32_t new_width);

    int32_t GetWidth() const;

    int32_t GetHeight() const;

    std::vector<std::vector<Color>>& GetColorArray();

    std::vector<std::vector<Color>> GetColorArray() const;

private:
    std::vector<std::vector<Color>> color_array_;
    int32_t width_;
    int32_t height_;
};
