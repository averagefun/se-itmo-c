#ifndef TRANSFORMATOR_ASM_H
#define TRANSFORMATOR_ASM_H

#include "processor.h"

extern int image_apply_sepia_asm_impl(struct image* img);

/* create image copy, that applied sepia filter */
transformator image_apply_sepia_asm;

#endif
