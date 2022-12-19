// SPDX-FileCopyrightText: 2022 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

/*
    Copyright (c) 2022 Daniel Aimé Valcour
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

/*
   png_wrapper.h v1.0.0
   Easy to use wrapper arround libpng
   The source for this library can be found on GitHub:
   https://github.com/Journeyman-dev/png_wrapper.h

    HOW TO SETUP
    The png_wrapper.h library can be included in your project in one of three different ways:
        - Copy and paste the png_wrapper.h file directly into your source tree.
        - Clone the GitHub as a git submodule to your project's repository.
          1. In bash console from the root directory of your project's repository:
                git submodule add https://github.com/Journeyman-dev/png_wrapper.h
                git submodule update --init
          2. In your project's top level CMakeLists.txt:
                set(PNGW_BUILD_EXAMPLE OFF) # Set to ON to build the example target.
                set(PNGW_EXAMPLE_AUTO_FETCH OFF) # Set to ON to fetch the example dependencies (see bellow).
                add_subdirectory(png_wrapper.h)
          3. Link it with the CMake target of your project:
               target_link_libraries(YOUR_TARGET_NAME PRIVATE pngw::pngw)
        - Fetch it using CMake FetchContent:
          1. Fetch it in your project's CMakeLists.txt:
                Include(FetchContent)
                FetchContent_Declare(
                        pngw
                        GIT_REPOSITORY https://github.com/Journeyman-dev/png_wrapper.h
                        GIT_TAG        v1.0.0 # replace with the version that you want
                        FIND_PACKAGE_ARGS
                )
                set(PNGW_BUILD_EXAMPLE OFF) # Set to ON to build the example target.
                set(PNGW_EXAMPLE_AUTO_FETCH OFF) # Set to ON to fetch the example dependencies (see bellow).
                FetchContent_MakeAvailable(pngw)
          2. Link it with the CMake target of your project:
                target_link_libraries(YOUR_TARGET_NAME PRIVATE pngw::pngw)

   If you choose to link with png_wrapper.h using its CMake generation scripts, note that the
   png_wrapper.h example requires to be linked with libpng. By default, CMake will try
   to find libpng using the function call find_package(PNG). A good option for making a
   libpng package avaliable is to add it using vcpkg. If you want to get png_wrapper.h up and
   running quickly, you can turn the CMake option PNGW_EXAMPLE_AUTO_FETCH to ON, and CMake will
   download libpng for the example project from github using FetchContent.

   After png_wrapper.h is included in your project, you must implement the library before you can
   actually use it. To implement png_wrapper.h, create a new .c or .cpp file and write in it the
   following:

           #include <png.h> // libpng include (see bellow)
           #define PNGW_IMPLEMENTATION
           #include <pngw/png_wrapper.h>

   Since png_wrapper.h is a wrapper around libpng, it shouldn't be surprising that libpng must also
   be included as a dependency to your project. You must include png.h, the main header for libpng,
   before you can implement png_wrapper.h.

   HOW TO DEBUG
   Many functions in png_wrapper.h return an enum value of type pngwresult_t. Result codes with
   names that start with PNGW_RESULT_ERROR_ are returned if an error occured in the function's
   execution. The only non-error result code is PNGW_RESULT_OK. String descriptions of result codes
   are stored in the constant string array variable PNGW_RESULT_DESCRIPTIONS. To get the string
   description of a specific result code from this array, index the array with the result code.

   This is an example of how to check for errors and print the result code from a function:

       pngwresult_t result = pngwFunction(arg0, arg1, arg2);
       printf("function result description: %s\n", PNGW_RESULT_DESCRIPTIONS[result]);
       if (result != PNGW_RESULT_OK)
       {
           printf("that was an error!\n");
           return 1;
       }

   HOW TO USE
   Usage of png_wrapper.h should be familliar to previous users of the stb libraries. However, one
   major difference from stb libraries is that no allocations are done by the library itself. This
   means that you must allocate the byte array to store the image bytes into yoruself. The first
   step of doing this is to get info about a png file using the function pngwFileInfo() like so:

           size_t image_width, image_height, image_depth;
           pngwcolor_t image_color;
           pngwresult_t result = pngwFileInfo(image_path_cstr, &image_width, &image_height,
                                 &image_depth, &image_color);
           if (result != PNGW_RESULT_OK)
           {
               printf("error getting png file info: %s\n", PNGW_RESULT_DESCRIPTIONS[result]);
               return 1;
           }

   With all of those variables filled with the image data, now its possible to calculate the amount
   of bytes requires to store the image pixels. png_wrapper.h includes the function pngwDataSize()
   to help you do just that. It is possible to convert the color type and bit depth on load. In
   this example, the bit depth will be converted to 8 bit depth on load, and the color will be
   converted to PNGW_COLOR_RGB on load. It is important for the byte array size to reflect the
   converted size, not the size of the actual image file format bytes, so these are passed into the
   next function instead.

           // this continues from the above code block
           size_t load_depth = 8;
           pngwcolor_t load_color = PNGW_COLOR_RGB;
           size_t size = 0;
           pngwresult_t result = pngwDataSize(width, height, load_depth, load_color, &size);
           if (result != PNGW_RESULT_OK)
           {
               printf("error getting png image size: %s\n", PNGW_RESULT_DESCRIPTIONS[result]);
               return 1;
           }

   Using the size, it is now possible to allocate a byte array that is large enough to contain the
   image bytes. This can be done using malloc from stdlib.h.

           // this continues from the above code block
           pngwbyte_t* bytes = malloc(size);
           if (bytes == NULL)
           {
               printf("error allocating image bytes: out of memory\n");
               return 1;
           }

   After the byte array is allocated, the bytes can be read using pngwReadFile(). Since we are
   simply reading an image without row offsets, for the row offset parameter
   PNGW_DEFAULT_ROW_OFFSET should be used. If you wish to load an image in a rectangular region
   within an image of larger dimensions, change the PNGW_DEFAULT_ROW_OFFSET to be equal the the
   amount of bytes in a single row of the larger image.

           // this continues from the above code block
           pngwresult_t result = pngwReadFile(image_path_cstr, bytes, PNGW_DEFAULT_ROW_OFFSET,
             image_width, image_height, load_depth, load_color);
           if (result != PNGW_RESULT_OK)
           {
               printf("error reading image file bytes: %s\n", PNGW_RESULT_DEXCRIPTIONS[result]);
               return 1;
           }

   Note that the pngwReadFile() and  pngwDataSize() functions only accept 8 and 16 bit depths, and
   do not accept PNGW_COLOR_PALETTE for the color type. While png files themselves may have other
   bit depths of may use palette colors, png files cannot be loaded in this way and must be
   converted on load.

   To write an image bytes to a new file, pngwWriteFile() function can be used. Images can not be
   converted on write, so the arguments passed in must match the image bytes exactly. Just like
   with loading, you can only save images with 8 or 16 bit depth and can not save images with
   PNGW_COLOR_PALETTE color type.

           // this continues from the above code block
           pngwresult_t result = pngwWriteFile(new_image_path_cstr, bytes, PNGW_DEFAULT_ROW_OFFSET,
                bytes_width, bytes_height, bytes_depth, bytes_color);
           if (result != PNGW_RESULT_OK)
           {
               printf("error writting image file bytes: %s\n", PNGW_RESULT_DESCRIPTIONS[result]);
               return 1;
           }

   Just like results, color type enum values also have a const char string array lookup table for
   string names.

           pngwcolor_t color = PNGW_COLOR_RGB;
           printf("the name of the color type is %s\n", PNGW_COLOR_NAMES[color]);

    When converting from rgb colors to grayscale on load, png_wrapper.h uses the default grayscale
    conversion equation that is built into libpng. If you want to use this same equation to convert
    image bytes within your code, you can use the functions pngGrayFromColor8() for 8 bit image bytes
    and pngGrayFromColor16() for 16 bit image bytes. For converting from grayscale to rgb, simply
    use the grayscape value for all three color channels.

    If you wish to convert between pngwcolor_t and libpng color type macros, you can use the functions
    pngwColorToPngColor() and pngwPngColorToColor().

   CHANGELOG
   - Version 1.0
       Initial Release
 */

#ifndef PNGW_H
#define PNGW_H
#ifdef __cplusplus
extern "C"
{
#endif
#include <stddef.h>
#include <stdint.h>

  typedef uint8_t pngwb_t;
  typedef uint16_t pngws_t;

  typedef enum pngwresult_t
  {
    PNGW_RESULT_OK = 0,
    PNGW_RESULT_ERROR_FILE_NOT_FOUND = 1,
    PNGW_RESULT_ERROR_FILE_CREATION_FAILURE = 2,
    PNGW_RESULT_ERROR_OUT_OF_MEMORY = 3,
    PNGW_RESULT_ERROR_INVALID_FILE_SIGNITURE = 4,
    PNGW_RESULT_ERROR_JUMP_BUFFER_CALLED = 5,
    PNGW_RESULT_ERROR_NULL_ARG = 6,
    PNGW_RESULT_ERROR_INVALID_DEPTH = 7,
    PNGW_RESULT_ERROR_INVALID_COLOR = 8,
    PNGW_RESULT_ERROR_INVALID_DIMENSIONS = 9,
    PNGW_RESULT_COUNT = 10
  } pngwresult_t;

  // array of error descriptions, indexable by pngwresult_t enum values.
  extern const char* const PNGW_RESULT_DESCRIPTIONS[PNGW_RESULT_COUNT];

  typedef enum pngwcolor_t
  {
    PNGW_COLOR_PALETTE = 0,
    PNGW_COLOR_G = 1,
    PNGW_COLOR_GA = 2,
    PNGW_COLOR_RGB = 3,
    PNGW_COLOR_RGBA = 4,
    PNGW_COLOR_COUNT = 5,
  } pngwcolor_t;

  // array of color type names, indexable by pngwcolor_t enum values.
  extern const char* const PNGW_COLOR_NAMES[PNGW_COLOR_COUNT];

#define PNGW_DEFAULT_ROW_OFFSET 0

  // Get information about a png image file's format. Depth may be 1, 2, 4, 8, or 16. Color may be
  // any type.
  pngwresult_t pngwFileInfo(const char* const path, size_t* const width, size_t* const height,
                            size_t* const depth, pngwcolor_t* const color);

  // Get the size of image data in bytes. Depth must be 8 or 16. Color may not be
  // PNGW_COLOR_PALETTE.
  pngwresult_t pngwDataSize(const size_t width, const size_t height, const size_t depth,
                            const pngwcolor_t color, size_t* const size);

  // Read png data from a file into a pixel byte array with the specified format. Data should be
  // allocated before this function is called with enough space to contain the bytes. If the file is
  // of a different format than specified in the arguments, the image will be converted on load.
  // Depth must be 8 or 16. Color may not be PNGW_COLOR_PALETTE. Width and height must match the
  // actual width and height of the image, which you can retrieve with pngwFileInfo() before
  // loading.
  pngwresult_t pngwReadFile(const char* const path, pngwb_t* const data, const size_t row_offset,
                            const size_t width, const size_t height, const size_t depth,
                            const pngwcolor_t color);

  // Save png data to a file from a pixel byte array. The width, height, depth and color must be be
  // the same as the format of data bytes.
  pngwresult_t pngwWriteFile(const char* path, const pngwb_t* const data, const size_t row_offset,
                             const size_t width, const size_t height, const size_t depth,
                             const pngwcolor_t color);

  // Convert an 8 bit depth RGB color to a grayscale value using libpng's default conversion
  // equation.
  pngwb_t pngGrayFromColor8(const pngwb_t r, const pngwb_t g, const pngwb_t b);

  // Convert a 16 bit depth RGB color to a grayscale value using libpng's default conversion
  // equation.
  pngws_t pngGrayFromColor16(const pngws_t r, const pngws_t g, const pngws_t b);

  // Get the libpng color macro of a pngw color type.
  int pngwColorToPngColor(const pngwcolor_t color);

  // Get the pngw_color enum of a libpng color macro.
  pngwcolor_t pngwPngColorToColor(const int png_color);

#ifdef PNGW_IMPLEMENTATION
#  ifndef PNGW_IMPLEMENTED
#    define PNGW_IMPLEMENTED

#    include <stdio.h>

#    ifndef PNG_H
#      error png.h must be included before png_wrapper.h can be implemented.
#    endif

  const char* const PNGW_RESULT_DESCRIPTIONS[PNGW_RESULT_COUNT] = {
      "no error has occured",    "file not found at path", "failed to create file",
      "out of memory",           "invalid file signiture", "jump buffer called",
      "NULL argument",           "invalid bit depth",      "invalid color type",
      "invalid pixel dimensions"};

  const char* const PNGW_COLOR_NAMES[PNGW_COLOR_COUNT] = {"Palette", "G", "GA", "RGB", "RGBA"};

  pngwresult_t pngwFileInfo(const char* const path, size_t* const width, size_t* const height,
                            size_t* const depth, pngwcolor_t* const color)
  {
    if (path == NULL)
    {
      return PNGW_RESULT_ERROR_NULL_ARG;
    }
    FILE* f = fopen(path, "rb");
    if (f == NULL)
    {
      return PNGW_RESULT_ERROR_FILE_NOT_FOUND;
    }
    char signiture[8];
    fread(signiture, 1, 8, f);
    if (png_sig_cmp((png_const_bytep)&signiture[0], 0, 8))
    {
      fclose(f);
      return PNGW_RESULT_ERROR_INVALID_FILE_SIGNITURE;
    }
    png_structp png_ptr;
    png_infop info_ptr;
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
      fclose(f);
      return PNGW_RESULT_ERROR_OUT_OF_MEMORY;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
      fclose(f);
      png_destroy_read_struct(&png_ptr, NULL, NULL);
      return PNGW_RESULT_ERROR_OUT_OF_MEMORY;
    }
    png_init_io(png_ptr, f);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    png_uint_32 png_width, png_height;
    int png_bit_depth, png_color_type;
    png_get_IHDR(png_ptr, info_ptr, &png_width, &png_height, &png_bit_depth, &png_color_type, NULL,
                 NULL, NULL);
    fclose(f);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    if (width != NULL)
    {
      *width = (size_t)png_width;
    }
    if (height != NULL)
    {
      *height = (size_t)png_height;
    }
    if (depth != NULL)
    {
      *depth = (size_t)png_bit_depth;
    }
    if (color != NULL)
    {
      *color = pngwPngColorToColor(png_color_type);
    }
    return PNGW_RESULT_OK;
  }

  pngwresult_t pngwDataSize(const size_t width, const size_t height, const size_t depth,
                            const pngwcolor_t color, size_t* const size)
  {
    if (!(color >= PNGW_COLOR_G && color <= PNGW_COLOR_RGBA))
    {
      return PNGW_RESULT_ERROR_INVALID_COLOR;
    }
    if (depth != 8 && depth != 16)
    {
      return PNGW_RESULT_ERROR_INVALID_DEPTH;
    }
    if (width == 0 || height == 0)
    {
      return PNGW_RESULT_ERROR_INVALID_DIMENSIONS;
    }
    if (size != NULL)
    {
      *size = width * height * (depth / 8) * ((size_t)color);
    }
    return PNGW_RESULT_OK;
  }

  pngwresult_t pngwReadFile(const char* const path, pngwb_t* const data, const size_t row_offset,
                            const size_t width, const size_t height, const size_t depth,
                            const pngwcolor_t color)
  {
    if (path == NULL || data == NULL)
    {
      return PNGW_RESULT_ERROR_NULL_ARG;
    }
    /* Initial arg checks */
    if (!(color >= PNGW_COLOR_G && color <= PNGW_COLOR_RGBA))
    {
      return PNGW_RESULT_ERROR_INVALID_COLOR;
    }
    if (depth != 8 && depth != 16)
    {
      return PNGW_RESULT_ERROR_INVALID_DEPTH;
    }
    /* Open file */
    FILE* f = fopen(path, "rb");
    if (f == NULL)
    {
      return PNGW_RESULT_ERROR_FILE_NOT_FOUND;
    }
    /* Check file signiture */
    char signiture[8];
    fread(signiture, 1, 8, f);
    if (png_sig_cmp((png_const_bytep)&signiture[0], 0, 8))
    {
      fclose(f);
      return PNGW_RESULT_ERROR_INVALID_FILE_SIGNITURE;
    }
    /* Create libpng structs */
    png_structp png_ptr;
    png_infop info_ptr;
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
      fclose(f);
      return PNGW_RESULT_ERROR_OUT_OF_MEMORY;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
      fclose(f);
      png_destroy_read_struct(&png_ptr, NULL, NULL);
      return PNGW_RESULT_ERROR_OUT_OF_MEMORY;
    }
    /* Create jump buffer to handle errors */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
      fclose(f);
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      return PNGW_RESULT_ERROR_JUMP_BUFFER_CALLED;
    }
    /* Get png format from file */
    png_init_io(png_ptr, f);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    png_uint_32 png_width, png_height;
    int png_bit_depth, png_color_type;
    png_get_IHDR(png_ptr, info_ptr, &png_width, &png_height, &png_bit_depth, &png_color_type, NULL,
                 NULL, NULL);
    if (width != (size_t)png_width || (size_t)png_height == 0)
    {
      fclose(f);
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      return PNGW_RESULT_ERROR_INVALID_DIMENSIONS;
    }
    int load_png_color_type = pngwColorToPngColor(color);
    int load_png_bit_depth = (int)depth;
    // if alpha channel not wanted, strip it if the image has one.
    if ((png_color_type & PNG_COLOR_MASK_ALPHA) && !(load_png_color_type & PNG_COLOR_MASK_ALPHA))
    {
      png_set_strip_alpha(png_ptr);
    }
    // if expecting an alpha channel and none exists in the image, add a fully opaque alpha value to
    // each pixel
    if ((load_png_color_type & PNG_COLOR_MASK_ALPHA) &&
        (png_color_type == PNG_COLOR_TYPE_GRAY || png_color_type == PNG_COLOR_TYPE_RGB ||
         png_color_type == PNG_COLOR_TYPE_PALETTE))
    {
      png_set_filler(png_ptr, 0xffff, PNG_FILLER_AFTER);
    }
    // if multiple pixels are packed per byte on 8 bit depth, seperate them into seperate bytes
    // cleanly
    if (png_bit_depth < 8 && load_png_bit_depth == 8)
    {
      png_set_packing(png_ptr);
    }
    // if the image has bit depth 16 and 8 is wanted, auto convert it to bit depth 8
    if (png_bit_depth == 16 && load_png_bit_depth == 8)
    {
#    ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
      png_set_scale_16(png_ptr);
#    else
      png_set_strip_16(png_ptr);  // if scaling not supported, strip off the excess byte instead
#    endif
    }
    // if image has less than 16 bit depth and 16 is wanted, upscale it to 16
    if (png_bit_depth < 16 && load_png_bit_depth == 16)
    {
      png_set_expand_16(png_ptr);
    }
    // if gray and bit depth is less than 8, up it to 8
    if ((png_color_type == PNG_COLOR_TYPE_GRAY || png_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) &&
        png_bit_depth < 8)
    {
      png_set_expand_gray_1_2_4_to_8(png_ptr);
    }
    // auto convert palette images into rgb or rgba
    if (png_color_type == PNG_COLOR_TYPE_PALETTE)
    {
      png_set_palette_to_rgb(png_ptr);
    }
    // set transparency to full alpha channels
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS) != 0)
    {
      png_set_tRNS_to_alpha(png_ptr);
    }
    // set rgb image to gray output or vice versa
    if ((load_png_color_type == PNG_COLOR_TYPE_GRAY ||
         load_png_color_type == PNG_COLOR_TYPE_GRAY_ALPHA) &&
        (png_color_type == PNG_COLOR_TYPE_RGB || png_color_type == PNG_COLOR_TYPE_RGB_ALPHA ||
         png_color_type == PNG_COLOR_TYPE_PALETTE))
    {
      // negative weights causes default calculation to be used  ((6969 * R + 23434 * G + 2365 *
      // B)/32768)
      png_set_rgb_to_gray_fixed(
          png_ptr, 1, -1.0,
          -1.0);  // error action 1 causes no waring warning if image was not actually gray
    }
    if ((load_png_color_type == PNG_COLOR_TYPE_RGB || load_png_color_type == PNG_COLOR_TYPE_RGBA) &&
        (png_color_type == PNG_COLOR_TYPE_GRAY || png_color_type == PNG_COLOR_TYPE_GRAY_ALPHA))
    {
      png_set_gray_to_rgb(png_ptr);
    }
    int actual_row_offset;
    if (row_offset == PNGW_DEFAULT_ROW_OFFSET)
    {
      actual_row_offset = width * (size_t)color * (depth / 8);
    }
    else
    {
      actual_row_offset = row_offset;
    }
    /* Load the pixels */
    for (size_t y = 0; y < height; y++)
    {
      png_bytep row_start = &data[y * actual_row_offset];
      png_read_row(png_ptr, row_start, NULL);
    }
    /* Cleanup */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(f);
    return PNGW_RESULT_OK;
  }

  pngwresult_t pngwWriteFile(const char* path, const pngwb_t* const data, const size_t row_offset,
                             const size_t width, const size_t height, const size_t depth,
                             const pngwcolor_t color)
  {
    if (path == NULL || data == NULL)
    {
      return PNGW_RESULT_ERROR_NULL_ARG;
    }
    /* Create the file */
    FILE* f = fopen(path, "wb");
    if (!f)
    {
      return PNGW_RESULT_ERROR_FILE_CREATION_FAILURE;
    }
    /* Create libpng structs */
    png_structp png_ptr;
    png_infop info_ptr;
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
      fclose(f);
      return PNGW_RESULT_ERROR_OUT_OF_MEMORY;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
      fclose(f);
      png_destroy_read_struct(&png_ptr, NULL, NULL);
      return PNGW_RESULT_ERROR_OUT_OF_MEMORY;
    }
    /* Create jump buffer to handle errors */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
      fclose(f);
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      return PNGW_RESULT_ERROR_JUMP_BUFFER_CALLED;
    }
    const int png_color_type = pngwColorToPngColor(color);
    /* Configure for writing */
    png_init_io(png_ptr, f);
    // Set the compression to a setup that will be good enough. No need for more complicated options
    // in this library.
    png_set_IHDR(png_ptr, info_ptr, (uint32_t)width, (uint32_t)height, (int)depth, png_color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png_ptr, info_ptr);
    int actual_row_offset;
    if (row_offset == PNGW_DEFAULT_ROW_OFFSET)
    {
      actual_row_offset = width * (size_t)color * (depth / 8);
    }
    else
    {
      actual_row_offset = row_offset;
    }
    for (size_t y = 0; y < height; y++)
    {
      png_const_bytep row_start = data + (y * actual_row_offset);
      png_write_row(png_ptr, row_start);
    }
    png_write_end(png_ptr, NULL);
    fclose(f);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return PNGW_RESULT_OK;
  }

  pngwb_t pngGrayFromColor8(const pngwb_t r, const pngwb_t g, const pngwb_t b)
  {
    return ((((6969 * ((uint32_t)r))) + (23434 * ((uint32_t)g)) + (2365 * ((uint32_t)b))) / 32768);
  }

  pngws_t pngGrayFromColor16(const pngws_t r, const pngws_t g, const pngws_t b)
  {
    return ((((6969 * ((uint64_t)r))) + (23434 * ((uint64_t)g)) + (2365 * ((uint64_t)b))) / 32768);
  }

  int pngwColorToPngColor(const pngwcolor_t color)
  {
    switch (color)
    {
    case PNGW_COLOR_PALETTE:
      return PNG_COLOR_TYPE_PALETTE;
    case PNGW_COLOR_G:
      return PNG_COLOR_TYPE_GRAY;
    case PNGW_COLOR_GA:
      return PNG_COLOR_TYPE_GRAY_ALPHA;
    case PNGW_COLOR_RGB:
      return PNG_COLOR_TYPE_RGB;
    case PNGW_COLOR_RGBA:
      return PNG_COLOR_TYPE_RGBA;
    default:
      return PNG_COLOR_TYPE_GRAY;
    }
  }

  pngwcolor_t pngwPngColorToColor(const int png_color)
  {
    switch (png_color)
    {
    case PNG_COLOR_TYPE_PALETTE:
      return PNGW_COLOR_PALETTE;
    case PNG_COLOR_TYPE_GRAY:
      return PNGW_COLOR_G;
    case PNG_COLOR_TYPE_GRAY_ALPHA:
      return PNGW_COLOR_GA;
    case PNG_COLOR_TYPE_RGB:
      return PNGW_COLOR_RGB;
    case PNG_COLOR_TYPE_RGBA:
      return PNGW_COLOR_RGBA;
    default:
      return PNGW_COLOR_G;
    }
  }

#  endif
#endif
#ifdef __cplusplus
}
#endif
#endif
