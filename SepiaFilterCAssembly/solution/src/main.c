#include <stdio.h>
#include <time.h>

#include "bmp.h"
#include "image.h"
#include "processor.h"
#include "transformator.h"
#include "transformator_asm.h"

long get_bmp_transform_time(transformator my_transformator,
                            const char *input_path, const char *output_path) {
    struct processing_steps bmp_rotate_steps = {
        .reader = from_bmp,
        .transformator = my_transformator,
        .writer = to_bmp};

    struct io_files my_files = {.input_path = input_path,
                                .output_path = output_path};

    clock_t begin = clock();
    if (!process(bmp_rotate_steps, my_files)) {
        return -1;
    }
    clock_t end = clock();
    return (end - begin) * 1000 / CLOCKS_PER_SEC;
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

    long time_asm = get_bmp_transform_time(image_apply_sepia_asm, input_path,
                                           output_asm_path);
    long time_c =
        get_bmp_transform_time(image_apply_sepia, input_path, output_c_path);

    printf("- ASM: %ld ms\n", time_asm);
    printf("- C: %ld ms\n", time_c);

    return 0;
}
