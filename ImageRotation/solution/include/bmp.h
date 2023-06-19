#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include <stdio.h>

#include "image.h"
#include "io_utils.h"

// fill img from bmp
from_format_reader from_bmp;

// fill file with img
to_format_writer to_bmp;

#endif
