#include "common.h"

/* A record of eight words this clears. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC[3];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
} Record8;

void GfxPool_Clear(Record8 *arg0) {
    s32 var_v1;
    s32 *var_v0;

    var_v1 = 2;
    var_v0 = &arg0->unk8;
    arg0->unk0 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unk18 = 0;
    arg0->unk1C = 0;
    do {
        var_v0[3] = 0;
        var_v1 -= 1;
        var_v0 -= 1;
    } while (var_v1 >= 0);
}
