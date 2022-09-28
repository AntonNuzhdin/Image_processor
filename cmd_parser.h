#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct MyException : std::runtime_error {
    explicit MyException(const std::string& message) : std::runtime_error(message.c_str()){};
};

namespace FilterFeatures {
enum FilterTypes {
    Crop,
    Grayscale,
    Negative,
    Sharpening,
    EdgeDetection,
    GaussianBlur,
    Pixelation,
};

struct FilterDescriptor {
    FilterTypes filter_name;
    std::vector<std::string> filter_params;
};
}  // namespace FilterFeatures

class CmdParser {
public:
    CmdParser(int argc, char* argv[]);

    const std::string GetInputFileName() const;

    const std::string GetOutputFileName() const;

    std::vector<FilterFeatures::FilterDescriptor> GetFiltersPipeline() const;

protected:
    static const int min_amount_of_params = 2;
    static const int input_file_name_position = 1;
    static const int output_file_name_position = 2;
    std::vector<FilterFeatures::FilterDescriptor> filters_pipeline_;

protected:
    const char* input_file_name_;
    const char* output_file_name_;
    const std::unordered_map<std::string, FilterFeatures::FilterTypes> cmd_name_to_filter_name_ = {
        {"-crop", FilterFeatures::Crop},          {"-gs", FilterFeatures::Grayscale},
        {"-neg", FilterFeatures::Negative},       {"-sharp", FilterFeatures::Sharpening},
        {"-edge", FilterFeatures::EdgeDetection}, {"-blur", FilterFeatures::GaussianBlur},
        {"-pix", FilterFeatures::Pixelation}};
};
