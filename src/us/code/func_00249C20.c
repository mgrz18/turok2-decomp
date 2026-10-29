/* func_00249C20 and func_0024A0DC, one source file: the file's .rodata
 * opens with func_00249C20's literals at 0x800A6AF0, and func_0024A0DC's
 * jump table at 0x800A6B28 is 8-aligned from there (from its own start,
 * 0x800A6B0C, it would not be). */
#include "common.h"

#define TRUE 1
#define FALSE 0

/* Weapon auto-aim: finds the target closest to the player's aim direction
 * within the weapon's cone and returns the rotation that turns the aim
 * toward it (identity when there is none). */

typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

typedef struct Quat {
    f32 x, y, z, w;
} Quat;

typedef struct WeaponDef {
    /* 0x00 */ u8 pad0[0xE];
    /* 0x0E */ s16 aimHold;
    /* 0x10 */ s16 lockHold;
    /* 0x12 */ u8 pad12[0x1C - 0x12];
    /* 0x1C */ f32 aimCone;
} WeaponDef;

typedef struct AimOptions {
    /* 0x00 */ u8 pad0[0x2E];
    /* 0x2E */ u8 autoAim;
} AimOptions;

typedef struct AimView {
    /* 0x000 */ u8 pad0[0x14C];
    /* 0x14C */ f32 mtx[4][4];
} AimView;

typedef struct AimActor AimActor;
struct AimActor {
    /* 0x0000 */ u8 pad0[4];
    /* 0x0004 */ Vec3f pos;
    /* 0x0010 */ u8 pad10[0x54 - 0x10];
    /* 0x0054 */ f32 height;
    /* 0x0058 */ u8 pad58[0x214 - 0x58];
    /* 0x0214 */ Vec3f aimPos;
    /* 0x0220 */ u8 pad220[0x518 - 0x220];
    /* 0x0518 */ AimOptions *options;
    /* 0x051C */ AimView *view;
    /* 0x0520 */ u8 pad520[0x996 - 0x520];
    /* 0x0996 */ s16 weapon;
    /* 0x0998 */ u8 pad998[0xB2C - 0x998];
    /* 0x0B2C */ s32 lockState;
    /* 0x0B30 */ f32 lockTimer;
    /* 0x0B34 */ u8 padB34[4];
    /* 0x0B38 */ AimActor *lockTarget;
    /* 0x0B3C */ s32 hold;
    /* 0x0B40 */ Vec3f aimPoint;
    /* 0x0B4C */ Vec3f aimVel;
    /* 0x0B58 */ AimActor *seenTarget;
    /* 0x0B5C */ s32 range;
    /* 0x0B60 */ u8 padB60[4];
    /* 0x0B64 */ Vec3f viewPoint;
    /* 0x0B70 */ Vec3f viewVel;
    /* 0x0B7C */ Quat aimRot;
    /* 0x0B8C */ s32 unkB8C;
    /* 0x0B90 */ u8 padB90[0xCC4 - 0xB90];
    /* 0x0CC4 */ s32 unkCC4;
    /* 0x0CC8 */ u8 padCC8[0x1320 - 0xCC8];
    /* 0x1320 */ AimActor *next;
};

/* Game state from the player list on; the multiplayer flag is at +0x1261. */
typedef struct GameState {
    /* 0x0000 */ AimActor *players;
    /* 0x0004 */ u8 pad4[0x1261 - 4];
    /* 0x1261 */ u8 multiplayer;
} GameState;

extern GameState D_80119890;
extern WeaponDef *D_800AF618[];
extern s32 D_800F7E70;
extern AimActor *D_800F7C70[];
extern f32 D_800DD78C;

void func_0021F00C(Vec3f *out, AimActor *actor, Vec3f off);
f32 func_00253CFC(AimActor *);
f32 func_002974C0(f32);
f32 func_002982F0(f32);
f32 func_00298470(f32);
f32 func_0021153C(f32);
s32 func_00249A58(AimActor *, AimActor *);
f32 func_002671B4(AimActor *);
void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);
void func_0020EFDC(Vec3f *out, Vec3f *a, Vec3f *b);
f32 func_0020F040(Vec3f *);
Quat func_0021AE6C(AimActor *);
void func_00210F68(Quat *out, Quat *a, Quat *b);
extern f32 D_800B6D28;
void func_0020F85C(f32 mtx[4][4], Vec3f *in, Vec3f *out);
void func_0020EF60(Vec3f *out, Vec3f *a, f32 s);
void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);

Quat func_00249C20(AimActor *pl, AimActor **outTarget, Vec3f *outPos) {
    Vec3f tpos;
    Vec3f dir;
    Vec3f eye;
    Vec3f bestPos;
    Vec3f fwd;
    Vec3f off;
    Vec3f bestDir;
    Vec3f axis;
    Quat turn;
    Quat rot;
    Quat result;
    AimActor *best;
    AimActor *t;
    f32 cone;
    f32 bestDot;
    f32 dot;
    f32 s;
    f32 angle;
    f32 theta;
    f32 half;
    s32 i;
    s32 n;

    off.x = 0.0f;
    off.y = 0.0f;
    off.z = 1.0f;
    func_0021F00C(&fwd, pl, off);
    eye = pl->pos;
    eye.y += func_00253CFC(pl);
    cone = D_800AF618[pl->weapon]->aimCone;
    best = NULL;
    bestDot = func_002974C0(cone * 0.5f);
    bestPos.x = 0.0f;
    bestPos.y = 0.0f;
    bestPos.z = 0.0f;
    bestDir.x = 0.0f;
    bestDir.y = 0.0f;
    bestDir.z = 0.0f;

    if (D_80119890.multiplayer) {
        for (t = D_80119890.players; t != NULL; t = t->next) {
            if (func_00249A58(pl, t) && t != pl) {
                if (pl->weapon == 29) {
                    tpos = t->aimPos;
                } else {
                    tpos = t->pos;
                    tpos.y += t->height;
                    tpos.y += func_002671B4(t) * 0.75f;
                }
                func_0020EF2C(&dir, &tpos, &eye);
                func_0020F040(&dir);
                dot = fwd.x * dir.x + fwd.y * dir.y + fwd.z * dir.z;
                if (bestDot < dot) {
                    bestDot = dot;
                    bestPos = tpos;
                    best = t;
                }
            }
        }
    }

    n = D_800F7E70;
    for (i = 0; i < n; i++) {
        t = D_800F7C70[i];
        if (func_00249A58(pl, t)) {
            if (pl->weapon == 13) {
                tpos = t->aimPos;
            } else {
                tpos = t->pos;
                tpos.y += t->height;
                tpos.y += func_002671B4(t) * 0.75f;
            }
            func_0020EF2C(&dir, &tpos, &eye);
            func_0020F040(&dir);
            dot = fwd.x * dir.x + fwd.y * dir.y + fwd.z * dir.z;
            if (bestDot < dot) {
                bestDot = dot;
                best = t;
                bestPos = tpos;
                bestDir = dir;
            }
        }
    }

    *outTarget = best;
    *outPos = bestPos;
    if (best != NULL) {
        func_0020EFDC(&axis, &fwd, &bestDir);
        func_0020F040(&axis);
        theta = func_0021153C(bestDot);
        half = 0.5f;
        angle = theta * func_00298470(func_00298470(1.0f - theta / (cone * half))) * half;
        s = func_002982F0(angle);
        turn.x = axis.x * s;
        turn.y = axis.y * s;
        turn.z = axis.z * s;
        D_800DD78C = s;
        turn.w = func_002974C0(angle);
        rot = func_0021AE6C(pl);
        func_00210F68(&result, &rot, &turn);
    } else {
        result.x = result.y = result.z = 0.0f;
        result.w = 1.0f;
    }
    return result;
}

/* Per-frame weapon aiming: the aim rotation, the auto-aim target lock
 * (acquire, hold, switch target) and the two spring-damped aim points. */
void func_0024A0DC(AimActor *pl) {
    Vec3f tpos;
    Vec3f off;
    Vec3f delta;
    Vec3f aimDir;
    AimActor *target;
    AimView *view;
    s32 canLock;
    s32 hold;
    s32 range;
    f32 stiff;
    f32 damp;
    f32 switchTime;
    f32 lockTime;

    view = pl->view;
    target = NULL;
    off.x = 0.0f;
    off.y = 0.0f;
    off.z = -1024.0f;
    func_0020F85C(view->mtx, &off, &aimDir);
    canLock = TRUE;
    stiff = 0.080000006f;
    damp = 0.52f;
    hold = D_800AF618[pl->weapon]->aimHold;
    range = hold + 100;

    switch (pl->weapon) {
    case 13:
        lockTime = switchTime = 15.0f;
        break;
    case 16:
        lockTime = switchTime = 7.5f;
        break;
    case 29:
        lockTime = switchTime = 11.25f;
        break;
    default:
        lockTime = switchTime = 0.0f;
        break;
    }

    switch (pl->weapon) {
    case 10:
    case 27:
        if (pl->unkB8C == 0) {
            break;
        }
        if (pl->unkCC4 != 2) {
            break;
        }
        hold = 740;
        range = -1;
        break;
    case 3:
        if (pl->unkCC4 == 2) {
            break;
        }
        hold = 730;
        range = -1;
        break;
    case 13:
    case 16:
    case 29:
        func_00249C20(pl, &target, &tpos);
        canLock = FALSE;
        break;
    default:
        range = -1;
        break;
    }

    pl->aimRot = func_0021AE6C(pl);
    if (target == NULL) {
        tpos = aimDir;
        if (pl->options->autoAim && canLock) {
            Vec3f tpos2;
            Quat rot2;
            AimActor *target2;

            rot2 = func_00249C20(pl, &target2, &tpos2);
            if (target2 != NULL) {
                pl->aimRot = rot2;
            }
        }
    }

    if (hold == -1) {
        pl->lockState = 0;
    } else if (pl->lockState == 0) {
        pl->lockState = 1;
        pl->aimPoint = aimDir;
        pl->aimVel.x = 0.0f;
        pl->aimVel.y = 0.0f;
        pl->aimVel.z = 0.0f;
        pl->viewPoint = aimDir;
        pl->viewVel.x = 0.0f;
        pl->viewVel.y = 0.0f;
        pl->viewVel.z = 0.0f;
    }

    func_0020EF2C(&delta, &tpos, &pl->viewPoint);
    func_0020EF60(&delta, &delta, stiff * 1.25f);
    func_0020EEF8(&pl->viewVel, &pl->viewVel, &delta);
    func_0020EEF8(&pl->viewPoint, &pl->viewPoint, &pl->viewVel);
    func_0020EF60(&pl->viewVel, &pl->viewVel, damp * 1.25f);

    if (target != NULL) {
        pl->range = D_800AF618[pl->weapon]->lockHold + 100;
    } else {
        pl->range = range;
    }

    switch (pl->lockState) {
    case 0:
        tpos = aimDir;
        break;
    case 1:
        if (target == NULL || pl->lockTarget != target) {
            pl->lockTarget = target;
            pl->lockTimer = 0.0f;
        }
        pl->lockTimer += D_800B6D28;
        if (lockTime <= pl->lockTimer) {
            pl->lockState = 2;
            pl->lockTimer = 0.0f;
        }
        tpos = aimDir;
        break;
    case 2:
        if (pl->seenTarget != target) {
            pl->seenTarget = target;
            pl->lockTimer = 0.0f;
        } else {
            pl->lockTimer += D_800B6D28;
        }
        if (target != NULL && pl->lockTarget != target && switchTime <= pl->lockTimer) {
            pl->lockTarget = target;
            pl->lockTimer = 0.0f;
        }
        target = pl->lockTarget;
        if (target != NULL) {
            if (!func_00249A58(pl, target)) {
                pl->lockState = 1;
                pl->lockTimer = 0.0f;
            }
            if (pl->weapon == 13 || pl->weapon == 29) {
                tpos = target->aimPos;
            } else {
                tpos = target->pos;
                tpos.y += target->height;
                tpos.y += func_002671B4(target) * 0.75f;
            }
            hold = D_800AF618[pl->weapon]->lockHold;
        } else {
            tpos = aimDir;
        }
        break;
    }

    func_0020EF2C(&delta, &tpos, &pl->aimPoint);
    func_0020EF60(&delta, &delta, stiff);
    func_0020EEF8(&pl->aimVel, &pl->aimVel, &delta);
    func_0020EEF8(&pl->aimPoint, &pl->aimPoint, &pl->aimVel);
    func_0020EF60(&pl->aimVel, &pl->aimVel, damp);
    pl->hold = hold;
}
