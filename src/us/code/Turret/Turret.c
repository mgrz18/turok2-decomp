/* A hinged/rotating actor: swing open and shut (mode 1) or turn toward a target (mode 2). */

#include "common.h"
#include "structs/vector.h"

typedef struct SwingParams {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ f32 height;
    /* 0x5C */ f32 radius;
    /* 0x60 */ f32 heading;
    /* 0x64 */ f32 speed;
} SwingParams;
typedef struct SwingDef {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ SwingParams params;
} SwingDef;
typedef struct SwingOwner {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ SwingDef *def;
} SwingOwner;
typedef struct SwingActor {
    /* 0x000 */ u8 pad0[0x37];
    /* 0x037 */ s8 state;
    /* 0x038 */ u8 pad38[0x54 - 0x38];
    /* 0x054 */ u8 rotation[0x64 - 0x54];
    /* 0x064 */ f32 open;
    /* 0x068 */ u8 pad68[0x9C - 0x68];
    /* 0x09C */ CVector3 pos;
    /* 0x0A8 */ u8 padA8[0x110 - 0xA8];
    /* 0x110 */ f32 velocity;
} SwingActor;
typedef struct SwingTarget {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ CVector3 pos;
} SwingTarget;
extern f32 func_00298470(f32);                          /* sqrtf */
extern f32 func_002982F0(f32);                          /* sinf */
extern f32 func_002974C0(f32);                          /* cosf */
extern f32 Math_Approach(f32 cur, f32 target, f32 rate);
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
extern void CMtxF__SetRotateY(f32 mtx[4][4], f32 angle);
extern void CMtxF__TransformPoint(f32 mtx[4][4], CVector3 *in, CVector3 *out);
extern void CVector3__Lerp(void *rotation, f32 angle, CVector3 *pivot, CVector3 *point);

void Turret_UpdateHinge(SwingOwner *owner, SwingActor *actor, s32 mode, SwingTarget *target) {
    SwingParams *p;
    CVector3 hinge;
    CVector3 delta;
    CVector3 local;
    f32 mtx[4][4];
    f32 open;
    f32 maxSpeed;
    f32 speed;
    f32 desired;
    f32 radius;
    f32 velocity;

    p = &owner->def->params;
    open = actor->open;
    radius = p->radius;
    if (radius == 0.0f && p->height == 0.0f) {
        maxSpeed = 0.0f;
    } else {
        maxSpeed = p->speed / func_00298470(radius * radius + p->height * p->height);
    }

    switch (mode) {
        case 1:
            speed = maxSpeed;
            if (actor->state == mode) {
                speed = -speed;
            }
            velocity = Math_Approach(actor->velocity, speed, 0.25f);
            open += velocity;
            actor->velocity = velocity;
            if (actor->state == 0) {
                if (open > 1.0f) {
                    actor->velocity = 0.0f;
                    actor->state = mode;
                }
            } else {
                if (open < 0.0f) {
                    actor->velocity = 0.0f;
                    actor->state = 0;
                }
            }
            break;
        case 0:
            break;
        case 2:
            if (target != NULL) {
                if (p->heading == 0.0f) {
                    desired = 0.0f;
                } else {
                    CVector3__Subtract(&delta, &target->pos, &actor->pos);
                    CMtxF__SetRotateY(mtx, -p->heading);
                    CMtxF__TransformPoint(mtx, &delta, &local);
                    desired = local.z / p->radius;
                }
                speed = desired - open;
                if (speed > maxSpeed) {
                    speed = maxSpeed;
                } else if (speed < -maxSpeed) {
                    speed = -maxSpeed;
                }
                velocity = Math_Approach(actor->velocity, speed, 0.25f);
                open += velocity;
                actor->velocity = velocity;
            }
            break;
    }

    if (open > 1.0f) {
        open = 1.0f;
    } else if (open < 0.0f) {
        open = 0.0f;
    }
    actor->open = open;

    hinge = actor->pos;
    hinge.x += p->radius * func_002982F0(p->heading);
    hinge.y += p->height;
    hinge.z += p->radius * func_002974C0(p->heading);
    CVector3__Lerp(actor->rotation, open, &actor->pos, &hinge);
}

#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "protos/ai.h"

void Angle_Wrap(f32 *angle);
void AI_UpdateTargets(CGameObjectInstance *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
f32 GameObject_GetCollisionHeight(void *actor);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
f32 CVector3__GetPitch(CVector3 *v);
f32 Math_Approach(f32 cur, f32 target, f32 rate);

s32 Turret_Aim(CGameObjectInstance *self, CAI *ai) {
    CAITargets t;
    CVector3 from;
    CVector3 to;
    CVector3 delta;
    f32 yaw;
    CTurretIntelligence *params;
    CGameObjectInstance *target;
    s32 fire;
    f32 pitch;
    f32 turn;
    f32 sum;
    f32 limit;

    yaw = self->m_RotY - ai->m_InitialRotY;
    params = &self->header.m_pIntelligence->m_Turret;
    Angle_Wrap(&yaw);
    AI_UpdateTargets(self, ai, &t, 0, 1);
    target = ai->m_pSightTarget;
    fire = 0;
    if (target != NULL && t.m_Sight.m_FlatDist <= params->m_AimRange
        && (t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle : t.m_Sight.m_Angle) < params->m_AimFov * 0.5f) {
        if (t.m_Sight.m_FlatDist <= params->m_FireRange
            && (t.m_Sight.m_Angle < 0.0f ? -t.m_Sight.m_Angle : t.m_Sight.m_Angle) < params->m_FireFov * 0.5f) {
            fire = 1;
        }
        from = self->header.m_vPos;
        from.y += GameObject_GetCollisionHeight(self) * 0.5f;
        to = target->header.m_vPos;
        to.y += GameObject_GetCollisionHeight(target) * 0.5f;
        CVector3__Subtract(&delta, &to, &from);
        pitch = CVector3__GetPitch(&delta);
        turn = t.m_Sight.m_Angle;
        if (params->m_TurnArc < 6.283186f) {
            limit = params->m_TurnArc * 0.5f;
            sum = yaw + turn;
            if (limit < sum) {
                turn = limit - yaw;
            } else if (sum < -limit) {
                turn = -limit - yaw;
            }
        }
        Turret_UpdateHinge(self, ai, params->m_AimMode, target);
    } else {
        pitch = 0.0f;
        turn = -yaw;
        Turret_UpdateHinge(self, ai, params->m_IdleMode, target);
    }
    AI_TurnClamped(self, ai, params->m_RotSpeed, turn);
    ai->m_Turret.m_Pitch = Math_Approach(ai->m_Turret.m_Pitch, pitch, params->m_RotSpeed);
    return fire;
}

#include "globals/ai.h"

typedef void (*AICallback)(void *actor, void *ai);
void Turret_Step(void *arg0, CAI *arg1);
void Turret_CalcOrientation(CGameObjectInstance *self, CAI *ai);
void Turret_Effects(); /* defined with the host or N64 view of its argument (func_00262B58.c) */
#include "globals/scene.h"
void Turret_Hurt(sptr arg0, CAI *arg1, DamageEvent *arg2);

/* AI init: installs this actor kind's table and callbacks. */
void Turret_Setup(void *actor, CAI *ai) {
    /* FAKEMATCH: the ROM builds Turret_Effects's address in $a0 (the dead
     * actor argument) while $v0 still holds Turret_CalcOrientation's; no natural
     * source shape tried (temporaries, inline setters, reusing the
     * parameter, store order, permuter) gives a second register other
     * than $v1, so the value is pinned to $a0. */
    register AICallback unk108 asm("$4");

    ai->m_pModeTable = gTurretModeTable;
    ai->m_pfStep = Turret_Step;
    ai->m_pfHurt = Turret_Hurt;
    ai->m_pfCalculateOrientationMatrix = Turret_CalcOrientation;
    unk108 = Turret_Effects;
    ai->m_pfEffects = unk108;
}

#include "protos/scene.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"

/* The port: the same on CGameObjectInstance/CAI (the N64 code reads both at raw offsets:
 * def 0x14, flags 0xD4; the AI's m_VarS32[0..1] 0x10C/0x110, m_ConditionTimer 0x64,
 * m_Action 0x37). The intelligence is asset data (the same bytes on the host): its words at
 * 0x14 (the type flags) and 0x7C stay byte offsets. */
void Turret_Advance(u8 *actor_, u8 *anim_) {
    CGameObjectInstance *actor = (CGameObjectInstance *) actor_;
    CAI *anim = (CAI *) anim_;
    u8 *def;

    def = (u8 *) actor->header.m_pIntelligence;
    anim->m_VarS32[0] = 0;
    anim->m_VarS32[1] = 0;
    anim->m_ConditionTimer = *(f32 *) (def + 0x7C);
    if (CScene__GetInstanceSavedBit(&gScene, actor) == 0) {
        if (*(s32 *) (def + 0x14) & 0x1000) {
            actor->m_dwFlags &= ~0x2000;
            actor->m_dwFlags &= ~0x100;
        }
        if (*(s32 *) (def + 0x14) & 0x800) {
            actor->m_dwFlags &= ~0x100;
            CAI__SetMode(actor, anim, 0);
        } else {
            CAI__SetMode(actor, anim, 1);
        }
    } else {
        CAI__SetMode(actor, anim, 0x40);
    }
    anim->m_Action = 0;
}
#else
void Turret_Advance(u8 *actor, u8 *anim) {
    u8 *def;

    def = *(u8 **) (actor + 0x14);
    *(s32 *) (anim + 0x10C) = 0;
    *(s32 *) (anim + 0x110) = 0;
    *(f32 *) (anim + 0x64) = *(f32 *) (def + 0x7C);
    if (CScene__GetInstanceSavedBit(&gScene, actor) == 0) {
        if (*(s32 *) (def + 0x14) & 0x1000) {
            *(s32 *) (actor + 0xD4) &= ~0x2000;
            *(s32 *) (actor + 0xD4) &= ~0x100;
        }
        if (*(s32 *) (def + 0x14) & 0x800) {
            *(s32 *) (actor + 0xD4) &= ~0x100;
            CAI__SetMode(actor, anim, 0);
        } else {
            CAI__SetMode(actor, anim, 1);
        }
    } else {
        CAI__SetMode(actor, anim, 0x40);
    }
    anim[0x37] = 0;
}
#endif

/* ABI (docs/port/ABI.md section 5, abicheck indirect missing_args): MIPS calls the mode callback
 * with this function's own a0/a1 (pass-through: the actor and its AI); the host passes them, and
 * the actor stays a full pointer (the callbacks take CGameObjectInstance *). */
void Turret_Step(void *arg0, CAI *arg1) {
    s32 (*cb)(void *, CAI *);

    if (arg1->m_pModeCurrent != NULL) {
        cb = (s32 (*)(void *, CAI *))arg1->m_pModeCurrent->m_pfCallback;
        if (cb != NULL) {
            cb(arg0, arg1);
        }
    }
}

s32 Turret_GetIdleAnim(CGameObjectInstance *arg0) {
    s32 v = arg0->header.m_pIntelligence->m_Enemy.m_StartAnim;

    if (v != 0) {
        return v;
    }
    return 0x2F44;
}

void Turret_ActivateUpdate(sptr arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 0x3E);
    }
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS Turret_Aim receives Turret_AimUpdate's own argument
 * registers (pass-through); the host passes them. */
void Turret_AimUpdate(sptr arg0, sptr arg1) {
    if (Turret_Aim(arg0, arg1) != 0) {
        CAI__SetMode(arg0, arg1, 0x3F);
    }
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS Turret_Aim receives Turret_FireUpdate's own argument
 * registers (pass-through); the host passes them. */
void Turret_FireUpdate(sptr arg0, sptr arg1) {
    if (Turret_Aim(arg0, arg1) == 0) {
        CAI__SetMode(arg0, arg1, 0x3E);
    }
}

#include "protos/game.h"
#include "structs/word.h"

typedef union B01FH { f32 f; s32 i; s16 h; } B01FH;
typedef struct B01Info {
                u32 flags;
                s32 unk4;
                s16 unk8;
                s16 unkA;
                CWord unkC;
                B01FH unk10;
                s16 unk14;
                s16 unk16;
                s16 unk18;
                u8 unk1A;
                u8 unk1B;
                u8 pad1C[0x8];
                u32 flags24;
                u8 pad28[0x4];
                s32 unk2C;
                s32 unk30;
                CWord unk34;
                s32 unk38;
                s32 unk3C;
                B01FH unk40;
                u8 pad44[0x4];
                f32 unk48;
                f32 unk4C;
                u8 pad50[0x1C];
                f32 unk6C;
} B01Info;
void AI_DropPickupsOnce(CGameObjectInstance *actor, CAI *state, s32 kind, s32 a, s32 b);

void Turret_DeathEnter(CGameObjectInstance *actor, CAI *state) {
    B01Info *info = ((B01Info *)((u8 *)(actor->header.m_pIntelligence) + 0x14));
    AI_DropPickupsOnce(actor, state, 4, info->unkC.i, info->unk10.i);
    state->m_pfHurt = 0;
    CScene__SetInstanceSavedBit(&gScene, actor, 1);
    GameObject_FireTriggers(actor, 1, actor);
    actor->m_dwFlags &= ~0x10000;
    if (gScene.m_Mode != 4) {
        actor->m_dwFlags |= 0x8000000;
    }
}

/* An event handler: (instance, AI, event value), the last two unused. */
void Turret_EventShow(CGameObjectInstance *arg0, CAI *ai, s32 arg2) {
    if (CScene__GetInstanceSavedBit(&gScene, arg0) == 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    }
}

void Turret_EventActivate(sptr arg0, CAI *ai, sptr arg2) {
    if ((CScene__GetInstanceSavedBit(&gScene, arg0) == 0) && (ai->m_Mode == 0)) {
        Turret_EventShow((CGameObjectInstance *) arg0, ai, arg2);
        CAI__SetMode(arg0, ai, 1);
    }
}

#include "globals/math.h"

f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
void CQuaternion__Multiply(CQuaternion *out, CQuaternion *a, CQuaternion *b);
void CQuaternion__ToMatrix(CQuaternion *q, f32 mtx[4][4]);
void CMtxF__ScaleAxes(f32 mtx[4][4], f32 x, f32 y, f32 z);
void CVector3__ClampLength(CVector3 *v, f32 limit);
void CMtxF__Translate(f32 mtx[4][4], f32 x, f32 y, f32 z);
void CMtxF__SwapYZRows(f32 mtx[4][4]);

void Turret_CalcOrientation(CGameObjectInstance *self, CAI *ai) {
    CQuaternion q;
    CQuaternion rot;
    f32 sinHalf;

    if (self->header.m_pIntelligence->m_Turret.m_RotNode == -1) {
        sinHalf = func_002982F0(self->m_RotY * 0.5f);
        q.x = 0.0f;
        q.y = sinHalf;
        q.z = 0.0f;
        gQuaternionSin = sinHalf;
        q.w = func_002974C0(self->m_RotY * 0.5f);
    } else {
        gQuaternionSin = func_002982F0(ai->m_InitialRotY * 0.5f);
    }
    q.x = 0.0f;
    q.y = gQuaternionSin;
    q.z = 0.0f;
    q.w = func_002974C0(ai->m_InitialRotY * 0.5f);
    CQuaternion__Multiply(&rot, &q, &self->m_qGround);
    CQuaternion__ToMatrix(&rot, self->m_mfOrient);
    CMtxF__ScaleAxes(self->m_mfOrient, self->m_vScale.x, self->m_vScale.y, self->m_vScale.z);
    CVector3__ClampLength(&self->header.m_vPos, 20000.0f);
    CMtxF__Translate(self->m_mfOrient, self->header.m_vPos.x, self->header.m_vPos.y, self->header.m_vPos.z);
    CMtxF__SwapYZRows(self->m_mfOrient);
}

#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define CMtxF__RotateX CMtxF__RotateX_i
#endif

s32 CMtxF__RotateX(s32, s32);                    /* extern */
s32 CMtxF__RotateZ(sptr, f32);                    /* extern */
/* Effect callback context: the actor it runs for and the current value. */
typedef struct {
    /* 0x00 */ CGameObjectInstance *owner;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ s32 value;
} Effect00262B58;
#ifdef TARGET_PORT
#include "structs/pose_host.h"
/* Host body (round 13 b4): CGameObjectInstance__PoseBone passes the pose context at its
 * HOST layout (structs/pose_host.h: obj, bone) and the matrix is a host static
 * (sPoseMtx): the s32 CMtxF__RotateX_i shim truncated it (macOS crash in level 124, the
 * first turret of the whole-game run). The float CMtxF__RotateX directly, the matrix
 * pointer-wide; m_VarBuffer[0] holds the gun pitch as float bits. */
#undef CMtxF__RotateX
void CMtxF__RotateX_f(void *m, f32 a) __asm__("CMtxF__RotateX"); /* the float definition */
void Turret_Effects(sptr arg0, T2PoseContext *ctx) {
    CGameObjectInstance *obj = ctx->obj;
    CTurretIntelligence *turret = &obj->header.m_pIntelligence->m_Turret;
    union {
        u32 u;
        f32 f;
    } pitch;

    if (ctx->bone == turret->m_GunNode) {
        pitch.u = obj->m_AI.m_VarBuffer[0];
        CMtxF__RotateX_f((void *) arg0, pitch.f);
    }
    if (ctx->bone == turret->m_RotNode) {
        CMtxF__RotateZ(arg0, obj->m_AI.m_InitialRotY - obj->m_RotY);
    }
}
#else
void Turret_Effects(sptr arg0, Effect00262B58 *arg1) {
    CGameObjectInstance *temp_a0;
    CTurretIntelligence *temp_s1;
    CGameObjectInstance *temp_v0;

    temp_a0 = arg1->owner;
    temp_s1 = &temp_a0->header.m_pIntelligence->m_Turret;
    if (arg1->value == temp_s1->m_GunNode) {
        CMtxF__RotateX(arg0, temp_a0->m_AI.m_VarBuffer[0]);
    }
    if (arg1->value == temp_s1->m_RotNode) {
        temp_v0 = arg1->owner;
        CMtxF__RotateZ(arg0, temp_v0->m_AI.m_InitialRotY - temp_v0->m_RotY);
    }
}
#endif
#include "structs/collision.h"

void Turret_Hurt(sptr arg0, CAI *arg1, DamageEvent *arg2) { /* sptr: the actor (soak S11, --high-memory) */
    s32 var_v1;

    var_v1 = arg1->m_Health - arg2->damage;
    if (var_v1 < 0) {
        var_v1 = 0;
    }
    arg1->m_Health = var_v1;
    if (var_v1 == 0) {
        CAI__SetMode(arg0, arg1, 0x40);
    }
}
