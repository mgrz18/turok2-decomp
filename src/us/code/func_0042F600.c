#include "common.h"

typedef struct Timer {
    /* 0x00 */ s32 value;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 reload;
    /* 0x0C */ s32 count;
    /* 0x10 */ s32 index;
    /* 0x14 */ void (*callback)(struct Timer *);
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
} Timer;

void func_0042F600(Timer *t, s32 unk4, s32 reload, s32 index, void (*callback)(Timer *), s32 unk18, s32 unk1C) {
    t->unk4 = unk4;
    t->reload = reload;
    t->index = index;
    t->count = 1;
    t->callback = callback;
    t->unk18 = unk18;
    t->unk1C = unk1C;
}
