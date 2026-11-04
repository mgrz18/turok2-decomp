#include "common.h"

/* Three words: a value and two counters this clears. */
typedef struct {
    /* 0x00 */ s32 value;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
} Triple;

extern Triple D_8011F290;

void func_0028D0B8(void) {
    D_8011F290.value = 0;
    D_8011F290.unk4 = 0;
    D_8011F290.unk8 = 0;
}
