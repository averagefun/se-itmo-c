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
