#include "cmd_parser.h"

#include <string_view>

CmdParser::CmdParser(int argc, char *argv[]) {
    if (argc < min_amount_of_params) {
        throw MyException("Too few parameters");
    }

    input_file_name_ = argv[input_file_name_position];
    output_file_name_ = argv[output_file_name_position];

    // Бежим по всем параметрам после имен файлов
    FilterFeatures::FilterDescriptor filter;
    bool flag = true;
    for (size_t i = 3; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (!flag) {
                filters_pipeline_.push_back(filter);
                filter = FilterFeatures::FilterDescriptor{};
            }
            filter.filter_name = cmd_name_to_filter_name_.at(argv[i]);
            flag = false;
        } else {
            filter.filter_params.push_back(argv[i]);
        }
    }
    filters_pipeline_.push_back(filter);
}

const std::string CmdParser::GetOutputFileName() const {
    return output_file_name_;
}

const std::string CmdParser::GetInputFileName() const {
    return input_file_name_;
}

std::vector<FilterFeatures::FilterDescriptor> CmdParser::GetFiltersPipeline() const {
    return filters_pipeline_;
}
