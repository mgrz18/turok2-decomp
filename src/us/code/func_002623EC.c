#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3;

typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ void *obj;
    /* 0x08 */ f32 angle;
    /* 0x0C */ u8 padC[0x40 - 0xC];
    /* 0x40 */ f32 dist;
} VecDst; /* 0x44 */

typedef struct {
    /* 0x00 */ VecDst a;
    /* 0x44 */ VecDst b;
} Targets;

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ f32 turnRate;
    /* 0x44 */ f32 turnArc;
    /* 0x48 */ f32 aimRange;
    /* 0x4C */ f32 aimFov;
    /* 0x50 */ f32 fireRange;
    /* 0x54 */ f32 fireFov;
    /* 0x58 */ u8 pad58[0x6C - 0x58];
    /* 0x6C */ u8 aimMode;
    /* 0x6D */ u8 idleMode;
} TurretParams;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ TurretParams params;
} TurretDef;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3 pos;
    /* 0x10 */ u8 pad10[0x14 - 0x10];
    /* 0x14 */ TurretDef *def;
    /* 0x18 */ u8 pad18[0x50 - 0x18];
    /* 0x50 */ f32 rotY;
} Turret;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3 pos;
} TargetActor;

typedef struct {
    /* 0x000 */ u8 pad0[0x80];
    /* 0x080 */ TargetActor *target;
    /* 0x084 */ u8 pad84[0x98 - 0x84];
    /* 0x098 */ f32 baseRotY;
    /* 0x09C */ u8 pad9C[0x10C - 0x9C];
    /* 0x10C */ f32 pitch;
} AI;

void func_00210EF0(f32 *angle);
void func_00246108(Turret *self, AI *ai, Targets *out, s32 f1, s32 f2);
f32 func_002671B4(void *actor);
void func_0020EF2C(Vec3 *out, Vec3 *a, Vec3 *b);
f32 func_0020F5C0(Vec3 *v);
void func_00262150(Turret *self, AI *ai, s32 mode, TargetActor *target);
void func_00245BAC(Turret *self, AI *ai, f32 turnRate, f32 angle);
f32 func_00211774(f32 cur, f32 target, f32 rate);

s32 func_002623EC(Turret *self, AI *ai) {
    Targets t;
    Vec3 from;
    Vec3 to;
    Vec3 delta;
    f32 yaw;
    TurretParams *params;
    TargetActor *target;
    s32 fire;
    f32 pitch;
    f32 turn;
    f32 sum;
    f32 limit;

    yaw = self->rotY - ai->baseRotY;
    params = &self->def->params;
    func_00210EF0(&yaw);
    func_00246108(self, ai, &t, 0, 1);
    target = ai->target;
    fire = 0;
    if (target != NULL && t.a.dist <= params->aimRange
        && (t.a.angle < 0.0f ? -t.a.angle : t.a.angle) < params->aimFov * 0.5f) {
        if (t.a.dist <= params->fireRange
            && (t.a.angle < 0.0f ? -t.a.angle : t.a.angle) < params->fireFov * 0.5f) {
            fire = 1;
        }
        from = self->pos;
        from.y += func_002671B4(self) * 0.5f;
        to = target->pos;
        to.y += func_002671B4(target) * 0.5f;
        func_0020EF2C(&delta, &to, &from);
        pitch = func_0020F5C0(&delta);
        turn = t.a.angle;
        if (params->turnArc < 6.283186f) {
            limit = params->turnArc * 0.5f;
            sum = yaw + turn;
            if (limit < sum) {
                turn = limit - yaw;
            } else if (sum < -limit) {
                turn = -limit - yaw;
            }
        }
        func_00262150(self, ai, params->aimMode, target);
    } else {
        pitch = 0.0f;
        turn = -yaw;
        func_00262150(self, ai, params->idleMode, target);
    }
    func_00245BAC(self, ai, params->turnRate, turn);
    ai->pitch = func_00211774(ai->pitch, pitch, params->turnRate);
    return fire;
}
