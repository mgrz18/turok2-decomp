/* Boundary fixed in versions/cuts.us.txt: splat starts func_0029635C one word early (a stray word); the
 * function starts at 0x296360 (D_00296360). */
/* From decompals/ultralib (libultra 2.0I): src/audio/mainbus.c, alMainBusPull. */

#include "common.h"
#include "structs/sound.h"


#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))

#define A_CLEARBUFF 2
#define A_SETBUFF 8
#define A_MIXER 12
#define AL_MAIN_L_OUT 1088
#define AL_MAIN_R_OUT 1408
#define AL_AUX_L_OUT 1728
#define AL_AUX_R_OUT 2048

#define aClearBuffer(pkt, d, c) { \
    Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = _SHIFTL(A_CLEARBUFF, 24, 8) | _SHIFTL(d, 0, 24); \
    _a->words.w1 = (unsigned int)(c); \
}
#define aSetBuffer(pkt, f, i, o, c) { \
    Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = (_SHIFTL(A_SETBUFF, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(i, 0, 16)); \
    _a->words.w1 = _SHIFTL(o, 16, 16) | _SHIFTL(c, 0, 16); \
}
#define aMix(pkt, f, g, i, o) { \
    Acmd *_a = (Acmd *)pkt; \
    _a->words.w0 = (_SHIFTL(A_MIXER, 24, 8) | _SHIFTL(f, 16, 8) | _SHIFTL(g, 0, 16)); \
    _a->words.w1 = _SHIFTL(i, 16, 16) | _SHIFTL(o, 0, 16); \
}

Acmd *func_00296360(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p) {
    Acmd *ptr = p;
    ALMainBus *m = (ALMainBus *)filter;
    ALFilter **sources = m->sources;
    s32 i;

    /* clear the output buffers here */
    aClearBuffer(ptr++, AL_MAIN_L_OUT, outCount << 1);
    aClearBuffer(ptr++, AL_MAIN_R_OUT, outCount << 1);

    for (i = 0; i < m->sourceCount; i++) {
        ptr = (sources[i]->handler)(sources[i], outp, outCount, sampleOffset, ptr);
        aSetBuffer(ptr++, 0, 0, 0, outCount << 1);
        aMix(ptr++, 0, 0x7fff, AL_AUX_L_OUT, AL_MAIN_L_OUT);
        aMix(ptr++, 0, 0x7fff, AL_AUX_R_OUT, AL_MAIN_R_OUT);
    }

    return ptr;
}
