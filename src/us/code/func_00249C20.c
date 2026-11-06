/* Player_FindAutoAimTarget and Player_UpdateAim, one source file: the file's .rodata
 * opens with Player_FindAutoAimTarget's literals at 0x800A6AF0, and Player_UpdateAim's
 * jump table at 0x800A6B28 is 8-aligned from there (from its own start,
 * 0x800A6B0C, it would not be). */
#include "common.h"
#include "structs/game.h"
#include "structs/vector.h"
#include "structs/weapon.h"

#define TRUE 1
#define FALSE 0

/* Weapon auto-aim: finds the target closest to the player's aim direction
 * within the weapon's cone and returns the rotation that turns the aim
 * toward it (identity when there is none). */

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
    /* 0x0004 */ CVector3 pos;
    /* 0x0010 */ u8 pad10[0x54 - 0x10];
    /* 0x0054 */ f32 height;
    /* 0x0058 */ u8 pad58[0x214 - 0x58];
    /* 0x0214 */ CVector3 aimPos;
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
    /* 0x0B40 */ CVector3 aimPoint;
    /* 0x0B4C */ CVector3 aimVel;
    /* 0x0B58 */ AimActor *seenTarget;
    /* 0x0B5C */ s32 range;
    /* 0x0B60 */ u8 padB60[4];
    /* 0x0B64 */ CVector3 viewPoint;
    /* 0x0B70 */ CVector3 viewVel;
    /* 0x0B7C */ CQuaternion aimRot;
    /* 0x0B8C */ s32 unkB8C;
    /* 0x0B90 */ u8 padB90[0xCC4 - 0xB90];
    /* 0x0CC4 */ s32 unkCC4;
    /* 0x0CC8 */ u8 padCC8[0x1320 - 0xCC8];
    /* 0x1320 */ AimActor *next;
};

extern s32 D_800F7E70;
extern AimActor *D_800F7C70[];
extern f32 D_800DD78C;

void CGameObjectInstance__LocalToWorld(CVector3 *out, AimActor *actor, CVector3 off);
f32 Actor_GetEyeHeight(AimActor *);
f32 func_002974C0(f32);
f32 func_002982F0(f32);
f32 func_00298470(f32);
f32 Math_AcosLookup(f32);
s32 CPlayer__CanLockOn(AimActor *, AimActor *);
f32 GameObject_GetCollisionHeight(AimActor *);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);
f32 CVector3__Normalize(CVector3 *);
CQuaternion CGameObjectInstance__GetRotation(AimActor *);
void CQuaternion__Multiply(CQuaternion *out, CQuaternion *a, CQuaternion *b);
extern f32 D_800B6D28;
void CMtxF__TransformPoint(f32 mtx[4][4], CVector3 *in, CVector3 *out);
void CVector3__Scale(CVector3 *out, CVector3 *a, f32 s);
void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);

CQuaternion Player_FindAutoAimTarget(AimActor *pl, AimActor **outTarget, CVector3 *outPos) {
    CVector3 tpos;
    CVector3 dir;
    CVector3 eye;
    CVector3 bestPos;
    CVector3 fwd;
    CVector3 off;
    CVector3 bestDir;
    CVector3 axis;
    CQuaternion turn;
    CQuaternion rot;
    CQuaternion result;
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
    CGameObjectInstance__LocalToWorld(&fwd, pl, off);
    eye = pl->pos;
    eye.y += Actor_GetEyeHeight(pl);
    cone = D_800AF618[pl->weapon]->aimCone;
    best = NULL;
    bestDot = func_002974C0(cone * 0.5f);
    bestPos.x = 0.0f;
    bestPos.y = 0.0f;
    bestPos.z = 0.0f;
    bestDir.x = 0.0f;
    bestDir.y = 0.0f;
    bestDir.z = 0.0f;

    if (D_80119870.m_Settings.multiplayer) {
        for (t = (AimActor *)D_80119870.m_PlayerList.m_ActiveList.m_pHead; t != NULL; t = t->next) {
            if (CPlayer__CanLockOn(pl, t) && t != pl) {
                if (pl->weapon == 29) {
                    tpos = t->aimPos;
                } else {
                    tpos = t->pos;
                    tpos.y += t->height;
                    tpos.y += GameObject_GetCollisionHeight(t) * 0.75f;
                }
                CVector3__Subtract(&dir, &tpos, &eye);
                CVector3__Normalize(&dir);
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
        if (CPlayer__CanLockOn(pl, t)) {
            if (pl->weapon == 13) {
                tpos = t->aimPos;
            } else {
                tpos = t->pos;
                tpos.y += t->height;
                tpos.y += GameObject_GetCollisionHeight(t) * 0.75f;
            }
            CVector3__Subtract(&dir, &tpos, &eye);
            CVector3__Normalize(&dir);
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
        CVector3__Cross(&axis, &fwd, &bestDir);
        CVector3__Normalize(&axis);
        theta = Math_AcosLookup(bestDot);
        half = 0.5f;
        angle = theta * func_00298470(func_00298470(1.0f - theta / (cone * half))) * half;
        s = func_002982F0(angle);
        turn.x = axis.x * s;
        turn.y = axis.y * s;
        turn.z = axis.z * s;
        D_800DD78C = s;
        turn.w = func_002974C0(angle);
        rot = CGameObjectInstance__GetRotation(pl);
        CQuaternion__Multiply(&result, &rot, &turn);
    } else {
        result.x = result.y = result.z = 0.0f;
        result.w = 1.0f;
    }
    return result;
}

/* Per-frame weapon aiming: the aim rotation, the auto-aim target lock
 * (acquire, hold, switch target) and the two spring-damped aim points. */
void Player_UpdateAim(AimActor *pl) {
    CVector3 tpos;
    CVector3 off;
    CVector3 delta;
    CVector3 aimDir;
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
    CMtxF__TransformPoint(view->mtx, &off, &aimDir);
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
        Player_FindAutoAimTarget(pl, &target, &tpos);
        canLock = FALSE;
        break;
    default:
        range = -1;
        break;
    }

    pl->aimRot = CGameObjectInstance__GetRotation(pl);
    if (target == NULL) {
        tpos = aimDir;
        if (pl->options->autoAim && canLock) {
            CVector3 tpos2;
            CQuaternion rot2;
            AimActor *target2;

            rot2 = Player_FindAutoAimTarget(pl, &target2, &tpos2);
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

    CVector3__Subtract(&delta, &tpos, &pl->viewPoint);
    CVector3__Scale(&delta, &delta, stiff * 1.25f);
    CVector3__Add(&pl->viewVel, &pl->viewVel, &delta);
    CVector3__Add(&pl->viewPoint, &pl->viewPoint, &pl->viewVel);
    CVector3__Scale(&pl->viewVel, &pl->viewVel, damp * 1.25f);

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
            if (!CPlayer__CanLockOn(pl, target)) {
                pl->lockState = 1;
                pl->lockTimer = 0.0f;
            }
            if (pl->weapon == 13 || pl->weapon == 29) {
                tpos = target->aimPos;
            } else {
                tpos = target->pos;
                tpos.y += target->height;
                tpos.y += GameObject_GetCollisionHeight(target) * 0.75f;
            }
            hold = D_800AF618[pl->weapon]->lockHold;
        } else {
            tpos = aimDir;
        }
        break;
    }

    CVector3__Subtract(&delta, &tpos, &pl->aimPoint);
    CVector3__Scale(&delta, &delta, stiff);
    CVector3__Add(&pl->aimVel, &pl->aimVel, &delta);
    CVector3__Add(&pl->aimPoint, &pl->aimPoint, &pl->aimVel);
    CVector3__Scale(&pl->aimVel, &pl->aimVel, damp);
    pl->hold = hold;
}
