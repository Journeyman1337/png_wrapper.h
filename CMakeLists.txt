# SPDX-FileCopyrightText: 2022-2023 Daniel Aimé Valcour <fosssweeper@gmail.com>
#
# SPDX-License-Identifier: MIT

# Copyright (c) 2022-2023 Daniel Aimé Valcour
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

cmake_minimum_required(VERSION 3.19)
PROJECT(pngw
    VERSION 1.0.0
    DESCRIPTION "A header only wrapper around libpng"
    LANGUAGES C
)
option(PNGW_BUILD_EXAMPLE "Build the png_wrapper.h example project" OFF)
option(PNGW_EXAMPLE_AUTO_FETCH "Automatically fetch the dependencies of the png_wrapper.h example project" OFF)
option(PNGW_INSTALL "Generate the png_wrapper.h installation target." ON)
add_library(${PROJECT_NAME} INTERFACE "")
add_library(pngw::pngw ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
    INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)
if(PNGW_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
if(PNGW_INSTALL)
    include(CMakePackageConfigHelpers)
    configure_package_config_file(
        pngw-config.cmake.in
    )
    write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/pngwConfigVersion.cmake"
        VERSION 1.0.1
        COMPATIBILITY SameMajorVersion
    )
    include(GNUInstallDirs)
    install(TARGETS pngw
        EXPORT pngwTargets
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
            COMPONENT          pngw_Development
    )
    install(EXPORT pngwTargets DESTINATION lib/cmake/mylib)
    install(
        DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include"
        DESTINATION "${CMAKE_INSTALL_LINCUDEDIR}"
    )
endif()

