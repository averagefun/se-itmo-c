#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

struct pixel {
    uint8_t b, g, r;
};

struct image {
    uint64_t width, height;
    struct pixel* data;
};

/* create image (width x height) and allocate memory for image data */
struct image image_create(uint32_t width, uint32_t height);

/* clear memory, that allocated for the img */
void image_destroy(struct image* img);

#endif
