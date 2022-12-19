# SPDX-FileCopyrightText: 2022 Daniel Aimé Valcour <fosssweeper@gmail.com>
#
# SPDX-License-Identifier: MIT

# Copyright (c) 2022 Daniel Aimé Valcour
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

add_executable(pngw_example "")
add_executable(pngw::example ALIAS pngw_example)
add_subdirectory(src)
target_link_libraries(pngw_example
    PUBLIC
        pngw::pngw
        png
)
set(PNGW_ASSET_FILES
    "dude.png"
)
list(
    TRANSFORM PNGW_ASSET_FILES
    PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
)
add_custom_command(TARGET pngw_example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PNGW_ASSET_FILES}
        $<TARGET_FILE_DIR:pngw_example>
)