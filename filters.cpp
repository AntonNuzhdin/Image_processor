#include "filters.h"

#include <math.h>
#include <vector>

template <typename T>
void ApplyMatrix(Image& bitmap, const std::vector<std::vector<T>>& coefficients) {
    struct TmpColor {
        T r, g, b;
    };
    std::vector<std::vector<TmpColor>> new_array;
    new_array.resize(bitmap.GetHeight(), std::vector<TmpColor>(bitmap.GetWidth()));
    int32_t edge = coefficients.size() / 2;
    for (size_t row = 0; row < bitmap.GetWidth(); ++row) {
        for (size_t column = 0; column < bitmap.GetHeight(); ++column) {
            double n_r = 0;
            double n_g = 0;
            double n_b = 0;
            for (int32_t delta_x = -edge; delta_x <= edge; ++delta_x) {
                for (int32_t delta_y = -edge; delta_y <= edge; ++delta_y) {
                    int32_t x = row + delta_x;
                    int32_t y = column + delta_y;
                    x = std::min(std::max(x, 0), static_cast<int32_t>(bitmap.GetHeight()) - 1);
                    y = std::min(std::max(y, 0), static_cast<int32_t>(bitmap.GetWidth()) - 1);
                    n_r += bitmap.GetBitmapPix(x, y).r * coefficients[delta_x + edge][delta_y + edge];
                    n_g += bitmap.GetBitmapPix(x, y).g * coefficients[delta_x + edge][delta_y + edge];
                    n_b += bitmap.GetBitmapPix(x, y).b * coefficients[delta_x + edge][delta_y + edge];
                }
            }
            TmpColor element;

            element.r = std::min(255.0, std::max(0.0, n_r));
            element.g = std::min(255.0, std::max(0.0, n_g));
            element.b = std::min(255.0, std::max(0.0, n_b));
            new_array[row][column] = element;
        }
    }
for (size_t i = 0; i < new_array.size(); ++i) {
    for (size_t j = 0; j < new_array.size(); ++j) {
        bitmap.GetBitmapPix(i, j).r = new_array[i][j].r;
        bitmap.GetBitmapPix(i, j).g = new_array[i][j].g;
        bitmap.GetBitmapPix(i, j).b = new_array[i][j].b;
    }
}
}


void Grayscale::Apply(Image& bitmap) const {
    for (size_t i = 0; i < bitmap.GetHeight(); ++i) {
        for (size_t j = 0; j < bitmap.GetWidth(); ++j) {
            uint8_t new_pixel =
                static_cast<uint8_t>(0.299 * bitmap.GetBitmapPix(i, j).r + 0.589 * bitmap.GetBitmapPix(i, j).g +
                                     0.114 * bitmap.GetBitmapPix(i, j).b);
            bitmap.GetBitmapPix(i, j).r = new_pixel;
            bitmap.GetBitmapPix(i, j).g = new_pixel;
            bitmap.GetBitmapPix(i, j).b = new_pixel;
        }
    }
}

void Negative::Apply(Image& bitmap) const {
    for (size_t i = 0; i < bitmap.GetHeight(); ++i) {
        for (size_t j = 0; j < bitmap.GetWidth(); ++j) {
            bitmap.GetBitmapPix(i, j).r = 255 - bitmap.GetBitmapPix(i, j).r;
            bitmap.GetBitmapPix(i, j).g = 255 - bitmap.GetBitmapPix(i, j).g;
            bitmap.GetBitmapPix(i, j).b = 255 - bitmap.GetBitmapPix(i, j).b;
        }
    }
}

void Crop::Apply(Image& bitmap) const {
    if (new_width_ < bitmap.GetWidth() || new_height_ < bitmap.GetHeight()) {
        bitmap.SetHeightWidth(new_height_, new_width_);
    } else {
        return;
    }

    std::vector<std::vector<Color>> new_array;
    new_array.resize(new_height_, std::vector<Color>(new_width_));

    for (int i = 0; i < bitmap.GetHeight(); ++i) {
        for (size_t j = 0; j < new_width_; ++j) {
            new_array[i][j] = bitmap.GetBitmapPix(i, j);
        }
    }
    bitmap.GetColorArray() = new_array;
}

Crop::Crop(size_t height, size_t width) : new_height_(height), new_width_(width) {
}

void Sharpening::Apply(Image& bitmap) const {
    std::vector<std::vector<int>> coefficients = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
    ApplyMatrix(bitmap, coefficients);
}

void EdgeDetection::Apply(Image& bitmap) const {
    Grayscale gs_filter;
    gs_filter.Apply(bitmap);
    std::vector<std::vector<Color>> new_array;
    std::vector<std::vector<int>> coefficients = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
    ApplyMatrix(bitmap, coefficients);

    for (size_t row = 0; row < bitmap.GetHeight(); ++row) {
        for (size_t column = 0; column < bitmap.GetWidth(); ++column) {
            if (bitmap.GetBitmapPix(row, column).r > threshold_) {
                bitmap.GetBitmapPix(row, column).r = 255;
                bitmap.GetBitmapPix(row, column).g = 255;
                bitmap.GetBitmapPix(row, column).b = 255;
            } else {
                bitmap.GetBitmapPix(row, column).r = 0;
                bitmap.GetBitmapPix(row, column).g = 0;
                bitmap.GetBitmapPix(row, column).b = 0;
            }
        }
    }
}

EdgeDetection::EdgeDetection(int threshold) : threshold_(threshold){};

double Blur::GaussFunc(int32_t x, int32_t y) const {
    return pow(M_E, -(pow(x, 2) + pow(y, 2)) / (2 * pow(sigma_, 2))) / (2 * M_PI * pow(sigma_, 2));
}

void Blur::Apply(Image& bitmap) const {
    int kernel_size = 3 * sigma_;
    std::vector<std::vector<double>> kernel;

    double whole_sum = 0;
    for (int32_t x = -kernel_size; x <= kernel_size; ++x) {
        std::vector<double> tmp;
        for (int32_t y = -kernel_size; y <= kernel_size; ++y) {
            double gauss_func = GaussFunc(x, y);
            whole_sum += gauss_func;
            tmp.push_back(gauss_func);
        }
        kernel.push_back(tmp);
    }

    for (int32_t x = -kernel_size; x < kernel_size; ++x) {
        for (int32_t y = kernel_size; y < kernel_size; ++y) {
            kernel[x + kernel_size][y + kernel_size] /= whole_sum;
        }
    }
    ApplyMatrix(bitmap, kernel);
}

Pixelation::Pixelation(int param) : adjustment(param){};

void Pixelation::Apply(Image& bitmap) const {
    std::vector<std::vector<Color>> new_array;
    new_array.resize(bitmap.GetHeight(), std::vector<Color>(bitmap.GetWidth()));

    for (int y = 0; y < bitmap.GetHeight() - 3; y++) {
        for (int x = 0; x < bitmap.GetWidth() - 3; x++) {
            for (int c = 0; c < 3; ++c) {
                int y_pixel = y - y % adjustment;
                int x_pixel = x - x % adjustment;

                if (y % (adjustment * 2) < adjustment && x % (adjustment * 2) < adjustment) {
                    new_array[y][x + c] = bitmap.GetBitmapPix(y_pixel, x_pixel + c);
                } else {
                    new_array[y][x + c].r = 0;
                    new_array[y][x + c].g = 0;
                    new_array[y][x + c].b = 0;
                }
            }
        }
    }
    bitmap.GetColorArray() = new_array;
}
