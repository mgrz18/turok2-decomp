/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002149C0.c, func_002149C0, with Turok 2's layouts. */
#include "common.h"

typedef struct { f32 x, y, z; } Vec3f;

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ u8 pad4[0x14 - 4];
    /* 0x14 */ u32 flags14;
    /* 0x18 */ u8 pad18[0x48 - 0x18];
    /* 0x48 */ u32 flags48;         /* 0x40/0x80/0x100: per side */
    /* 0x4C */ u8 pad4C[0x94 - 0x4C];
    /* 0x94 */ u16 flags94;         /* 5 bits per side */
} ActorDef;

typedef struct Actor {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ s32 region;
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ u8 pad18[0x54 - 0x18];
    /* 0x054 */ f32 eye;
    /* 0x058 */ u8 pad58[0xD4 - 0x58];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x1A8 - 0xD8];
    /* 0x1A8 */ void *owner;
    /* 0x1AC */ u8 pad1AC[0x52A - 0x1AC];
    /* 0x52A */ s16 alive;          /* players */
    /* 0x52C */ u8 pad52C[0xA08 - 0x52C];
    /* 0xA08 */ s16 character;      /* players */
} Actor;

/* The first 0x34 bytes of an actor, saved around a line-of-sight test. */
typedef struct {
    s32 w[13];
} ActorHead;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ f32 height;
} CameraInfo;

typedef struct {
    /* 0x00 */ u32 *flags;
    /* 0x04 */ u8 pad4[0x18 - 4];
} CharacterInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x37];
    /* 0x37 */ s8 unk37;
    /* 0x38 */ u8 pad38[0x68 - 0x38];
    /* 0x68 */ Actor *ignore;
    /* 0x6C */ u8 pad6C[0x78 - 0x6C];
    /* 0x78 */ Actor *forced;
    /* 0x7C */ u8 pad7C[4];
    /* 0x80 */ Actor *approachFrom;
    /* 0x84 */ u8 pad84[0xAC - 0x84];
    /* 0xAC */ Vec3f pos;
    /* 0xB8 */ s32 region;
} Plan;

extern u8 D_800F7078[];
extern u8 D_80119870[];
extern u8 D_800F1B40[];
extern Actor *D_800F1CE0;
extern CharacterInfo D_800ADF5C[];

s32 func_002438C0(Actor *self, Plan *plan, Actor *target);
s32 func_002435AC(Actor *viewer, Actor *target, f32 range, f32 across, f32 up);
s32 func_0025E44C(Actor *self, Plan *plan);
s32 func_0025E428(Actor *self, Plan *plan);
void func_0025E52C(Actor *self, Plan *plan, Vec3f *pos, s32 arg3);
s32 Cinema_PathPlaying(void);
s32 func_00284408(void);
Actor *func_00225848(u8 *scene, Actor *self);
Actor *func_0022594C(u8 *scene, Actor *self);
Actor *func_002259D0(u8 *scene, Actor *self);
Actor *func_002532EC(u8 *players, Vec3f pos);
Actor *func_00253370(u8 *players, Vec3f pos, void *owner);
CameraInfo *func_00268A2C(Actor *player);
f32 func_002465F0(Actor *a, Vec3f pos);
f32 func_0024665C(Actor *a, Vec3f pos);
s32 func_00267E74(Actor *a);
f32 func_00268534(Actor *a);
f32 func_002671B4(Actor *a);                           /* height */
s32 func_00239F00(Actor *a, Vec3f from, u8 *result);

#define ADD(t) if ((t) != NULL) { list[n] = (t); n++; }

/* The plan's best target: the nearest of its candidates (sides' leaders,
 * the actor it ignores, a nearby creature, the nearest player) that it may
 * attack and, with `checkSight`, can see. */
Actor *func_00243C90(Actor *self, Plan *plan, s32 checkSight, s32 flag) {
    Actor *list[128];
    ActorHead saved;
    Vec3f eye;
    f32 range;
    f32 across;
    f32 up;
    f32 best;
    f32 d;
    f32 limit;
    f32 dist;
    Actor *bestA;
    Actor *t;
    CameraInfo *cam;
    s32 n;
    s32 i;
    s32 ok;
    s32 side;
    s32 r;

    if (plan->forced != NULL) {
        if (flag) {
            func_002438C0(self, plan, plan->forced);
        }
        return plan->forced;
    }
    if (self->def->kind == 1 && func_0025E44C(self, plan) != 2) {
        range = 2048.0f;
        across = 2.4434612f;
        up = 0.785398245f;
    } else {
        range = 2048.0f;
        across = 2.4434612f;
        up = 1.832596f;
    }
    n = 0;
    bestA = NULL;
    ok = 1;
    if (Cinema_PathPlaying()) {
        ok = 0;
        if (func_00284408() >= 10) {
            ok = func_00284408() < 61;
        }
    }
    if (ok) {
        if (flag) {
            if (self->def->kind == 1 && plan->approachFrom != NULL) {
                side = func_0025E428(self, plan);
                if ((side == 0 && (self->def->flags48 & 0x40))
                    || (side == 1 && (self->def->flags48 & 0x80))
                    || (side == 2 && (self->def->flags48 & 0x100))) {
                    t = func_00225848(D_800F7078, self);
                    ADD(t);
                }
                if (plan->approachFrom != NULL
                    && ((side == 0 && (self->def->flags94 & 0x1F))
                        || (side == 1 && (self->def->flags94 & 0x3E0))
                        || (side == 2 && (self->def->flags94 & 0x7C00)))) {
                    t = func_0022594C(D_800F7078, self);
                    ADD(t);
                }
            }
            t = plan->ignore;
            ADD(t);
            t = func_002259D0(D_800F7078, self);
            ADD(t);
        }
        if (self->flags & 0x300000) {
            t = func_00253370(D_80119870, self->pos, self->owner);
        } else {
            t = func_002532EC(D_80119870, self->pos);
        }
        if (t != NULL && t->alive != 0 && t->character != 1) {
            cam = func_00268A2C(t);
            if (cam != NULL) {
                range = cam->height - 102.399994f;
            }
            if (self->def->kind == 1 && plan->unk37 == 0 && (*D_800ADF5C[t->character].flags & 0x80)) {
                across *= 0.75f;
                up *= 0.75f;
                range *= 0.5f;
            }
            ADD(t);
        }
        best = 3.4028235e38f;
        i = 0;
        bestA = NULL;
        if (n > 0) {
            do {
                t = list[i];
                d = func_002465F0(t, self->pos);
                if (d < best) {
                    if (flag) {
                        if (self->def->kind == 1 && plan->unk37 == 0) {
                            goto next;
                        }
                        if (func_00267E74(self)) {
                            dist = func_0024665C(t, plan->pos);
                            limit = func_00268534(self);
                            if (limit * func_00268534(self) <= dist) {
                                goto next;
                            }
                        }
                        if (func_002438C0(self, plan, t) == 0) {
                            goto next;
                        }
                    }
                    if (checkSight) {
                        saved = *(ActorHead *)t;
                        t->pos.y = t->pos.y + func_002671B4(t);
                        t->pos.y = t->pos.y + t->eye;
                        eye = self->pos;
                        eye.y = eye.y + func_002671B4(self);
                        eye.y = eye.y + self->eye;
                        r = func_00239F00(t, eye, D_800F1B40);
                        if (D_800F1CE0 == self) {
                            r = 0;
                        }
                        *(ActorHead *)t = saved;
                        if (r) {
                            goto next;
                        }
                        if (self->def->kind == 1) {
                            if ((self->def->flags14 & 0x8000) && func_002435AC(self, t, range, across, up) == 0) {
                                if (plan->unk37 == 0) {
                                    goto next;
                                }
                            } else {
                                func_0025E52C(self, plan, &t->pos, 3);
                            }
                        }
                    }
                    best = d;
                    bestA = t;
                }
            next:
                i++;
            } while (i < n);
        }
    }
    if (flag && bestA == NULL && func_002438C0(self, plan, NULL) == 0) {
        plan->pos = self->pos;
        plan->region = self->region;
    }
    return bestA;
}
