#include "image.h"

#include <malloc.h>

struct image image_create(uint32_t width, uint32_t height) {
    return (struct image){
        .width = width,
        .height = height,
        .data = malloc(width * height * sizeof(struct pixel))};
}

void image_destroy(struct image* img) { free(img->data); }
