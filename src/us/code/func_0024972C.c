#include "common.h"
#include "gfx.h"

typedef struct PlayerOptions {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ u8 tagMode;
} PlayerOptions;

typedef struct ActorDef {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s16 type;
} ActorDef;

typedef struct Player {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ u8 pad18[0x518 - 0x18];
    /* 0x518 */ PlayerOptions *options;
    /* 0x51C */ u8 pad51C[0xC04 - 0x51C];
    /* 0xC04 */ s32 unkC04;
} Player;

extern u8 D_8011AAF1;           /* multiplayer */
extern s32 D_8011ACE4;
extern s32 D_800ADB5C;
extern s32 D_800ADB30[];
extern u8 D_800F7078[];

void func_002241D8(void *world, Player *player, s32 model, s32 arg3);
void func_0021D39C(Player *player, s32 arg1);

/* Draws the player's model: picks it (single player, tag mode or the
   character), then sets up the render state before func_0021D39C. */
void func_0024972C(Player *player, s32 arg1) {
    s32 model;
    Gfx *g;

    if (D_8011AAF1 == 0) {
        model = 0x66;
    } else if (D_8011ACE4 != 0 && player->options->tagMode != 0) {
        model = D_800ADB5C;
    } else {
        model = D_800ADB30[player->def->type];
    }
    func_002241D8(D_800F7078, player, model, player->unkC04);
    g = D_800D8E24++;
    g->w0 = 0xE7000000;
    g->w1 = 0;
    g = D_800D8E24++;
    g->w0 = 0xE3000A01;
    g->w1 = 0x100000;
    g = D_800D8E24++;
    g->w0 = 0xD9FFFFFF;
    g->w1 = 0x10000;
    g = D_800D8E24++;
    g->w0 = 0xDB040004;
    g->w1 = 2;
    g = D_800D8E24++;
    g->w0 = 0xDB04000C;
    g->w1 = 2;
    g = D_800D8E24++;
    g->w0 = 0xDB040014;
    g->w1 = 0xFFFE;
    g = D_800D8E24++;
    g->w0 = 0xDB04001C;
    g->w1 = 0xFFFE;
    func_0021D39C(player, arg1);
}
