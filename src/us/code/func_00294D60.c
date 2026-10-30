/* alAuxBusPull */
#include "common.h"

typedef struct {
    u32 w0;
    u32 w1;
} AcmdW;

typedef struct Filter {
    /* 0x00 */ struct Filter *source;
    /* 0x04 */ AcmdW *(*handler)(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, AcmdW *p);
} Filter;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 sourceCount;
    /* 0x18 */ s32 maxSources;
    /* 0x1C */ Filter **sources;
} AuxBus;

#define aClearBuffer(pkt, d, c) {          \
        AcmdW *_a = (AcmdW *)pkt;          \
        _a->w0 = 0x02000000 | (d);         \
        _a->w1 = (u32)(c);                 \
    }

AcmdW *func_00294D60(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, AcmdW *p) {
    AcmdW *ptr = p;
    AuxBus *m = (AuxBus *)filter;
    Filter **sources = m->sources;
    s32 i;

    aClearBuffer(ptr++, 0x6C0, outCount << 1);
    aClearBuffer(ptr++, 0x800, outCount << 1);

    for (i = 0; i < m->sourceCount; i++) {
        ptr = (*sources[i]->handler)(sources[i], outp, outCount, sampleOffset, ptr);
    }
    return ptr;
}
