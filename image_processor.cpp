#include "cmd_parser.h"
#include "bmp_image.h"
#include "controller.h"

int main(int argc, char *argv[]) {
    CmdParser parser(argc, argv);
    auto img = BmpFormat::Load(parser.GetInputFileName());
    Controller application(parser.GetFiltersPipeline());
    try {
        application.RunApplication(img);
        BmpFormat::WriteFile(img, parser.GetOutputFileName());
    } catch (std::exception& ex) {
        throw MyException(ex.what());
    }
}
