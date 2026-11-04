#include "common.h"

/* The FFT tables func_002A09E8 needs. */
typedef struct {
    /* 0x0 */ s32 cosTable;
    /* 0x4 */ s32 sinTable;
    /* 0x8 */ s32 bitrev;
    /* 0xC */ s32 n;
} FFTTables;

s32 func_002A09E8(s32, s32, s32, s32, s32, s32);

void func_002A10CC(s32 arg0, FFTTables *arg1, s32 arg2) {
    func_002A09E8(arg0, arg1->n, arg2, arg1->cosTable, arg1->sinTable, arg1->bitrev);
}
