// SPDX-FileCopyrightText: 2022-2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

/*
    Copyright (c) 2022-2024 Daniel Aimé Valcour

    Permission is hereby granted, free of charge, to any person obtaining a copy of
    this software and associated documentation files (the "Software"), to deal in
    the Software without restriction, including without limitation the rights to
    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
    the Software, and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
    FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
    COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <png.h>
#include <pngw/png_wrapper.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  size_t width, height, file_depth;
  pngwcolor_t file_color;
  pngwresult_t result = pngwFileInfo("dude.png", &width, &height, &file_depth, &file_color);
  if (result != PNGW_RESULT_OK)
  {
    printf("an error has occured: %s\n", PNGW_RESULT_DESCRIPTIONS[result]);
    return 1;
  }
  printf("got dude.png info: width=%zu height=%zu depth=%zu color=%s\n", width, height, file_depth, PNGW_COLOR_NAMES[file_color]);

  size_t size;
  const size_t load_depth = 16;
  const pngwcolor_t load_color = PNGW_COLOR_RGB;
  result = pngwDataSize(width, height, load_depth, load_color, &size);
  if (result != PNGW_RESULT_OK)
  {
    printf("an error has occured: %s\n", PNGW_RESULT_DESCRIPTIONS[result]);
    return 3;
  }
  printf("determined dude.png pixel data size: %zu bytes\n", size);

  pngwb_t* data = malloc(size);
  if (!data)
  {
    printf("an error has occured: out of memory\n");
    return 4;
  }

  printf("loading dude.png with convert on load: width=%zu, height=%zu depth=%zu color=%s\n", width, height,
         load_depth, PNGW_COLOR_NAMES[load_color]);
  result = pngwReadFile("dude.png", data, PNGW_DEFAULT_ROW_OFFSET, width, height, load_depth,
                     load_color);
  if (result != PNGW_RESULT_OK)
  {
    printf("an error has occured: %s\n", PNGW_RESULT_DESCRIPTIONS[result]);
    return 5;
  }

  printf("saving new_dude.png to a file\n");
  result = pngwWriteFile("new_dude.png", data, PNGW_DEFAULT_ROW_OFFSET, width, height, load_depth,
                      load_color);
  if (result != PNGW_RESULT_OK)
  {
    printf("an error has occured: %s\n", PNGW_RESULT_DESCRIPTIONS[result]);
    return 6;
  }

  return 0;
}
