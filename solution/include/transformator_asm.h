#ifndef TRANSFORMATOR_ASM_H
#define TRANSFORMATOR_ASM_H

#include "processor.h"

extern void image_apply_sepia_asm_impl(struct pixel* source_data,
                                       struct pixel* new_img_data,
                                       size_t quad_number);

/* create image copy, that applied sepia filter */
transformator image_apply_sepia_asm;

#endif
