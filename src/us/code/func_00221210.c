/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00287EE8.c, func_00287EE8, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "structs/world.h"
#include "globals/scene.h"
#include "protos/scene.h"
#ifdef TARGET_PORT
#include "structs/game.h"
extern CGame gEngineApp_m_Game;
#endif

/* Builds the frame's actor lists: every live level actor (flag 0x100) whose
 * type is shown in the current view is tested against the view's bounding
 * box (and frustum), or against the wider active box when it is not drawn;
 * the visible ones are animated (CGameObjectInstance__ClearDrawMatrices) and put in the world's lists
 * (drawn, ..., all, enemies, flagged 2, type 5, and actor types 0x64F and
 * 0x64D); the others lose their "visible" flag 0x200. */

/* gEngineApp_m_Settings (structs/game.h) seen from its difficulty, gEngineApp_m_Settings_difficulty: the
 * ROM bases the CGame and camera-pool reads below on this symbol, and with
 * gEngineApp_m_Settings / gEngineApp_m_Game the code no longer matches. */
typedef struct {
    s32 mask;               /* 0x00 */
    u8 pad4[0xD];
    u8 unk11;               /* 0x11 */
} ViewState;

extern ViewState gEngineApp_m_Settings_difficulty; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern f32 D_800F5A34[];    /* view box max (min at -3) */
extern f32 D_800F5A18[];    /* active box: max at +4 (min at +1) */

void CPlayerPool__RegisterActors(void *, CScene *);
s32 Cinema_PathPlaying(void);
s32 GameObject_IsSolid(CGameObjectInstance *);
s32 func_002681BC(CGameObjectInstance *);
s32 Frustum_TestBox(void *, f32 *);
void CGameObjectInstance__ClearDrawMatrices(CGameObjectInstance *);
void CInstancePool__UpdateVisibility(void *, CScene *);

void CScene__BuildActorLists(CScene *w) {
    s32 lim, mask, sflag, i, s2, t;
    s32 t0, t1, t2, t3, t4, t5, t6, t7;
    u8 *view;
    CGameObjectInstance *actors;
    CGameObjectInstance *o;
    CIntelligence *info;
    s32 fl;
    f32 *bB;
    f32 *bA;

    mask = gEngineApp_m_Settings_difficulty.mask;
    w->m_nRegistered = 0;
    w->m_nKind1 = 0;
    w->m_nFlagged = 0;
#ifdef TARGET_PORT /* the CGame / CCamera fields by name (host layout) */
    CPlayerPool__RegisterActors(&gEngineApp_m_Game.m_PlayerList, w);
#else
    CPlayerPool__RegisterActors((u8 *)&gEngineApp_m_Settings_difficulty - 0x1270, w);
#endif
    view = NULL;
    if (gEngineApp_m_Settings_difficulty.unk11 == 0) {
#ifdef TARGET_PORT /* the CGame / CCamera fields by name (host layout) */
        view = gEngineApp_m_Game.m_CameraPool->m_ActiveList.m_pHead;
#else
        view = *(u8 **)((u8 *)&gEngineApp_m_Settings_difficulty - 0x1210);
#endif
    }
    sflag = 0;
    if (Cinema_PathPlaying() == 0) {
        sflag = w->m_bStreamAll == 0;
    }
    i = w->m_nFirstActor;
    lim = w->m_nActors;
    actors = (CGameObjectInstance *)w->m_pActors;
    if (i < lim) {
        bB = D_800F5A34;
        bA = D_800F5A18 + 1;
        do {
            o = &actors[i];
            fl = o->m_dwFlags;
            if (fl & 0x100) {
                info = o->header.m_pIntelligence;
                s2 = 0;
                if (info->m_dwFlags & mask) {
                    if (sflag != 0 && !(fl & 2) && o->m_CurrentAnimation.m_bAnimActive == 0 && !(fl & 0x400) && info->m_Type != 3
                        && GameObject_IsSolid(o) == 0) {
                        if (bB[0] > o->m_vMin.x && bB[-3] < o->m_vMax.x && bB[2] > o->m_vMin.z
                            && bB[-1] < o->m_vMax.z && bB[1] > o->m_vMin.y && bB[-2] < o->m_vMax.y) {
                            if (view != NULL) {
#ifdef TARGET_PORT /* the CGame / CCamera fields by name (host layout) */
                                s2 = Frustum_TestBox(((CCamera *)view)->m_ViewVolume, &o->m_vMin.x);
#else
                                s2 = Frustum_TestBox(view + 0x2D8, &o->m_vMin.x);
#endif
                            } else {
                                goto set1;
                            }
                        }
                    } else {
                        s2 = 0;
                        if (bA[0] > o->m_vMin.x && bA[-3] < o->m_vMax.x && bA[2] > o->m_vMin.z
                            && bA[-1] < o->m_vMax.z && bA[1] > o->m_vMin.y && bA[-2] < o->m_vMax.y) {
                        set1:
                            s2 = 1;
                        }
                    }
                }
                if (Cinema_PathPlaying() != 0 && gCinemaHidesActors != 0) {
                    if (!(info->m_dwFlags & 0x200)) {
                        s2 = 0;
                    }
                } else if (!(info->m_dwFlags & 0x100) && info->m_Type != 0xB) {
                    s2 = 0;
                }
                if (s2 != 0) {
                    CGameObjectInstance__ClearDrawMatrices(o);
                    if (GameObject_IsSolid(o) != 0) {
                        t0 = w->m_nDrawActors;
                        if (t0 != 0x200) {
                            w->m_pDrawActors[t0] = (CGameObjectInstance *)o;
                            w->m_nDrawActors = t0 + 1;
                        }
                    }
                    if (func_002681BC(o) != 0) {
                        t1 = w->m_nMovers;
                        if (t1 != 0x80) {
                            w->m_pMovers[t1] = (CGameObjectInstance *)o;
                            w->m_nMovers = t1 + 1;
                        }
                    }
                    t2 = w->m_nRegistered;
                    if (t2 < 0x80) {
                        w->m_pRegistered[t2] = (CGameObjectInstance *)o;
                        w->m_nRegistered = t2 + 1;
                    }
                    t3 = w->m_nKind1;
                    if (t3 < 0x40 && o->header.m_pIntelligence->m_Type == 1) {
                        w->m_pKind1[t3] = (CGameObjectInstance *)o;
                        {
                            s32 u = w->m_nKind1;
                            o->m_AI.m_PathFindCount = u;
                            w->m_nKind1 = u + 1;
                        }
                    }
                    t4 = w->m_nFlagged;
                    if (t4 < 0x20 && (o->m_dwFlags & 2)) {
                        w->m_pFlagged[t4] = (CGameObjectInstance *)o;
                        w->m_nFlagged = t4 + 1;
                    }
                    t5 = w->m_nKind5;
                    if (t5 < 0x10 && o->header.m_pIntelligence->m_Type == 5) {
                        w->m_pKind5[t5] = (CGameObjectInstance *)o;
                        w->m_nKind5 = t5 + 1;
                    }
                    t6 = w->m_nList6;
                    if (t6 < 0x20) {
                        if (o->m_nTypeFlag == 0x64F) {
                            w->m_pList6[t6] = (CGameObjectInstance *)o;
                            w->m_nList6 = t6 + 1;
                        }
                    }
                    if (o->m_nTypeFlag == 0x64D) {
                        t7 = w->m_nList7;
                        if (t7 < 4) {
                            w->m_pList7[t7] = (CGameObjectInstance *)o;
                            w->m_nList7 = t7 + 1;
                        }
                    }
                } else {
                    o->m_dwFlags &= ~0x200;
                }
            }
            i++;
        } while (i < lim);
    }
    CInstancePool__UpdateVisibility(&w->m_InstancePool, w);
    t = w->m_Kind1Cursor + 1;
    w->m_Kind1Cursor = t;
    if (t >= w->m_nKind1) {
        w->m_Kind1Cursor = 0;
    }
}
