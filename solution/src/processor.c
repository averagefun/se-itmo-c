#include "processor.h"

#include "image.h"
#include "io_utils.h"

bool process(struct processing_steps processing_steps,
             struct io_files io_files) {
    struct image input_img;
    enum read_status read_status =
        read_file(io_files.input_path, processing_steps.reader, &input_img);

    if (read_status != READ_OK) {
        fprintf(stderr, "%s", read_status_msgs_en[read_status]);
        return false;
    }

    struct image processed_img = processing_steps.transformator(input_img);
    image_destroy(&input_img);

    enum write_status write_status = write_file(
        io_files.output_path, processing_steps.writer, processed_img);
    image_destroy(&processed_img);

    if (write_status != WRITE_OK) {
        fprintf(stderr, "%s", write_status_msgs_en[write_status]);
        return false;
    }

    return true;
}
