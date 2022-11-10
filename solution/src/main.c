#include <stdio.h>

#include "bmp.h"
#include "image.h"
#include "processor.h"
#include "rotator.h"

int main(int argc, char **argv) {
    // supress 'unused parameters' warning
    (void)argc;
    (void)argv;

    if (argc != 3) {
        printf("Error: 2 arguments required!\n");
        return 1;
    }
    const char *input_path = argv[1];
    const char *output_path = argv[2];

    struct processing_steps bmp_rotate_steps = {
        .reader = from_bmp, .transformator = image_rotate, .writer = to_bmp};

    struct io_files my_files = {.input_path = input_path,
                                .output_path = output_path};

    if (!process(bmp_rotate_steps, my_files)) {
        return 1;
    }

    return 0;
}
