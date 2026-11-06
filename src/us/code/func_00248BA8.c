#include "common.h"
#include "structs/game.h"
#include "structs/vector.h"
#include "structs/world.h"

#define FALSE 0
#define TRUE 1

/* Puts the player at a warp point: unlocks/redirects level hub warps, marks
 * visited hubs, then picks the warp record (the one farthest from the other
 * players in multiplayer) and moves the player there. */

typedef struct WarpPlayer WarpPlayer;
struct WarpPlayer {
    /* 0x0000 */ u8 pad0[4];
    /* 0x0004 */ CVector3 pos;
    /* 0x0010 */ s32 room;
    /* 0x0014 */ u8 pad14[0x50 - 0x14];
    /* 0x0050 */ f32 angle;
    /* 0x0054 */ u8 pad54[0x28C - 0x54];
    /* 0x028C */ CVector3 spawnPos;
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

void CPlayer__StopLoopingSounds(void);
s32 Controller_SetRumbleOption(s32, s32);
s32 Mission_LevelObjectivesDone(s32);
s32 CScene__FindWarpDests(void *, s32, s32, CWarpPoint *, s32);
f32 CVector3__Distance(CVector3 *, CVector3 *);
s32 CScene__GetRegion(void *, s32);
void CPlayer__LevelStart(WarpPlayer *);
void CScene__RequestWarp(void *, s32, s32);

/* Sum of the distances from a warp record to every active player. */
static __inline__ f32 sumDistance(CWarpPoint *r) {
    WarpPlayer *other;
    f32 dist;

    dist = 0.0f;
    for (other = (WarpPlayer *)D_80119870.m_PlayerList.m_ActiveList.m_pHead; other != NULL; other = other->next) {
        if (other->active) {
            dist += CVector3__Distance(&r->m_vPos, &other->pos);
        }
    }
    return dist;
}

void CPlayer__Warp(WarpPlayer *pl, s32 warp, s32 arg2, s32 respawn) {
    CWarpPoint rec;
    CWarpPoint recs[32];
    s32 skip;
    s32 count;
    s32 best;
    s32 i;
    f32 bestDist;
    f32 dist;

    skip = FALSE;
    CPlayer__StopLoopingSounds();
    Controller_SetRumbleOption(pl->controller, 0);

    if (D_8011001C && respawn) {
        skip = TRUE;
    }
    if (!skip) {
        switch (warp) {
        case 1000:
            if (!Mission_LevelObjectivesDone(1)) {
                pl->unkC00 = 1;
            }
            break;
        case 2000:
            if (!Mission_LevelObjectivesDone(2)) {
                pl->unkC00 = 1;
            }
            break;
        case 3000:
            if (!Mission_LevelObjectivesDone(3)) {
                pl->unkC00 = 1;
            }
            break;
        case 4000:
            if (!Mission_LevelObjectivesDone(4)) {
                pl->unkC00 = 1;
            }
            break;
        case 5000:
            if (!Mission_LevelObjectivesDone(5)) {
                pl->unkC00 = 1;
            }
            break;
        case 6000:
            if (!Mission_LevelObjectivesDone(6)) {
                pl->unkC00 = 1;
            }
            break;
        case 7100:
            if (Mission_LevelObjectivesDone(1)) {
                if (pl->keys[0]) {
                    warp = 1999;
                }
            } else {
                pl->unkC00 = 1;
                warp = 1000;
            }
            break;
        case 7200:
            if (Mission_LevelObjectivesDone(2)) {
                if (pl->keys[1]) {
                    warp = 2999;
                }
            } else {
                pl->unkC00 = 1;
                warp = 2000;
            }
            break;
        case 7300:
            if (Mission_LevelObjectivesDone(3)) {
                if (pl->keys[2]) {
                    warp = 3999;
                }
            } else {
                pl->unkC00 = 1;
                warp = 3000;
            }
            break;
        case 7400:
            if (Mission_LevelObjectivesDone(4)) {
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
            if (Mission_LevelObjectivesDone(5)) {
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
            if (Mission_LevelObjectivesDone(6)) {
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

    if (D_80119870.m_Settings.multiplayer) {
        count = CScene__FindWarpDests(D_800F7078, -1, warp, recs, 32);
        best = -1;
        bestDist = 0.0f;
        for (i = 0; i < count; i++) {
            dist = sumDistance(&recs[i]);
            if (bestDist <= dist) {
                bestDist = dist;
                best = i;
            }
        }
        if (best != -1) {
            pl->spawnPos = recs[best].m_vPos;
            pl->pos = pl->spawnPos;
            pl->angle = pl->spawnAngle = recs[best].m_RotY;
            pl->room = pl->spawnRoom = CScene__GetRegion(D_800F7078, recs[best].m_nID);
            CPlayer__LevelStart(pl);
            return;
        }
    } else {
        do { /* a block of its own: it decides which of the two addresses gets $s0 */
            if (CScene__FindWarpDests(D_800F7078, -1, warp, &rec, 1) && rec.m_nLevel == D_80110020 &&
                (D_80119870.m_Settings.multiplayer || D_8011001C)) {
                pl->spawnPos = rec.m_vPos;
                pl->pos = pl->spawnPos;
                pl->angle = pl->spawnAngle = rec.m_RotY;
                pl->room = pl->spawnRoom = CScene__GetRegion(D_800F7078, rec.m_nID);
                CPlayer__LevelStart(pl);
                return;
            }
        } while (0);
    }
    CScene__RequestWarp(D_800F7078, warp, arg2);
}
