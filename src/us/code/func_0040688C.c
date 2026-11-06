#include "common.h"
#include "structs/game_object.h"
#include "globals/engine.h"

/* Boss module state from 0x8012F9E4. */
typedef struct {
    /* 0x00 */ f32 timer;           /* 0x8012F9E4 */
    /* 0x04 */ f32 pushTimer;       /* 0x8012F9E8 */
    /* 0x08 */ u8 pad08[0x18];
    /* 0x20 */ s32 index;           /* 0x8012FA04 */
} BossTimers;

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void AI_TurnClamped(CGameObjectInstance *obj, CAI *ai, f32 turnRate, f32 angle);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);       /* out = a - b */
extern f32 CVector3__GetYaw(CVector3 *v);                                    /* heading */
extern s32 GameObject_IsOnGround(CGameObjectInstance *obj);
extern f32 func_002982F0(f32 angle);                                      /* sin */
extern f32 func_002974C0(f32 angle);                                      /* cos */

extern s32 D_800F1F50[];
extern f32 D_8012F628;          /* last angle to the target */
extern BossTimers gLevelScript_timer; /* local type: the field's real type (or the member path) changes the ROM's code here */

/* Pushes the target away from the boss while the push timer has run out. */
void Primagen_UpdateMode20(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;
    CVector3 pos;
    CVector3 d;
    CGameObjectInstance *target;
    f32 heading;
    f32 push;

    D_800F1F50[0] |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 0, 1);
    D_800F1F50[0] &= ~0x8000;
    if (t.m_Sight.m_pTarget != NULL) {
        AI_TurnClamped(obj, ai, 0.261799425f, t.m_Sight.m_Angle);
        D_8012F628 = t.m_Sight.m_Angle;
    } else {
        AI_TurnClamped(obj, ai, 0.261799425f, D_8012F628);
    }
    target = t.m_Sight.m_pTarget;
    if (target != NULL && gLevelScript_timer.pushTimer <= 0.0f) {
        pos = target->header.m_vPos;
        CVector3__Subtract(&d, &obj->header.m_vPos, &pos);
        heading = CVector3__GetYaw(&d);
        if (GameObject_IsOnGround(target)) {
            push = 716.8f;
        } else {
            push = 179.2f;
        }
        target->m_vVelocity.x += push * func_002982F0(heading);
        target->m_vVelocity.z += push * func_002974C0(heading);
        if (gLevelScript_timer.timer > 0.0f) {
            gLevelScript_timer.timer -= gFrameIncrement;
        }
    }
    if (ai->m_AnimRepeatedCount != 0 && gLevelScript_timer.timer <= 0.0f && gLevelScript_timer.pushTimer <= 0.0f) {
        gLevelScript_timer.index = -1;
        obj->m_AI.m_AnimSpeed = 1.0f;
        CAI__SetMode(obj, ai, 7);
    }
}
