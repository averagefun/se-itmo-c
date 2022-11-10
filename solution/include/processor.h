#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdbool.h>
#include <stdio.h>

#include "image.h"
#include "io_utils.h"

// specify steps that processor will do
struct processing_steps {
    enum read_status (*from_format)(FILE*, struct image*);
    struct image (*transformator)(struct image const);
    enum write_status (*to_format)(FILE*, struct image);
};

struct io_files {
    const char* input_path;
    const char* output_path;
};

// processing steps (including clean memory)
bool process(struct processing_steps processing_steps,
             struct io_files io_files);

#endif
