#include <stdio.h>

#include "bmp.h"
#include "image.h"
#include "processor.h"
#include "transformator.h"
#include "transformator_asm.h"

bool test_bmp_transform(transformator my_transformator, const char *input_path,
                        const char *output_path) {
    struct processing_steps bmp_rotate_steps = {
        .reader = from_bmp,
        .transformator = my_transformator,
        .writer = to_bmp};

    struct io_files my_files = {.input_path = input_path,
                                .output_path = output_path};

    return process(bmp_rotate_steps, my_files);
}

int main(int argc, char **argv) {
    // supress 'unused parameters' warning
    (void)argc;
    (void)argv;

    if (argc != 4) {
        printf("Error: 3 arguments required!\n");
        return 1;
    }
    const char *input_path = argv[1];
    const char *output_c_path = argv[2];
    const char *output_asm_path = argv[3];

    if (!test_bmp_transform(image_apply_sepia, input_path, output_c_path)) {
        return 1;
    }

    if (!test_bmp_transform(image_apply_sepia_asm, input_path,
                            output_asm_path)) {
        return 1;
    }

    return 0;
}
