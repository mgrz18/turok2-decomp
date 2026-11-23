#include "common.h"
#include "structs/demo.h"


void DemoStream_Init(DemoStream *t, sptr bits, s32 pos, s32 player, void (*decode)(DemoStream *), s32 unk18, s32 unk1C) {
    t->bits = (u8 *)bits;
    t->pos = pos;
    t->player = player;
    t->frames = 1;
    t->decode = decode;
    t->unk18 = unk18;
    t->unk1C = unk1C;
}
