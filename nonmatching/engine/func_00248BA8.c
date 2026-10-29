/* NON-MATCHING: 68 of 348 words differ: register allocation only, from three
 * address values the ROM keeps differently: &D_8011AAF1 (the multiplayer flag
 * in GameState) lands in $s1 in the ROM but $s0 here; the ROM keeps
 * &D_800F7078 in $s0 across the single-player lookup call; and the ROM forms
 * &recs[i] in the scoring loop as (i * 0x14 + sp+0x18) + 0x18 while this C
 * reuses the call argument sp+0x30 (an inline helper taking the record
 * pointer gets that form, 61 words). Logic and layout are complete. */
#include "common.h"

#define FALSE 0
#define TRUE 1

/* Puts the player at a warp point: unlocks/redirects level hub warps, marks
 * visited hubs, then picks the warp record (the one farthest from the other
 * players in multiplayer) and moves the player there. */

typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

typedef struct WarpPoint {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 angle;
    /* 0x10 */ s16 level;
    /* 0x12 */ s16 room;
} WarpPoint; /* size 0x14 */

typedef struct WarpPlayer WarpPlayer;
struct WarpPlayer {
    /* 0x0000 */ u8 pad0[4];
    /* 0x0004 */ Vec3f pos;
    /* 0x0010 */ s32 room;
    /* 0x0014 */ u8 pad14[0x50 - 0x14];
    /* 0x0050 */ f32 angle;
    /* 0x0054 */ u8 pad54[0x28C - 0x54];
    /* 0x028C */ Vec3f spawnPos;
    /* 0x0298 */ s32 spawnRoom;
    /* 0x029C */ u8 pad29C[0x2D8 - 0x29C];
    /* 0x02D8 */ f32 spawnAngle;
    /* 0x02DC */ u8 pad2DC[0x51C - 0x2DC];
    /* 0x051C */ s32 active;
    /* 0x0520 */ u8 pad520[0x9D0 - 0x520];
    /* 0x09D0 */ s32 keys[8];
    /* 0x09F0 */ u8 pad9F0[0x9F8 - 0x9F0];
    /* 0x09F8 */ u32 hubsVisited;
    /* 0x09FC */ u8 pad9FC[0xA50 - 0x9FC];
    /* 0x0A50 */ s32 controller;
    /* 0x0A54 */ u8 padA54[0xC00 - 0xA54];
    /* 0x0C00 */ s32 unkC00;
    /* 0x0C04 */ u8 padC04[0x1320 - 0xC04];
    /* 0x1320 */ WarpPlayer *next;
};

extern s32 D_8011001C;
extern s32 D_80110020;
extern u8 D_800F7078[];
/* Game state from the player list on; the multiplayer flag is at +0x1261. */
typedef struct GameState {
    /* 0x0000 */ WarpPlayer *players;
    /* 0x0004 */ u8 pad4[0x1261 - 4];
    /* 0x1261 */ u8 multiplayer;
} GameState;

extern GameState D_80119890;

void func_0042EB04(void);
s32 func_00285BD0(s32, s32);
s32 func_00424A60(s32);
s32 func_0042C194(void *, s32, s32, WarpPoint *, s32);
f32 func_0020F6BC(Vec3f *, Vec3f *);
s32 func_002242F8(void *, s32);
void func_0042DE74(WarpPlayer *);
void func_0042C700(void *, s32, s32);

void func_00248BA8(WarpPlayer *pl, s32 warp, s32 arg2, s32 respawn) {
    WarpPoint rec;
    WarpPoint recs[32];
    WarpPoint *r;
    WarpPlayer *other;
    s32 skip;
    s32 count;
    s32 best;
    s32 i;
    s32 room;
    f32 bestDist;
    f32 dist;

    skip = FALSE;
    func_0042EB04();
    func_00285BD0(pl->controller, 0);

    if (D_8011001C && respawn) {
        skip = TRUE;
    }
    if (!skip) {
        switch (warp) {
        case 1000:
            if (!func_00424A60(1)) {
                pl->unkC00 = 1;
            }
            break;
        case 2000:
            if (!func_00424A60(2)) {
                pl->unkC00 = 1;
            }
            break;
        case 3000:
            if (!func_00424A60(3)) {
                pl->unkC00 = 1;
            }
            break;
        case 4000:
            if (!func_00424A60(4)) {
                pl->unkC00 = 1;
            }
            break;
        case 5000:
            if (!func_00424A60(5)) {
                pl->unkC00 = 1;
            }
            break;
        case 6000:
            if (!func_00424A60(6)) {
                pl->unkC00 = 1;
            }
            break;
        case 7100:
            if (func_00424A60(1)) {
                if (pl->keys[0]) {
                    warp = 1999;
                }
            } else {
                pl->unkC00 = 1;
                warp = 1000;
            }
            break;
        case 7200:
            if (func_00424A60(2)) {
                if (pl->keys[1]) {
                    warp = 2999;
                }
            } else {
                pl->unkC00 = 1;
                warp = 2000;
            }
            break;
        case 7300:
            if (func_00424A60(3)) {
                if (pl->keys[2]) {
                    warp = 3999;
                }
            } else {
                pl->unkC00 = 1;
                warp = 3000;
            }
            break;
        case 7400:
            if (func_00424A60(4)) {
                if (pl->keys[3]) {
                    if (pl->keys[5]) {
                        warp = 4999;
                    } else {
                        warp = 7401;
                    }
                }
            } else {
                pl->unkC00 = 1;
                warp = 4000;
            }
            break;
        case 7500:
            if (func_00424A60(5)) {
                if (pl->keys[4]) {
                    if (pl->keys[6]) {
                        warp = 5999;
                    } else {
                        warp = 7501;
                    }
                }
            } else {
                pl->unkC00 = 1;
                warp = 5000;
            }
            break;
        case 7601:
            if (func_00424A60(6)) {
                if (pl->keys[7]) {
                    warp = 6999;
                }
            } else {
                pl->unkC00 = 1;
                warp = 6000;
            }
            break;
        }
    }
    if (respawn) {
        pl->unkC00 = 0;
    }

    switch (warp) {
    case 1000:
        pl->hubsVisited |= 1;
        break;
    case 2000:
        pl->hubsVisited |= 2;
        break;
    case 3000:
        pl->hubsVisited |= 4;
        break;
    case 4000:
        pl->hubsVisited |= 8;
        break;
    case 5000:
        pl->hubsVisited |= 0x10;
        break;
    case 6000:
        pl->hubsVisited |= 0x20;
        break;
    }

    if (D_80119890.multiplayer) {
        count = func_0042C194(D_800F7078, -1, warp, recs, 32);
        best = -1;
        bestDist = 0.0f;
        for (i = 0; i < count; i++) {
            dist = 0.0f;
            for (other = D_80119890.players; other != NULL; other = other->next) {
                if (other->active) {
                    dist += func_0020F6BC(&recs[i].pos, &other->pos);
                }
            }
            if (bestDist <= dist) {
                bestDist = dist;
                best = i;
            }
        }
        if (best != -1) {
            r = &recs[best];
            pl->spawnPos = r->pos;
            pl->pos = pl->spawnPos;
            pl->angle = pl->spawnAngle = r->angle;
            room = r->room;
            goto found;
        }
    } else if (func_0042C194(D_800F7078, -1, warp, &rec, 1) && rec.level == D_80110020 &&
               (D_80119890.multiplayer || D_8011001C)) {
        pl->spawnPos = rec.pos;
        pl->pos = pl->spawnPos;
        pl->angle = pl->spawnAngle = rec.angle;
        room = rec.room;
    found:
        pl->room = pl->spawnRoom = func_002242F8(D_800F7078, room);
        func_0042DE74(pl);
        return;
    }
    func_0042C700(D_800F7078, warp, arg2);
}
