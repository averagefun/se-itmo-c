#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdbool.h>
#include <stdio.h>

#include "image.h"
#include "io_utils.h"

typedef struct image(transformator)(struct image const);

// specify steps that processor will do
struct processing_steps {
    from_format_reader* reader;
    transformator* transformator;
    to_format_writer* writer;
};

struct io_files {
    const char* input_path;
    const char* output_path;
};

// processing steps (including clean memory)
bool process(struct processing_steps processing_steps,
             struct io_files io_files);

#endif
