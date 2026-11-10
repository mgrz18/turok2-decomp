#include "common.h"
#include "structs/game.h"
#include "structs/vector.h"
#include "structs/world.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/game.h"

#define FALSE 0
#define TRUE 1

/* Puts the player at a warp point: unlocks/redirects level hub warps, marks
 * visited hubs, then picks the warp record (the one farthest from the other
 * players in multiplayer) and moves the player there. */

typedef struct WarpPlayer WarpPlayer;
#ifdef TARGET_PORT
struct WarpPlayer {
/* the view at the host offsets of CPlayer (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_WarpPlayer(m) __builtin_offsetof(CPlayer, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_WarpPlayer(m_CharacterActor.header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_room[T2_HOFF_WarpPlayer(m_CharacterActor.header.m_pCurrentRegion)]; sptr room; }; /* N64 0x10 */
        struct { u8 _hp_angle[T2_HOFF_WarpPlayer(m_CharacterActor.m_RotY)]; f32 angle; }; /* N64 0x50 */
        struct { u8 _hp_spawnPos[T2_HOFF_WarpPlayer(m_WeaponActor.header.m_vPos)]; CVector3 spawnPos; }; /* N64 0x28C */
        struct { u8 _hp_spawnRoom[T2_HOFF_WarpPlayer(m_WeaponActor.header.m_pCurrentRegion)]; sptr spawnRoom; }; /* N64 0x298 */
        struct { u8 _hp_spawnAngle[T2_HOFF_WarpPlayer(m_WeaponActor.m_RotY)]; f32 spawnAngle; }; /* N64 0x2D8 */
        struct { u8 _hp_active[T2_HOFF_WarpPlayer(m_pCamera)]; sptr active; }; /* N64 0x51C */
        struct { u8 _hp_keys[T2_HOFF_WarpPlayer(m_Data) + 1200]; s32 keys[8]; }; /* N64 0x9D0 */
        struct { u8 _hp_hubsVisited[T2_HOFF_WarpPlayer(m_Data.m_dwLevelFirstTimeFlags)]; u32 hubsVisited; }; /* N64 0x9F8 */
        struct { u8 _hp_controller[T2_HOFF_WarpPlayer(m_ContState.m_pContState)]; struct ControllerProfile *controller; }; /* N64 0xA50 */
        struct { u8 _hp_unkC00[T2_HOFF_WarpPlayer(m_bLevelNotCompleteExit)]; s32 unkC00; }; /* N64 0xC00 */
        struct { u8 _hp_next[T2_HOFF_WarpPlayer(m_pNext)]; WarpPlayer *next; }; /* N64 0x1320 */
    };
};
#else
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
#endif


#ifdef TARGET_PORT /* unprototyped: the host passes the pass-through arguments */
void CPlayer__StopLoopingSounds();
#else
void CPlayer__StopLoopingSounds(void);
#endif
s32 Mission_LevelObjectivesDone(s32);
f32 CVector3__Distance(CVector3 *, CVector3 *);
void CPlayer__LevelStart(WarpPlayer *);

/* Sum of the distances from a warp record to every active player. */
static __inline__ f32 sumDistance(CWarpPoint *r) {
    WarpPlayer *other;
    f32 dist;

    dist = 0.0f;
    for (other = (WarpPlayer *)gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; other != NULL; other = other->next) {
        if (other->active) {
            dist += CVector3__Distance(&r->m_vPos, &other->pos);
        }
    }
    return dist;
}

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__StopLoopingSounds receives CPlayer__Warp's own argument
 * registers (pass-through); the host passes them. */
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
    CPlayer__StopLoopingSounds(pl);
    Controller_SetRumbleOption(pl->controller, 0);

    if (gScene.m_bStreamAll && respawn) {
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

    if (gEngineApp_m_Game.m_Settings.multiplayer) {
        count = CScene__FindWarpDests(&gScene, -1, warp, recs, 32);
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
            pl->room = pl->spawnRoom = CScene__GetRegion(&gScene, recs[best].m_nID);
            CPlayer__LevelStart(pl);
            return;
        }
    } else {
        do { /* a block of its own: it decides which of the two addresses gets $s0 */
            if (CScene__FindWarpDests(&gScene, -1, warp, &rec, 1) && rec.m_nLevel == gScene.m_nLevel &&
                (gEngineApp_m_Game.m_Settings.multiplayer || gScene.m_bStreamAll)) {
                pl->spawnPos = rec.m_vPos;
                pl->pos = pl->spawnPos;
                pl->angle = pl->spawnAngle = rec.m_RotY;
                pl->room = pl->spawnRoom = CScene__GetRegion(&gScene, rec.m_nID);
                CPlayer__LevelStart(pl);
                return;
            }
        } while (0);
    }
    CScene__RequestWarp(&gScene, warp, arg2);
}
#else
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

    if (gScene.m_bStreamAll && respawn) {
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

    if (gEngineApp_m_Game.m_Settings.multiplayer) {
        count = CScene__FindWarpDests(&gScene, -1, warp, recs, 32);
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
            pl->room = pl->spawnRoom = CScene__GetRegion(&gScene, recs[best].m_nID);
            CPlayer__LevelStart(pl);
            return;
        }
    } else {
        do { /* a block of its own: it decides which of the two addresses gets $s0 */
            if (CScene__FindWarpDests(&gScene, -1, warp, &rec, 1) && rec.m_nLevel == gScene.m_nLevel &&
                (gEngineApp_m_Game.m_Settings.multiplayer || gScene.m_bStreamAll)) {
                pl->spawnPos = rec.m_vPos;
                pl->pos = pl->spawnPos;
                pl->angle = pl->spawnAngle = rec.m_RotY;
                pl->room = pl->spawnRoom = CScene__GetRegion(&gScene, rec.m_nID);
                CPlayer__LevelStart(pl);
                return;
            }
        } while (0);
    }
    CScene__RequestWarp(&gScene, warp, arg2);
}
#endif
