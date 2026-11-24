#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x46];
    /* 0x46 */ s16 unk46;
} Ref00408BFC;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Ref00408BFC *ref;
    /* 0x08 */ u8 pad8[0x10 - 0x8];
    /* 0x10 */ s32 unk10;
} Unk00408BFC;

s32 TotemDefense_KillsLeft(Unk00408BFC *arg0) {
    return arg0->ref->unk46 - arg0->unk10;
}
