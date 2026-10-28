#include "common.h"

typedef struct { f32 x, y, z; } Vec3f;

/* The player as this reads it. */
typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ s32 region;
    /* 0x014 */ u8 pad14[0x3C];
    /* 0x050 */ f32 rotY;
} Player;

/* The state at *D_800C1BB0 (cleared by func_002837E0, see func_002838A4). */
typedef struct {
    /* 0x00 */ u8 pad0[0x4C];
    /* 0x4C */ s32 restored;
    /* 0x50 */ u8 pad50[0x68];
    /* 0xB8 */ s32 saveStart;
    /* 0xBC */ s32 startLevel;
    /* 0xC0 */ s32 startWarp;
    /* 0xC4 */ Vec3f startPos;
    /* 0xD0 */ f32 startRotY;
    /* 0xD4 */ s32 startRegion;
} State;

extern State *D_800C1BB0;
extern Player *D_80119890;          /* current player */
extern s32 D_8011AC88;
extern u8 D_800F7078[];             /* the world */
extern s32 D_80110020;              /* world +0x18FA8: the current warp id */
extern s32 D_80110024;
extern s32 D_80110028;
extern s32 D_8011002C;
extern s32 D_80110048;              /* world +0x18FD0: selectionValues */
extern s32 D_8011004C;

void func_0042AC0C(void *world, s32 id, s32 flags);
s32 func_002242F8(void *world, s32 region);
void func_0024CF58(Player *player, Player *other);

/* Puts the current player back where func_002838A4 saw it start (once,
 * when D_8011AC88 is 13 and a start was saved), warping there first if
 * needed; any other D_8011AC88 drops the saved start. */
void func_00283A4C(void) {
    void *world;
    Player *player;
    Vec3f pos;
    f32 rotY;
    s32 region;
    s32 level;
    s32 warp;

    world = D_800F7078;
    level = D_800C1BB0->startLevel;
    warp = D_800C1BB0->startWarp;
    if (D_8011AC88 != 13) {
        D_800C1BB0->restored = 1;
        D_800C1BB0->saveStart = 0;
        D_800C1BB0->startLevel = -1;
        D_800C1BB0->startWarp = -1;
        return;
    }
    if (D_800C1BB0->restored == 0 && level != -1 && warp != -1 && D_800C1BB0->saveStart != 0) {
        D_800C1BB0->restored = 1;
        pos = D_800C1BB0->startPos;
        rotY = D_800C1BB0->startRotY;
        region = D_800C1BB0->startRegion;
        if (D_80110020 != warp) {
            func_0042AC0C(world, warp, 1);
        }
        player = D_80119890;
        D_80110024 = 0;
        D_80110028 = 0;
        D_8011002C = 0;
        D_80110048 = level;
        D_8011004C = level;
        if (player != NULL) {
            player->pos = pos;
            player->rotY = rotY;
            player->region = func_002242F8(world, region);
            func_0024CF58(player, player);
        }
        D_800C1BB0->saveStart = 0;
        D_800C1BB0->startLevel = -1;
        D_800C1BB0->startWarp = -1;
    }
}
