#include "transformator_asm.h"

#include "image.h"

struct image image_apply_sepia_asm(struct image const source) {
    struct image new_img = image_create(source.width, source.height);

    int n = image_apply_sepia_asm_impl(&new_img);
    printf("Return code from assembler: %d\n", n);

    return new_img;
}