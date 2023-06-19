#include "transformator_asm.h"

#include "image.h"

struct image image_apply_sepia_asm(struct image const source) {
    struct image new_img = image_create(source.width, source.height);

    size_t pixels_num = source.width * source.height;
    size_t quad_num = pixels_num / 4;

    image_apply_sepia_asm_impl(source.data, new_img.data, quad_num);

    return new_img;
}