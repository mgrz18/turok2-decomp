/* The second VM module's copy of src/us/vm1/Demo/func_0042F600.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/demo.h"


void func_00468520(DemoStream *t, sptr bits, s32 pos, s32 player, void (*decode)(DemoStream *), s32 unk18, s32 unk1C) {
    t->bits = (u8 *)bits;
    t->pos = pos;
    t->player = player;
    t->frames = 1;
    t->decode = decode;
    t->unk18 = unk18;
    t->unk1C = unk1C;
}
