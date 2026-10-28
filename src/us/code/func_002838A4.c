#include "common.h"

typedef struct { f32 x, y, z; } Vec3f;

/* The player as this reads it. */
typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ s32 region;
    /* 0x014 */ u8 pad14[0x3C];
    /* 0x050 */ f32 rotY;
    /* 0x054 */ u8 pad54[0x480];
    /* 0x4D4 */ s32 unk4D4;
    /* 0x4D8 */ s32 unk4D8;
    /* 0x4DC */ u8 pad4DC[0xC];
    /* 0x4E8 */ s32 unk4E8;
} Player;

/* The state at *D_800C1BB0 (cleared by func_002837E0). */
typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ s32 started;
    /* 0x44 */ u8 pad44[0x8];
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s32 resetPlayer;
    /* 0x58 */ u8 pad58[0x4];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 pad60[0x58];
    /* 0xB8 */ s32 saveStart;
    /* 0xBC */ s32 startLevel;
    /* 0xC0 */ s32 startWarp;
    /* 0xC4 */ Vec3f startPos;
    /* 0xD0 */ f32 startRotY;
    /* 0xD4 */ s32 startRegion;
    /* 0xD8 */ s32 warpId;
} State;

extern State *D_800C1BB0;
extern Player *D_80119890;          /* current player */
extern u8 D_801198B0[];
extern u8 D_800F5A50[];
/* The world and fields of it (see func_002237C4's World). D_80110020 and
 * D_80110048 are arrays because the ROM accesses them as struct members: a
 * store through a pointer may alias them, so their loads stay after it. */
extern u8 D_800F7078[];             /* the world */
extern s32 D_80110020[];            /* world +0x18FA8: the current warp id */
extern s32 D_80110030;              /* world +0x18FB8: selectionFlags */
extern u8 D_80110034[];             /* world +0x18FBC: selectionSpawn */
extern s32 D_80110048[];            /* world +0x18FD0: selectionValues */

void func_002568CC(Player *player);
s32 func_002242B4(void *world, s32 region);
void func_002819E0(void *game);
void func_0026EDA8(void *arg0);
s32 func_0042C560(void *world, s32 *out, s32 id, void *spawn);
void func_0042AC0C(void *world, s32 id, s32 flags);
s32 func_0041266C(void);

/* Runs once (until the state is cleared): resets the current player when
 * asked, remembering where it starts the first time; warps to the pending
 * warp id unless it is the current one; then marks the state started. */
void func_002838A4(void) {
    Player *player;
    s32 warp;
    void *world;
    s32 out[3];

    warp = D_800C1BB0->warpId;
    if (D_800C1BB0->started != 0) {
        return;
    }
    if (D_800C1BB0->resetPlayer != 0) {
        player = D_80119890;
        if (player != NULL) {
            func_002568CC(player);
            if (D_800C1BB0->saveStart != 0 && D_800C1BB0->startLevel == -1
                && D_800C1BB0->startWarp == D_800C1BB0->startLevel) {
                D_800C1BB0->startLevel = D_80110048[0];
                D_800C1BB0->startWarp = D_80110020[0];
                D_800C1BB0->unk4C = 0;
                D_800C1BB0->startPos = player->pos;
                D_800C1BB0->startRotY = player->rotY;
                D_800C1BB0->startRegion = func_002242B4(D_800F7078, player->region);
            }
            player->unk4D4 = 0;
            player->unk4D8 = 0;
            player->unk4E8 = 0;
        }
        func_002819E0(D_801198B0);
        func_0026EDA8(D_800F5A50);
    }
    if (warp != -1) {
        world = D_800F7078;
        if (D_80110020[0] != warp) {
            out[0] = 0;
            out[1] = 0;
            out[2] = 0;
            D_80110048[0] = func_0042C560(world, out, warp, D_80110034);
            D_80110030 = D_80110048[0] != 0;
            func_0042AC0C(world, warp, D_80110030);
        }
    }
    D_800C1BB0->unk5C = func_0041266C();
    D_800C1BB0->unk50 = 1;
    D_800C1BB0->warpId = -1;
    D_800C1BB0->started = 1;
}
