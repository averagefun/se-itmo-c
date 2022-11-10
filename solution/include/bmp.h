#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include <stdio.h>

#include "image.h"
#include "io_utils.h"

// fill img from bmp
enum read_status from_bmp(FILE* in, struct image* img);

// fill file with img
enum write_status to_bmp(FILE* out, struct image img);

#endif
