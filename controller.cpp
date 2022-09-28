#include "controller.h"

Controller::Controller(const std::vector<FilterFeatures::FilterDescriptor>& filter_pipeline)
    : filter_pipeline_(filter_pipeline) {
}

void Controller::RunApplication(Image& bitmap) {

    FilterFactory factory;

    for (const auto& fd : filter_pipeline_) {
        std::unique_ptr<BaseFilter> filter = factory.CreateFilter(fd);
        filter->Apply(bitmap);
    }
}
