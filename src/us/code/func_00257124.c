#include "common.h"

typedef struct {
    /* 0x00 */ s32 data[7];
} Record1C; /* size 0x1C */

/* Swaps two 0x1C-byte records. */
void func_00257124(Record1C *a, Record1C *b) {
    Record1C tmp = *a;

    *a = *b;
    *b = tmp;
}
