#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x46];
    /* 0x46 */ s16 total;
} Info;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Info *info;
    /* 0x08 */ u8 pad8[0x10 - 0x8];
    /* 0x10 */ s32 used;
} Arg0;

/* What is left: info->total - used. */
s32 func_00441BFC(Arg0 *arg0) {
    return arg0->info->total - arg0->used;
}
