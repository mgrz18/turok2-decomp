#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0xA78];
    /* 0xA78 */ s32 unkA78;
    /* 0xA7C */ s32 unkA7C;
    /* 0xA80 */ u8 padA80[0xA98 - 0xA80];
    /* 0xA98 */ f32 unkA98;
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
} Arg1;

void func_00255C5C(Player *arg0, Arg1 *arg1) {
    arg0->unkA78 = 0;
    arg0->unkA7C = 0;
    arg0->unkA98 = 0.024348905f;
    arg1->unk18 = 0;
    arg1->unk1C = 0;
    arg1->unk20 = 0;
}
