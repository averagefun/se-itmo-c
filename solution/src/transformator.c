#include "transformator.h"

#include "image.h"

struct image image_rotate(struct image const source) {
    struct image new_img = image_create(source.height, source.width);

    for (uint32_t row = 0; row < new_img.height; row++) {
        for (uint32_t col = 0; col < new_img.width; col++) {
            *(new_img.data + new_img.width * row + col) =
                *(source.data + source.width * (source.height - 1 - col) + row);
        }
    }

    return new_img;
}

static struct pixel pixel_apply_sepia(struct pixel pixel) {
    double tone = 0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b;

    // double sr = (tone > 206) ? 255 : tone + 49;  // sr - sepia red
    double sg = (tone < 14) ? 0 : tone - 14;  // sg - sepia green
    double sb = (tone < 56) ? 0 : tone - 56;  // sb - sepia blue

    double sr = tone + 49;  // sr - sepia red

    return (struct pixel){
        .b = (uint8_t)sb, .g = (uint16_t)sg, .r = (uint8_t)sr};
}

struct image image_apply_sepia(struct image const source) {
    struct image new_img = image_create(source.width, source.height);

    for (uint32_t row = 0; row < source.height; row++) {
        for (uint32_t col = 0; col < source.width; col++) {
            *(new_img.data + new_img.width * row + col) =
                pixel_apply_sepia(*(source.data + source.width * row + col));
        }
    }

    return new_img;
}