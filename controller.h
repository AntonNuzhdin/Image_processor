#pragma once

#include "filter_factory.h"

class Controller {
public:
    Controller(const std::vector<FilterFeatures::FilterDescriptor>& filter_pipeline);

    void RunApplication(Image& bitmap);

protected:
    std::vector<FilterFeatures::FilterDescriptor> filter_pipeline_;
};
