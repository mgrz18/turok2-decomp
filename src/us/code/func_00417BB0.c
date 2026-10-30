#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ f32 time;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 started;
} MatchState;

extern MatchState D_8011ACB0;

void func_00417BB0(f32 minutes, s32 a1, s32 started, s32 a3) {
    MatchState *m = &D_8011ACB0;

    m->started = started;
    m->unk24 = a3;
    m->unk18 = a1;
    m->unk1C = 0;
    m->unk2C = 0;
    m->unk30 = 0;
    m->unk28 = 0;
    m->unk20 = 0;
    m->time = minutes * 900.0f;
    if (m->time == 0.0f) {
        m->time = -1.0f;
    }
}
