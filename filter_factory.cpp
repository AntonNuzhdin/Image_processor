#include "filter_factory.h"

std::unique_ptr<BaseFilter> FilterFactory::CreateFilter(const FilterFeatures::FilterDescriptor& descriptor) const {
    FFactory func = filter_to_makers_.at(descriptor.filter_name);
    std::unique_ptr<BaseFilter> current_filter = func(descriptor);
    return current_filter;
}

namespace FilterFactories {
std::unique_ptr<BaseFilter> CreateBlur(const FilterFeatures::FilterDescriptor& fd) {
    if (fd.filter_params.empty()) {
        throw std::invalid_argument("wrong blur filter params size");
    }
    return std::unique_ptr<BaseFilter>(new Blur(std::stod(fd.filter_params[0])));
}

std::unique_ptr<BaseFilter> CreateCrop(const FilterFeatures::FilterDescriptor& fd) {
    if (fd.filter_params.size() != 2) {
        throw std::invalid_argument("wrong crop filter params size");
    }

    return std::unique_ptr<BaseFilter>(new Crop(std::stoi(fd.filter_params[0]), std::stoi(fd.filter_params[1])));
}

std::unique_ptr<BaseFilter> CreateNegative(const FilterFeatures::FilterDescriptor& fd) {
    if (!fd.filter_params.empty()) {
        throw std::invalid_argument("wrong negative filter params size");
    }
    return std::unique_ptr<BaseFilter>(new Negative);
}

std::unique_ptr<BaseFilter> CreateGrayscale(const FilterFeatures::FilterDescriptor& fd) {
    if (!fd.filter_params.empty()) {
        throw std::invalid_argument("wrong grayscale filter params size");
    }
    return std::unique_ptr<BaseFilter>(new Grayscale);
}

std::unique_ptr<BaseFilter> CreateEdgeDetection(const FilterFeatures::FilterDescriptor& fd) {
    if (fd.filter_params.empty()) {
        throw std::invalid_argument("wrong edge detection filter params size");
    }
    return std::unique_ptr<BaseFilter>(new EdgeDetection(std::stoi(fd.filter_params[0])));
}

std::unique_ptr<BaseFilter> CreateSharp(const FilterFeatures::FilterDescriptor& fd) {
    if (!fd.filter_params.empty()) {
        throw std::invalid_argument("wrong sharpening filter params size");
    }
    return std::unique_ptr<BaseFilter>(new Sharpening);
}

std::unique_ptr<BaseFilter> CreatePixelation(const FilterFeatures::FilterDescriptor& fd) {
    if (fd.filter_params.empty()) {
        throw std::invalid_argument("wrong pixelation filter params size");
    }
    return std::unique_ptr<BaseFilter>(new Pixelation(std::stoi(fd.filter_params[0])));
}
}  // namespace FilterFactories

FilterFactory::FilterFactory() {
    filter_to_makers_ = {{FilterFeatures::GaussianBlur, FilterFactories::CreateBlur},
                         {FilterFeatures::EdgeDetection, FilterFactories::CreateEdgeDetection},
                         {FilterFeatures::Crop, FilterFactories::CreateCrop},
                         {FilterFeatures::Grayscale, FilterFactories::CreateGrayscale},
                         {FilterFeatures::Sharpening, FilterFactories::CreateSharp},
                         {FilterFeatures::Negative, FilterFactories::CreateNegative},
                         {FilterFeatures::Pixelation, FilterFactories::CreatePixelation}};
}
