/* Boundary fixed in versions/cuts.us.txt: splat starts func_002972D8 two words early; the function starts
 * at 0x2972E0 (D_002972E0). */
/* From decompals/ultralib (libultra 2.0I): src/audio/save.c, alSavePull. */
#include "common.h"
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

#define assert(e, line) ((e) ? (void)0 : __assert("EX", "audio/save.c", line))

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
