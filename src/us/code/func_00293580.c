#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[4];
} Track00293580; /* size 0x30 */

typedef struct {
    /* 0x00 */ u8 pad0[0x3C];
    /* 0x3C */ s32 current;
    /* 0x40 */ Track00293580 *tracks;
} Unk00293580;

s32 alSndpGetState(Unk00293580 *arg0) {
    return arg0->tracks[arg0->current].unk28;
}
