#include "libjpg_reader.hpp"

#include <jpeglib.h>

#include <cstdio>
#include <stdexcept>

Image ReadJpg(const std::string& filename) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr err;
    FILE* infile = fopen(filename.c_str(), "rb");

    if (!infile) {
        throw std::runtime_error("can't open " + filename);
    }

    cinfo.err = jpeg_std_error(&err);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);

    (void)jpeg_read_header(&cinfo, static_cast<boolean>(true));
    (void)jpeg_start_decompress(&cinfo);

    int row_stride = cinfo.output_width * cinfo.output_components;
    JSAMPARRAY buffer =
        (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);  // NOLINT

    Image result(cinfo.output_width, cinfo.output_height);
    size_t y = 0;

    while (cinfo.output_scanline < cinfo.output_height) {
        (void)jpeg_read_scanlines(&cinfo, buffer, 1);
        for (size_t x = 0; x < result.Width(); ++x) {
            RGB pixel;
            if (cinfo.output_components == 3) {
                pixel.r = buffer[0][x * 3];
                pixel.g = buffer[0][x * 3 + 1];
                pixel.b = buffer[0][x * 3 + 2];
            } else {
                pixel.r = pixel.g = pixel.b = buffer[0][x];
            }
            result.SetPixel(y, x, pixel);
        }
        ++y;
    }

    (void)jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
    return result;
}
