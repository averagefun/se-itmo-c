#include "bmp.h"

#include <stdbool.h>

#include "image.h"

#define BMP_TYPE 0x4d42  // BM
#define BMP_RESERVED 0
#define BMP_HEADER_SIZE 40
#define BMP_PLANES 1
#define BMP_BIT_COUNT 24
#define BMP_COMPRESSION 0
#define BMP_X_PELS_PER_METER 0
#define BMP_Y_PELS_PER_METER 0
#define BMP_CLR_USED 0
#define BMP_CLR_IMPORTANT 0

struct __attribute__((packed)) bmp_header {
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

static uint8_t bmp_calc_padding(uint64_t width) {
    return (4 - (width * 3) % 4) % 4;
}

/* return true if succesfully reading bmp reader  */
static bool bmp_read_header(struct bmp_header* bmp_header, FILE* in) {
    return fread(bmp_header, sizeof(struct bmp_header), 1, in) == 1;
}

enum read_status from_bmp(FILE* in, struct image* img) {
    if (in == NULL) {
        return READ_INVALID_FILE;
    }

    struct bmp_header bmp_header;
    if (!bmp_read_header(&bmp_header, in)) {
        return READ_INVALID_HEADER;
    }

    // FileBegin + bOffBits = BitMap
    // fseek -> 0 if success
    if (fseek(in, bmp_header.bOffBits, SEEK_SET) != 0) {
        return READ_INVALID_SIGNATURE;
    }

    // create empty image (set width and height)
    *img = image_create(bmp_header.biWidth, bmp_header.biHeight);

    uint8_t img_padding = bmp_calc_padding(img->width);

    // fill image with pixels
    for (uint32_t row = 0; row < img->height; row++) {
        if (fread(img->data + img->width * row, sizeof(struct pixel),
                  img->width, in) != img->width) {
            return READ_INVALID_BITS;
        }

        // fseek -> 0 if success
        if (fseek(in, img_padding, SEEK_CUR) != 0) {
            return READ_INVALID_BITS;
        }
    }

    return READ_OK;
}

static uint32_t bmp_calc_image_size(uint32_t width, uint32_t height) {
    return height * (width * sizeof(struct pixel) + bmp_calc_padding(width));
}

static uint32_t bmp_calc_file_size(uint32_t width, uint32_t height) {
    return bmp_calc_image_size(width, height) + sizeof(struct bmp_header);
}

static struct bmp_header bmp_gen_header(const struct image* img) {
    return (struct bmp_header){
        .bfType = BMP_TYPE,
        .bfileSize = bmp_calc_file_size(img->width, img->height),
        .bfReserved = BMP_RESERVED,
        .bOffBits = sizeof(struct bmp_header),
        .biSize = BMP_HEADER_SIZE,
        .biWidth = img->width,
        .biHeight = img->height,
        .biPlanes = BMP_PLANES,
        .biBitCount = sizeof(struct pixel) * 8,
        .biCompression = BMP_COMPRESSION,
        .biSizeImage = bmp_calc_image_size(img->width, img->height),
        .biXPelsPerMeter = BMP_X_PELS_PER_METER,
        .biYPelsPerMeter = BMP_Y_PELS_PER_METER,
        .biClrUsed = BMP_CLR_USED,
        .biClrImportant = BMP_CLR_IMPORTANT};
}

enum write_status to_bmp(FILE* out, struct image img) {
    if (out == NULL) {
        return WRITE_INVALID_FILE;
    }

    struct bmp_header bmp_header = bmp_gen_header(&img);
    fwrite(&bmp_header, sizeof(struct bmp_header), 1, out);

    // ferror -> 0 if success
    if (ferror(out) != 0) {
        return WRITE_HEADER_FAILED;
    }

    uint8_t img_padding = bmp_calc_padding(img.width);

    for (uint32_t row = 0; row < img.height; row++) {
        // write img data
        fwrite(img.data + img.width * row, sizeof(struct pixel), img.width,
               out);

        // write padding
        // zero_bytes is 3 seq zero bytes (because padding <= 3)
        const char zero_bytes[3] = {0, 0, 0};
        fwrite(zero_bytes, 1, img_padding, out);
    }

    return WRITE_OK;
}
