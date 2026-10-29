#include "common.h"

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
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 aimCone;
} WeaponDef;

typedef struct AimActor AimActor;
struct AimActor {
    /* 0x0000 */ u8 pad0[4];
    /* 0x0004 */ Vec3f pos;
    /* 0x0010 */ u8 pad10[0x54 - 0x10];
    /* 0x0054 */ f32 height;
    /* 0x0058 */ u8 pad58[0x214 - 0x58];
    /* 0x0214 */ Vec3f aimPos;
    /* 0x0220 */ u8 pad220[0x996 - 0x220];
    /* 0x0996 */ s16 weapon;
    /* 0x0998 */ u8 pad998[0x1320 - 0x998];
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
