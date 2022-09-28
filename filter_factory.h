#pragma once

#include "cmd_parser.h"
#include "filters.h"

#include <memory>
#include <unordered_map>
#include <vector>

class FilterFactory {
public:

    using FFactory = std::unique_ptr<BaseFilter> (*)(const FilterFeatures::FilterDescriptor&);
    using FilterToMakerMap = std::unordered_map<FilterFeatures::FilterTypes, FFactory>;

    FilterFactory();

    std::unique_ptr<BaseFilter> CreateFilter(const FilterFeatures::FilterDescriptor& descriptor) const;

protected:
    FilterToMakerMap filter_to_makers_;
};
