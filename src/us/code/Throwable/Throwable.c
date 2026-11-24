/* WRONG CUT: the function starts at 0x002649A0. The three words splat puts at
 * 0x00264994 (lw $2, 0x1CE0($2); beqz; lui $3, 0x100) are stray bytes, never
 * executed: the only reference, Throwable_Setup, stores func_00264994 + 0xC. */
#include "common.h"
#include "globals/math.h"
#include "structs/game_object.h"

extern f32 GameObject_GetCollisionHeight(void *obj);
extern void CMtxF__SetTranslate(f32 *mtx, f32 x, f32 y, f32 z);
extern void CMtxF__RotateY(f32 *mtx, f32 angle);
extern void CMtxF__RotateX(f32 *mtx, f32 angle);
extern void CMtxF__Translate(f32 *mtx, f32 x, f32 y, f32 z);
extern f32 func_002982F0(f32 x); /* sinf */
extern f32 func_002974C0(f32 x); /* cosf */
extern void CQuaternion__Multiply(f32 *out, f32 *a, f32 *b);
extern void CQuaternion__ToMatrix(f32 *quat, f32 *mtx);
extern void CMtxF__MultiplyAffine(f32 *out, f32 *left, f32 *right);
extern void CMtxF__ScaleAxes(f32 *mtx, f32 x, f32 y, f32 z);
extern void CVector3__ClampLength(f32 *vec, f32 limit);
extern void CMtxF__SwapYZRows(f32 *mtx);

void Throwable_CalcOrientation(CGameObjectInstance *obj, CAI *state) {
    f32 rotMtx[16];
    f32 tiltMtx[16];
    f32 axis[4];
    f32 quat[4];
    f32 s;

    CMtxF__SetTranslate(tiltMtx, 0.0f, -GameObject_GetCollisionHeight(obj) * 0.5f, 0.0f);
    CMtxF__RotateY(tiltMtx, -state->m_Throwable.m_TiltHeading);
    CMtxF__RotateX(tiltMtx, state->m_Throwable.m_TiltAngle);
    CMtxF__RotateY(tiltMtx, state->m_Throwable.m_TiltHeading);
    CMtxF__Translate(tiltMtx, 0.0f, GameObject_GetCollisionHeight(obj) * 0.5f, 0.0f);

    s = func_002982F0(obj->m_RotY * 0.5f);
    axis[0] = 0.0f;
    axis[1] = s;
    axis[2] = 0.0f;
    gQuaternionSin = s;
    axis[3] = func_002974C0(obj->m_RotY * 0.5f);
    CQuaternion__Multiply(quat, axis, &obj->m_qGround.x);
    CQuaternion__ToMatrix(quat, rotMtx);

    CMtxF__MultiplyAffine(obj->m_mfOrient[0], tiltMtx, rotMtx);
    CMtxF__ScaleAxes(obj->m_mfOrient[0], obj->m_vScale.x, obj->m_vScale.y, obj->m_vScale.z);
    CVector3__ClampLength(&obj->header.m_vPos.x, 20000.0f);
    CMtxF__Translate(obj->m_mfOrient[0], obj->header.m_vPos.x, obj->header.m_vPos.y, obj->header.m_vPos.z);
    CMtxF__SwapYZRows(obj->m_mfOrient[0]);
}

#include "globals/ai.h"

extern void CMtxF__FromMtxAffine(f32 *out, f32 *mtx);
extern void CMtxF__TransformPoint(f32 *mtx, f32 *in, f32 *out);

/* Midpoint of two attachment points on the owner, lifted by the height. */
void Throwable_GetCarryPoint(void *obj, CAI *state, f32 *out) {
    f32 mtxA[16];
    f32 mtxB[16];
    f32 a[4];
    f32 b[4];
    JointPoint *pa = &sThrowableCarryJointA;
    JointPoint *pb = &sThrowableCarryJointB;
    CGameObjectInstance *owner = state->m_Throwable.m_pOwner;
    CGameObjectInstance *src = owner;

    if (owner->m_pmtDrawMtxs != NULL) {
        CMtxF__FromMtxAffine(mtxA, (f32 *) ((u8 *) owner->m_pmtDrawMtxs + (pa->joint << 6)));
        CMtxF__FromMtxAffine(mtxB, (f32 *) ((u8 *) owner->m_pmtDrawMtxs + (pb->joint << 6)));
    } else {
        CMtxF__SetTranslate(mtxA, src->header.m_vPos.x, src->header.m_vPos.y, src->header.m_vPos.z);
        CMtxF__SetTranslate(mtxB, src->header.m_vPos.x, src->header.m_vPos.y, src->header.m_vPos.z);
    }
    CMtxF__Translate(mtxA, 0.0f, GameObject_GetCollisionHeight(obj), 0.0f);
    CMtxF__Translate(mtxB, 0.0f, GameObject_GetCollisionHeight(obj), 0.0f);
    CMtxF__TransformPoint(mtxA, pa->offset, a);
    CMtxF__TransformPoint(mtxB, pb->offset, b);
    out[0] = (a[0] + b[0]) * 0.5f;
    out[1] = (a[1] + b[1]) * 0.5f;
    out[2] = (a[2] + b[2]) * 0.5f;
}

s32 Throwable_GetModeAnim(sptr arg0, CAI *arg1) {
    return (sptr) arg1->m_VarPtr[0]; /* the owner, pointer-wide (func_002649A0.c) */
}

void Throwable_SetModeAnim(sptr arg0, CAI *arg1, sptr arg2) {
    arg1->m_VarPtr[0] = (void *) arg2; /* the owner, pointer-wide (func_002649A0.c) */
}

s32 func_00264994(s32, s32);
#ifdef TARGET_PORT
void Throwable_CalcOrientation(CGameObjectInstance *obj, CAI *state);
#endif

void Throwable_Collision(u8 *actor, u8 *state);
void Throwable_Step(void *arg0, CAI *arg1);

void Throwable_Setup(sptr arg0, CAI *ai) {
    ai->m_pModeTable = gThrowableModeTable;
    ai->m_pfStep = Throwable_Step;
    ai->m_pfCollision = Throwable_Collision;
#ifdef TARGET_PORT
    /* func_00264994 + 0xC is Throwable_CalcOrientation (func_002649A0.c: a wrong splat cut); the
     * host address + 0xC is inside data (STABILITY.md S14: SetupAnimation called a data address). */
    ai->m_pfCalculateOrientationMatrix = Throwable_CalcOrientation;
#else
    ai->m_pfCalculateOrientationMatrix = (void *) (func_00264994 + 0xC);
#endif
}

#include "protos/ai.h"

void Throwable_Advance(sptr arg0, CAI *arg1) {
#ifdef TARGET_PORT
    /* the tilt (N64 [1], [2]); [1] is the high half of the host owner pointer (func_002649A0.c) */
    arg1->m_VarF32[2] = 0.0f;
    arg1->m_VarF32[3] = 0.0f;
#else
    arg1->m_VarBuffer[1] = 0;
    arg1->m_VarBuffer[2] = 0;
#endif
    CAI__SetMode(arg0, arg1, 0);
}

#include "structs/collision.h"
#include "globals/collision.h"

void DamageEvent_Set(DamageEvent *event, void *source, s32 damage, f32 radius, s32 flags, sptr arg5);
s32 CAI__SetMode(void *actor, void *state, s32 mode);

void Throwable_Collision(u8 *actor, u8 *state) {
    DamageEvent event;
    u8 **player;

    if (((CAI *) state)->m_Mode == 2) { /* N64 0x34 */
        player = (u8 **)&gCollisionResults.m_pInstance;
        if (*player != NULL && gCollisionResults.m_pInstance->header.m_Type == 1) {
            DamageEvent_Set(&event, actor, 0x500, 25.599998f, 0x100000, 0);
            CAI__CallHurt(*player, &((CGameObjectInstance *) *player)->m_AI, &event); /* N64 +0x140 */
            CAI__SetMode(actor, state, 0);
        }
    }
}

/* ABI (docs/port/ABI.md section 5, abicheck indirect missing_args): MIPS calls the mode callback
 * with this function's own a0/a1 (pass-through: the actor and its AI); the host passes them, and
 * the actor stays a full pointer (the callbacks take CGameObjectInstance *). */
void Throwable_Step(void *arg0, CAI *arg1) {
    s32 (*cb)(void *, CAI *);

    if (arg1->m_pModeCurrent != NULL) {
        cb = (s32 (*)(void *, CAI *))arg1->m_pModeCurrent->m_pfCallback;
        if (cb != NULL) {
            cb(arg0, arg1);
        }
    }
}

#include "m2c_macros.h"

void Throwable_IdleUpdate(void) {
    gThrowableCollisionInfo_m_InstanceBehavior[0] = 2;
}

void Throwable_PickedUpEnter(CGameObjectInstance *obj, CAI *ai) {
    ai->m_ConditionTimer = 0.0f;
    obj->m_vVelocity.x = 0.0f;
    obj->m_vVelocity.z = 0.0f;
}

#include "globals/engine.h"

extern sptr CGameObjectInstance__PickRandomAnim(void *actor, sptr action, sptr skip);
extern void CVector3__Lerp(f32 *out, f32 t, f32 *from, f32 *to);

void Throwable_PickedUpUpdate(CGameObjectInstance *obj, CAI *state) {
    f32 target[3];
    sptr anim;
    CGameObjectInstance *owner = state->m_Throwable.m_pOwner;

    gThrowableCollisionInfo_m_InstanceBehavior[0] = 0;
    if (owner == NULL) {
        return;
    }
    anim = CGameObjectInstance__PickRandomAnim(owner, 10000, -1);
    if (owner->m_CurrentAnimation.m_nAnimIndex == anim
        && owner->m_CurrentAnimation.m_nFrames * 0.3f <= owner->m_CurrentAnimation.m_AnimationFrame) {
        Throwable_GetCarryPoint(obj, state, target);
        state->m_Throwable.m_TiltHeading = owner->m_RotY - obj->m_RotY;
        state->m_ConditionTimer += gFrameIncrement * 0.25f;
        if (state->m_ConditionTimer > 1.0f) {
            state->m_ConditionTimer = 1.0f;
        }
        CVector3__Lerp(&state->m_vDesiredPos.x, state->m_ConditionTimer, &obj->header.m_vPos.x, target);
        if (owner->m_CurrentAnimation.m_AnimationFrame < owner->m_CurrentAnimation.m_nFrames * 0.5f) {
            state->m_Throwable.m_TiltAngle += 0.087266475f;
        } else {
            state->m_Throwable.m_TiltAngle -= 0.087266475f;
        }
    }
    if (owner != NULL && owner->m_AI.m_Mode != 0x15) {
        CAI__SetMode(obj, state, 2);
    }
}

#include "structs/vector.h"

extern f32 Random_Blend(f32 min, f32 max); /* random float in [min, max] */
extern f32 AI_DistSqToPointAtHeight(void *obj, CVector3 v);
extern f32 func_00298470(f32 x); /* sqrtf */

/* Launch velocity: thrown away from the owner, aimed at its target. */
void Throwable_FlyingEnter(CGameObjectInstance *obj, CAI *state) {
    CGameObjectInstance *owner = state->m_Throwable.m_pOwner;
    CGameObjectInstance *target;
    f32 speed;
    f32 lift;
    f32 r;
    f32 x;

    if (owner->m_AI.m_Mode != 0x15) {
        speed = 51.199997f;
        lift = 204.79999f;
    } else if ((target = owner->m_AI.m_pSightTarget) != NULL) {
        r = Random_Blend(1.5f, 2.5f);
        speed = r * func_00298470(AI_DistSqToPointAtHeight(obj, target->header.m_vPos));
        lift = -gThrowableCollisionInfo_m_GravityAcceleration / (r * 3.8f);
    } else {
        speed = 512.0f;
        lift = 358.4f;
    }
    lift += Random_Blend(0.0f, 2.56f);
    x = speed * func_002982F0(owner->m_RotY + 3.1415927f);
    obj->m_vVelocity.y = lift;
    obj->m_vVelocity.x = x;
    obj->m_vVelocity.z = speed * func_002974C0(owner->m_RotY + 3.1415927f);
}

extern f32 AI_FlatDistSqToPoint(void *actor, CVector3 point); /* squared XZ distance */
extern f32 GameObject_GetCollisionRadius(void *actor);              /* collision radius */
extern void EnemyAI_SetGrabbed(void *actor, void *ai, sptr arg2);

void Throwable_FlyingUpdate(CGameObjectInstance *obj, CAI *state) {
    CGameObjectInstance *owner = state->m_Throwable.m_pOwner;
    f32 distSq;

    if (owner != NULL) {
        gThrowableCollisionInfo_m_InstanceBehavior[0] = 0;
        distSq = AI_FlatDistSqToPoint(obj, owner->header.m_vPos);
        if ((GameObject_GetCollisionRadius(obj) + GameObject_GetCollisionRadius(owner)) * (GameObject_GetCollisionRadius(obj) + GameObject_GetCollisionRadius(owner)) < distSq) {
            state->m_Throwable.m_pOwner = NULL;
            EnemyAI_SetGrabbed(owner, &owner->m_AI, 0); /* the CAI at its host offset */
        }
    } else {
        gThrowableCollisionInfo_m_InstanceBehavior[0] = 2;
    }
    if (obj->m_vVelocity.x == 0.0f && obj->m_vVelocity.y == 0.0f && obj->m_vVelocity.z == 0.0f) {
        CAI__SetMode(obj, state, 0);
    }
    state->m_Throwable.m_TiltAngle += func_00298470(obj->m_vVelocity.x * obj->m_vVelocity.x + obj->m_vVelocity.y * obj->m_vVelocity.y + obj->m_vVelocity.z * obj->m_vVelocity.z)
                        * 0.00021305290f;
}
