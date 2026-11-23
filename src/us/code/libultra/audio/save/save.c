/* Boundary fixed in versions/cuts.us.txt: splat starts func_002972D8 two words early; the function starts
 * at 0x2972E0 (D_002972E0). */
/* From decompals/ultralib (libultra 2.0I): src/audio/save.c, alSavePull. */
#include "common.h"
#include "globals/strings.h"
#include "structs/libaudio.h"
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define A_SETBUFF 8
#define A_INTERLEAVE 13
#define A_SAVEBUFF 6
#define AL_MAIN_L_OUT 0x440
#define AL_MAIN_R_OUT 0x580
#define aSetBuffer(pkt, f, i, o, c) { \
    Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = (_SHIFTL(A_SETBUFF, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(i, 0, 16)); \
    _a->words.w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16); \
}
#define aInterleave(pkt, l, r) { \
    Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = _SHIFTL(A_INTERLEAVE, 24, 8); \
    _a->words.w1 = _SHIFTL(l, 16, 16) | _SHIFTL(r, 0, 16); \
}
#define aSaveBuffer(pkt, s) { \
    Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = _SHIFTL(A_SAVEBUFF, 24, 8); \
    _a->words.w1 = (unsigned int)(s); \
}

void __assert(char *, char *, s32); /* __assert */
#define assert(e, line) ((e) ? (void)0 : __assert(sSaveAssertExpr, sSaveAssertFile, line))

Acmd *alSavePull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p) {
    Acmd *ptr = p;
    ALSave *f = (ALSave *)filter;
    ALFilter *source = f->filter.source;

    assert(f->filter.source, 34);

    ptr = (*source->handler)(source, outp, outCount, sampleOffset, ptr);

    aSetBuffer(ptr++, 0, 0, 0, outCount << 1);
    aInterleave(ptr++, AL_MAIN_L_OUT, AL_MAIN_R_OUT);
    aSetBuffer(ptr++, 0, 0, 0, outCount << 2);
    aSaveBuffer(ptr++, f->dramout);
    return ptr;
}

typedef struct {
    /* 0x00 */ s32 param1;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ s32 param6;
} Unk002973E0;

/* Sets parameter arg1 (1 or 6) to arg2. */
#ifndef TARGET_PORT /* the port defines alSaveParam in port/audio/t2audio_lib.c */
s32 alSaveParam(Unk002973E0 *arg0, s32 arg1, sptr arg2) {
    switch (arg1) {
        case 1:
            arg0->param1 = arg2;
            break;
        case 6:
            arg0->param6 = arg2;
            break;
    }
    return 0;
}
#endif /* !TARGET_PORT */
