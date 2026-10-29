/* Boundary fixed in versions/cuts.us.txt: splat starts func_0029635C one word early (a stray word); the
 * function starts at 0x296360 (D_00296360). */
/* From decompals/ultralib (libultra 2.0I): src/audio/mainbus.c, alMainBusPull. */

typedef unsigned int u32;
typedef short s16;
typedef long s32;

typedef union {
    struct { u32 w0; u32 w1; } words;
    long long force_align;
} Acmd;

typedef Acmd *(*ALCmdHandler)(void *, s16 *, s32, s32, Acmd *);

typedef struct ALFilter_s {
    /* 0x00 */ struct ALFilter_s *source;
    /* 0x04 */ ALCmdHandler handler;
    /* 0x08 */ void *setParam;
    /* 0x0C */ s16 inp;
    /* 0x0E */ s16 outp;
    /* 0x10 */ s32 type;
} ALFilter;

typedef struct {
    /* 0x00 */ ALFilter filter;
    /* 0x14 */ s32 sourceCount;
    /* 0x18 */ s32 maxSources;
    /* 0x1C */ ALFilter **sources;
} ALMainBus;

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
