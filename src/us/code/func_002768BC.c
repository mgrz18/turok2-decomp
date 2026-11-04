#include "common.h"

/* A sound effect descriptor (SndInfo in rw/junk_types.h), read through the accessors at 0x276820..0x2768B0. */
typedef struct SoundEffectInfo {
    /* 0x00 */ s16 group;
    /* 0x02 */ s16 sample;
    /* 0x04 */ s8 priority;
    /* 0x05 */ s8 chance;
    /* 0x06 */ u16 flags; /* low 14 bits flags, top 2 bits pan */
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s8 pitch;
    /* 0x0F */ s8 unkF;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 randomRange0;
    /* 0x14 */ s16 randomRange1;
} SoundEffectInfo;

s16 func_002768BC(SoundEffectInfo *arg0) {
    return arg0->unkC;
}
