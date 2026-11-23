#include "common.h"

/* The FFT tables mdctFft needs. */
typedef struct {
    /* 0x0 */ s32 cosTable;
    /* 0x4 */ s32 sinTable;
    /* 0x8 */ s32 bitrev;
    /* 0xC */ s32 n;
} FFTTables;

s32 mdctFft(sptr, s32, s32, sptr, sptr, sptr);

void mdctFftCtx(s32 arg0, FFTTables *arg1, s32 arg2) {
    mdctFft(arg0, arg1->n, arg2, arg1->cosTable, arg1->sinTable, arg1->bitrev);
}
