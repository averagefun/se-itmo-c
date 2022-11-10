#ifndef IO_UTILS_H
#define IO_UTILS_H

#include <stdio.h>

#include "image.h"

extern char* read_status_msgs_en[];
extern char* write_status_msgs_en[];

// read from format (bmp, etc) -> return status
enum read_status {
    READ_OK = 0,
    READ_INVALID_FILE,
    READ_INVALID_SIGNATURE,
    READ_INVALID_BITS,
    READ_INVALID_HEADER
};

// write to format (bmp, etc) -> return status
enum write_status {
    WRITE_OK = 0,
    WRITE_INVALID_FILE,
    WRITE_HEADER_FAILED,
    WRITE_DATA_FAILED
};

typedef enum read_status(from_format_reader)(FILE*, struct image*);

// read from file 'input_path' using from_format function
enum read_status read_file(const char* input_path, from_format_reader* reader,
                           struct image* input_img);

typedef enum write_status(to_format_writer)(FILE*, struct image);

// write to file 'output_path' using to_format function
enum write_status write_file(const char* output_path, to_format_writer* writer,
                             struct image output_img);

#endif
