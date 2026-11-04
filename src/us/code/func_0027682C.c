#include "common.h"

/* A sound definition record (the halfwords read by func_0027682C,
 * func_0027685C, func_00276868, func_0027688C). */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u16 flags6;          /* top two bits: a 0-3 value */
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ s16 unk10;
} SoundDef;

s16 func_0027682C(SoundDef *arg0) {
    return arg0->unk2;
}
