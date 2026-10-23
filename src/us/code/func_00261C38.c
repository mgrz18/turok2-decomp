#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ f32 unk34;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x40];
    /* 0x040 */ f32 unk40;
    /* 0x044 */ u8 pad44[0xFC - 0x44];
    /* 0x0FC */ void (*update)(Actor *, void *);
    /* 0x100 */ u8 pad100[0x10C - 0x100];
    /* 0x10C */ f32 unk10C;
    /* 0x110 */ u8 pad110[0x118 - 0x110];
    /* 0x118 */ f32 unk118;
    /* 0x11C */ f32 unk11C;
    /* 0x120 */ u8 pad120[0x128 - 0x120];
    /* 0x128 */ f32 unk128;
} AI;

f32 func_002119FC(f32 lo, f32 hi);
void func_00261B78(Actor *self, AI *ai);

void func_00261C38(Actor *self, AI *ai) {
    ai->update = (void (*)(Actor *, void *)) func_00261B78;
    ai->unk40 = func_002119FC(0.0f, 62.83186f);
    ai->unk10C = 0.0f;
    ai->unk118 = 0.017453294f;
    ai->unk11C = self->unk34;
    ai->unk128 = 0.0f;
}
