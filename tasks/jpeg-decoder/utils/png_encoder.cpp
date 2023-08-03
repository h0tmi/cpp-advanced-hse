#include "png_encoder.hpp"

#include <png.h>

#include <string>
#include <stdexcept>

void WritePng(const std::string& filename, const Image& image) {
    FILE* fp = fopen(filename.c_str(), "wb");
    if (!fp) {
        throw std::runtime_error("Can't open file for writing " + filename);
    }
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);  // NOLINT
    png_infop info = png_create_info_struct(png);
    if (setjmp(png_jmpbuf(png))) {
        throw std::runtime_error("Shit happens");
    }

    png_init_io(png, fp);

    png_set_IHDR(png, info, image.Width(), image.Height(), 8, PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    png_bytep* bytes = (png_bytep*)malloc(sizeof(png_bytep) * image.Height());  // NOLINT
    for (size_t y = 0; y < image.Height(); y++) {
        bytes[y] = (png_byte*)malloc(png_get_rowbytes(png, info));  // NOLINT
        for (size_t x = 0; x < image.Width(); ++x) {
            auto pixel = image.GetPixel(y, x);
            bytes[y][x * 4] = pixel.r;
            bytes[y][x * 4 + 1] = pixel.g;
            bytes[y][x * 4 + 2] = pixel.b;
            bytes[y][x * 4 + 3] = 255;
        }
    }
    png_write_image(png, bytes);
    png_write_end(png, NULL);  // NOLINT

    fclose(fp);
    png_destroy_write_struct(&png, &info);

    for (size_t i = 0; i < image.Height(); ++i) {
        free(bytes[i]);
    }
    free(bytes);
}
