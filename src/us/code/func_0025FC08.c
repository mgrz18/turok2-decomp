#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x51];
    /* 0x51 */ s8 unk51;
} DefData;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u8 *def;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad8[0x64 - 0x8];
    /* 0x64 */ f32 timer;
} AI;

extern f32 D_800B6D28; /* frame delta */

void func_004089C8(void);
void func_00243414(Actor *self, AI *ai, s32 mode);

void func_0025FC08(Actor *self, AI *ai) {
    DefData *data = (DefData *) (self->def + 0x14);

    if (ai->timer > 75.0f) {
        ai->timer = 75.0f;
    }
    ai->timer -= D_800B6D28;
    if (ai->timer <= 0.0f) {
        ai->unk4 = 0;
        func_004089C8();
        if (data->unk51 != -1) {
            func_00243414(self, ai, 0x38);
        } else {
            func_00243414(self, ai, 0x34);
        }
    }
}
