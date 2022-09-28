#pragma once

#include "bmp_image.h"

#include <vector>
#include <math.h>

class BaseFilter {
public:
    virtual void Apply(Image& bitmap) const = 0;

    virtual ~BaseFilter() = default;
};

class Blur : public BaseFilter {
public:
    Blur(double sigma) : sigma_(sigma){};
    void Apply(Image& bitmap) const override;

protected:
    double GaussFunc(int32_t x, int32_t y) const;
    double sigma_;
};

class Crop : public BaseFilter {
public:
    Crop(size_t height, size_t width);
    void Apply(Image& bitmap) const override;

protected:
    size_t new_height_;
    size_t new_width_;
};

class Negative : public BaseFilter {
public:
    void Apply(Image& bitmap) const override;
};

class Grayscale : public BaseFilter {
public:
    void Apply(Image& bitmap) const override;
};

class Sharpening : public BaseFilter {
public:
    void Apply(Image& bitmap) const override;
};

class EdgeDetection : public BaseFilter {
public:
    EdgeDetection(int threshold);
    void Apply(Image& bitmap) const override;

protected:
    int threshold_;
};

class Pixelation : public BaseFilter {
public:
    Pixelation(int param);
    void Apply(Image& bitmap) const override;

protected:
    int adjustment;
};
