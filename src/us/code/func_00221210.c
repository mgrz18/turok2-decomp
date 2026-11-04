/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00287EE8.c, func_00287EE8, with Turok 2's layouts. */
#include "common.h"
#include "structs/intelligence.h"
#include "structs/world.h"

/* Builds the frame's actor lists: every live level actor (flag 0x100) whose
 * type is shown in the current view is tested against the view's bounding
 * box (and frustum), or against the wider active box when it is not drawn;
 * the visible ones are animated (func_0021EBC0) and put in the world's lists
 * (drawn, ..., all, enemies, flagged 2, type 5, and actor types 0x64F and
 * 0x64D); the others lose their "visible" flag 0x200. */

typedef struct {
    u8 pad0[0x14];
    CIntelligence *info;        /* 0x014 */
    u8 pad18[0xA0];
    u16 type;               /* 0x0B8 */
    u8 padBA[2];
    f32 boxMin[3];          /* 0x0BC */
    f32 boxMax[3];          /* 0x0C8 */
    s32 flags;              /* 0x0D4 */
    u8 padD8[0xB];
    s8 unkE3;               /* 0x0E3 */
    u8 padE4[0x126];
    s8 enemyIndex;          /* 0x20A */
    u8 pad20B[0x7D];
} LevelActor;               /* 0x288 */

typedef struct {
    s32 mask;               /* 0x00 */
    u8 pad4[0xD];
    u8 unk11;               /* 0x11 */
} ViewState;

extern ViewState D_8011AAE0;
extern f32 D_800F5A34[];    /* view box max (min at -3) */
extern f32 D_800F5A18[];    /* active box: max at +4 (min at +1) */
extern s32 D_800ACDE4;

void func_002530A4(void *, CWorld *);
s32 Cinema_PathPlaying(void);
s32 func_00267090(LevelActor *);
s32 func_002681BC(LevelActor *);
s32 func_0026F60C(void *, f32 *);
void func_0021EBC0(LevelActor *);
void func_0022828C(void *, CWorld *);

void func_00221210(CWorld *w) {
    s32 lim, mask, sflag, i, s2, t;
    s32 t0, t1, t2, t3, t4, t5, t6, t7;
    u8 *view;
    LevelActor *actors;
    LevelActor *o;
    CIntelligence *info;
    s32 fl;
    f32 *bB;
    f32 *bA;

    mask = D_8011AAE0.mask;
    w->m_nRegistered = 0;
    w->m_nKind1 = 0;
    w->m_nFlagged = 0;
    func_002530A4((u8 *)&D_8011AAE0 - 0x1270, w);
    view = NULL;
    if (D_8011AAE0.unk11 == 0) {
        view = *(u8 **)((u8 *)&D_8011AAE0 - 0x1210);
    }
    sflag = 0;
    if (Cinema_PathPlaying() == 0) {
        sflag = w->m_bStreamAll == 0;
    }
    i = w->m_nFirstActor;
    lim = w->m_nActors;
    actors = (LevelActor *)w->m_pActors;
    if (i < lim) {
        bB = D_800F5A34;
        bA = D_800F5A18 + 1;
        do {
            o = &actors[i];
            fl = o->flags;
            if (fl & 0x100) {
                info = o->info;
                s2 = 0;
                if (info->m_dwFlags & mask) {
                    if (sflag != 0 && !(fl & 2) && o->unkE3 == 0 && !(fl & 0x400) && info->m_Type != 3
                        && func_00267090(o) == 0) {
                        if (bB[0] > o->boxMin[0] && bB[-3] < o->boxMax[0] && bB[2] > o->boxMin[2]
                            && bB[-1] < o->boxMax[2] && bB[1] > o->boxMin[1] && bB[-2] < o->boxMax[1]) {
                            if (view != NULL) {
                                s2 = func_0026F60C(view + 0x2D8, o->boxMin);
                            } else {
                                goto set1;
                            }
                        }
                    } else {
                        s2 = 0;
                        if (bA[0] > o->boxMin[0] && bA[-3] < o->boxMax[0] && bA[2] > o->boxMin[2]
                            && bA[-1] < o->boxMax[2] && bA[1] > o->boxMin[1] && bA[-2] < o->boxMax[1]) {
                        set1:
                            s2 = 1;
                        }
                    }
                }
                if (Cinema_PathPlaying() != 0 && D_800ACDE4 != 0) {
                    if (!(info->m_dwFlags & 0x200)) {
                        s2 = 0;
                    }
                } else if (!(info->m_dwFlags & 0x100) && info->m_Type != 0xB) {
                    s2 = 0;
                }
                if (s2 != 0) {
                    func_0021EBC0(o);
                    if (func_00267090(o) != 0) {
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
                    if (t3 < 0x40 && o->info->m_Type == 1) {
                        w->m_pKind1[t3] = (CGameObjectInstance *)o;
                        {
                            s32 u = w->m_nKind1;
                            o->enemyIndex = u;
                            w->m_nKind1 = u + 1;
                        }
                    }
                    t4 = w->m_nFlagged;
                    if (t4 < 0x20 && (o->flags & 2)) {
                        w->m_pFlagged[t4] = (CGameObjectInstance *)o;
                        w->m_nFlagged = t4 + 1;
                    }
                    t5 = w->m_nKind5;
                    if (t5 < 0x10 && o->info->m_Type == 5) {
                        w->m_pKind5[t5] = (CGameObjectInstance *)o;
                        w->m_nKind5 = t5 + 1;
                    }
                    t6 = w->m_nList6;
                    if (t6 < 0x20) {
                        if (o->type == 0x64F) {
                            w->m_pList6[t6] = (CGameObjectInstance *)o;
                            w->m_nList6 = t6 + 1;
                        }
                    }
                    if (o->type == 0x64D) {
                        t7 = w->m_nList7;
                        if (t7 < 4) {
                            w->m_pList7[t7] = (CGameObjectInstance *)o;
                            w->m_nList7 = t7 + 1;
                        }
                    }
                } else {
                    o->flags &= ~0x200;
                }
            }
            i++;
        } while (i < lim);
    }
    func_0022828C(w->unk_13B28, w);
    t = w->m_Kind1Cursor + 1;
    w->m_Kind1Cursor = t;
    if (t >= w->m_nKind1) {
        w->m_Kind1Cursor = 0;
    }
}
