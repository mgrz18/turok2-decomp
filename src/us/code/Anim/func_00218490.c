#include "common.h"

/* A cursor over fixed-size elements. */
typedef struct {
    /* 0x0 */ s32 ptr;
    /* 0x4 */ s32 stride;
} Cursor;

/* Advances the cursor arg1 elements. */
void func_00218490(Cursor *arg0, s32 arg1) {
    arg0->ptr = (s32) (arg0->ptr + (arg1 * arg0->stride));
}
