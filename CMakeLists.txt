cmake_minimum_required(VERSION 3.17)
project(image_processor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(SOURCE_FILES

        cmd_parser.cpp cmd_parser.h bmp_image.cpp bmp_image.h filters.h filters.cpp image.cpp image.h utilities.h filter_factory.h filter_factory.cpp controller.h controller.cpp)
add_executable(image_processor ${SOURCE_FILES} image_processor.cpp)
