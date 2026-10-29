/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00287EE8.c, func_00287EE8, with Turok 2's layouts. */
#include "common.h"

/* Builds the frame's actor lists: every live level actor (flag 0x100) whose
 * type is shown in the current view is tested against the view's bounding
 * box (and frustum), or against the wider active box when it is not drawn;
 * the visible ones are animated (func_0021EBC0) and put in the world's lists
 * (drawn, ..., all, enemies, flagged 2, type 5, and actor types 0x64F and
 * 0x64D); the others lose their "visible" flag 0x200. */

typedef struct {
    s32 kind;               /* 0x00 */
    s32 flags;              /* 0x04: views it shows in, 0x100, 0x200 */
} ActorInfo;

typedef struct {
    u8 pad0[0x14];
    ActorInfo *info;        /* 0x014 */
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
    u8 pad0[0xE0];
    LevelActor *actors;     /* 0x0E0 */
    s32 first;              /* 0x0E4 */
    s32 count;              /* 0x0E8 */
    LevelActor *l0[0x200];  /* 0x0EC */
    s32 n0;                 /* 0x8EC */
    LevelActor *l1[0x80];   /* 0x8F0 */
    s32 n1;                 /* 0xAF0 */
    u8 padAF4[0x104];
    LevelActor *l2[0x80];   /* 0xBF8 */
    s32 n2;                 /* 0xDF8 */
    LevelActor *l3[0x40];   /* 0xDFC */
    s32 n3;                 /* 0xEFC */
    s32 cur3;               /* 0xF00 */
    LevelActor *l4[0x20];   /* 0xF04 */
    s32 n4;                 /* 0xF84 */
    LevelActor *l5[0x10];   /* 0xF88 */
    s32 n5;                 /* 0xFC8 */
    LevelActor *l6[0x20];   /* 0xFCC */
    s32 n6;                 /* 0x104C */
    LevelActor *l7[4];      /* 0x1050 */
    s32 n7;                 /* 0x1060 */
    u8 pad1064[0x13B28 - 0x1064];
    u8 unk13B28[0x18FA4 - 0x13B28]; /* 0x13B28 */
    s32 unk18FA4;           /* 0x18FA4 */
} World;

typedef struct {
    s32 mask;               /* 0x00 */
    u8 pad4[0xD];
    u8 unk11;               /* 0x11 */
} ViewState;

extern ViewState D_8011AAE0;
extern f32 D_800F5A34[];    /* view box max (min at -3) */
extern f32 D_800F5A18[];    /* active box: max at +4 (min at +1) */
extern s32 D_800ACDE4;

void func_002530A4(void *, World *);
s32 Cinema_PathPlaying(void);
s32 func_00267090(LevelActor *);
s32 func_002681BC(LevelActor *);
s32 func_0026F60C(void *, f32 *);
void func_0021EBC0(LevelActor *);
void func_0022828C(void *, World *);

void func_00221210(World *w) {
    s32 lim, mask, sflag, i, s2, t;
    s32 t0, t1, t2, t3, t4, t5, t6, t7;
    u8 *view;
    LevelActor *actors;
    LevelActor *o;
    ActorInfo *info;
    s32 fl;
    f32 *bB;
    f32 *bA;

    mask = D_8011AAE0.mask;
    w->n2 = 0;
    w->n3 = 0;
    w->n4 = 0;
    func_002530A4((u8 *)&D_8011AAE0 - 0x1270, w);
    view = NULL;
    if (D_8011AAE0.unk11 == 0) {
        view = *(u8 **)((u8 *)&D_8011AAE0 - 0x1210);
    }
    sflag = 0;
    if (Cinema_PathPlaying() == 0) {
        sflag = w->unk18FA4 == 0;
    }
    i = w->first;
    lim = w->count;
    actors = w->actors;
    if (i < lim) {
        bB = D_800F5A34;
        bA = D_800F5A18 + 1;
        do {
            o = &actors[i];
            fl = o->flags;
            if (fl & 0x100) {
                info = o->info;
                s2 = 0;
                if (info->flags & mask) {
                    if (sflag != 0 && !(fl & 2) && o->unkE3 == 0 && !(fl & 0x400) && info->kind != 3
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
                    if (!(info->flags & 0x200)) {
                        s2 = 0;
                    }
                } else if (!(info->flags & 0x100) && info->kind != 0xB) {
                    s2 = 0;
                }
                if (s2 != 0) {
                    func_0021EBC0(o);
                    if (func_00267090(o) != 0) {
                        t0 = w->n0;
                        if (t0 != 0x200) {
                            w->l0[t0] = o;
                            w->n0 = t0 + 1;
                        }
                    }
                    if (func_002681BC(o) != 0) {
                        t1 = w->n1;
                        if (t1 != 0x80) {
                            w->l1[t1] = o;
                            w->n1 = t1 + 1;
                        }
                    }
                    t2 = w->n2;
                    if (t2 < 0x80) {
                        w->l2[t2] = o;
                        w->n2 = t2 + 1;
                    }
                    t3 = w->n3;
                    if (t3 < 0x40 && o->info->kind == 1) {
                        w->l3[t3] = o;
                        {
                            s32 u = w->n3;
                            o->enemyIndex = u;
                            w->n3 = u + 1;
                        }
                    }
                    t4 = w->n4;
                    if (t4 < 0x20 && (o->flags & 2)) {
                        w->l4[t4] = o;
                        w->n4 = t4 + 1;
                    }
                    t5 = w->n5;
                    if (t5 < 0x10 && o->info->kind == 5) {
                        w->l5[t5] = o;
                        w->n5 = t5 + 1;
                    }
                    t6 = w->n6;
                    if (t6 < 0x20) {
                        if (o->type == 0x64F) {
                            w->l6[t6] = o;
                            w->n6 = t6 + 1;
                        }
                    }
                    if (o->type == 0x64D) {
                        t7 = w->n7;
                        if (t7 < 4) {
                            w->l7[t7] = o;
                            w->n7 = t7 + 1;
                        }
                    }
                } else {
                    o->flags &= ~0x200;
                }
            }
            i++;
        } while (i < lim);
    }
    func_0022828C(w->unk13B28, w);
    t = w->cur3 + 1;
    w->cur3 = t;
    if (t >= w->n3) {
        w->cur3 = 0;
    }
}
