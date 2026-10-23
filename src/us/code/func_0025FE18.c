#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0xA08];
    /* 0xA08 */ s16 mode;
} Player;

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ u32 flagsHi : 14;
    /* 0x0D4 */ u32 flag20000 : 1;
    /* 0x0D4 */ u32 flag10000 : 1;
    /* 0x0D4 */ u32 flagsLo : 16;
    /* 0x0D8 */ u8 padD8[0x1A8 - 0xD8];
    /* 0x1A8 */ Player *owner;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ s8 mode;
    /* 0x035 */ u8 pad35[0xC7 - 0x35];
    /* 0x0C7 */ s8 unkC7;
    /* 0x0C8 */ u8 padC8[0xF4 - 0xC8];
    /* 0x0F4 */ s32 unkF4;
    /* 0x0F8 */ u8 padF8[0x114 - 0xF8];
    /* 0x114 */ Actor *target;
} AI;

void func_00243414(Actor *self, AI *ai, s32 mode);

void func_0025FE18(Actor *self, AI *ai) {
    Player *player = ai->target->owner;

    self->flag20000 = 0;
    self->flag10000 = 0;
    ai->unkF4 = 0;
    if (ai->mode != 0x2E || ai->unkC7 != 0) {
        if (player->mode == 0x21) {
            func_00243414(self, ai, 0x2C);
        } else if (player->mode == 0x22) {
            func_00243414(self, ai, 0x2D);
        }
    }
}
