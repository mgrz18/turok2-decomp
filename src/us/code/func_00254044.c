#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ u8 unk3E;
} Settings;

/* A spawn point record (from func_00225EBC). */
typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ f32 pos[3];
} Spawn;

/* The player: what this sets up. */
typedef struct {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ Spawn *spawn;
    /* 0x018 */ u8 pad18[0x34 - 0x18];
    /* 0x034 */ f32 pos[3];
    /* 0x040 */ u8 pad40[0x518 - 0x40];
    /* 0x518 */ Settings *settings;
    /* 0x51C */ u8 pad51C[4];
    /* 0x520 */ s32 team;
} Player;

Spawn *func_00225EBC(s32, s32, s32);
extern s32 D_800F7078;

/* Picks a spawn point for the player and copies its position. */
void func_00254044(Player *arg0) {
    s32 *var_s1;
    s32 var_a2;
    Spawn *var_v0;
    Player *var_s0;

    var_s0 = arg0;
    var_a2 = var_s0->team;
    if (var_s0->settings->unk3E == 1) {
        var_a2 = 0xB;
    }
    var_s1 = &D_800F7078;
    var_s1 = &D_800F7078;
    var_v0 = func_00225EBC((s32) &D_800F7078, 0xB, var_a2);
    if (var_v0 == 0) {
        var_v0 = func_00225EBC((s32) &D_800F7078, 0xB, -1);
        if (var_v0 == 0) {
            var_v0 = func_00225EBC((s32) &D_800F7078, -1, -1);
        }
    }
    var_s0->spawn = var_v0;
    var_s0->pos[0] = (f32) var_v0->pos[0];
    var_s0->pos[1] = (f32) var_v0->pos[1];
    var_s0->pos[2] = (f32) var_v0->pos[2];
}
