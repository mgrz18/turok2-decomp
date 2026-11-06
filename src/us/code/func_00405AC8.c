/* the real function is 0x00405AC8-0x00405C68 (size 0x1A0, 104 words); splat's func_00405BFC
 * (0x00405BFC, 0x6C bytes) is its tail, reached by a j from inside this function.
 * All 104 words match the ROM (checked over the real extent; D_8012FA04 as an array keeps the
 * store order that leaves li a2,0x13 in the jal delay slot). */
#include "common.h"
#include "structs/game_object.h"

extern void AI_UpdateTargets(CGameObjectInstance *obj, CAI *ai, CAITargets *t, s32 f1, s32 f2);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern f32 AI_AngleToPoint(CGameObjectInstance *obj, CVector3 point);       /* angle to a point */
extern void AI_TurnClamped(CGameObjectInstance *obj, CAI *ai, f32 turnRate, f32 angle);
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);       /* out = a - b */
extern f32 CVector3__GetYaw(CVector3 *v);                                    /* heading */
extern f32 Math_StepClamped(f32 value, f32 target, f32 rate);
extern f32 func_002982F0(f32 angle);                                      /* sin */
extern f32 func_002974C0(f32 angle);                                      /* cos */

extern s32 D_800F1F50[];
extern s32 D_8012FA04[];        /* boss script index */

void Primagen_UpdateMode27(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;
    CVector3 pos;
    CVector3 dir;
    f32 speed;
    f32 heading;
    f32 target;

    if (ai->m_AnimRepeatedCount != 0) {
        obj->m_AI.m_AnimSpeed = 1.0f;
        D_8012FA04[0] = -1;
        CAI__SetMode(obj, ai, 0x13);
    }
    D_800F1F50[0] |= 0x8000;
    AI_UpdateTargets(obj, ai, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;
    if (t.m_Sight.m_pTarget == NULL) {
        return;
    }
    pos = t.m_Sight.m_pTarget->header.m_vPos;
    speed = 0.0f;
    AI_TurnClamped(obj, ai, 0.261799425f, AI_AngleToPoint(obj, pos));
    CVector3__Subtract(&dir, &obj->header.m_vPos, &pos);
    heading = CVector3__GetYaw(&dir);
    if (obj->m_CurrentAnimation.m_AnimationFrame <= 9.0f) {
        target = t.m_Sight.m_Dist * 0.229568392f;
    } else {
        target = speed;
    }
    speed = Math_StepClamped(speed, target, 512.0f);
    obj->m_vVelocity.x += speed * func_002982F0(heading);
    obj->m_vVelocity.z += speed * func_002974C0(heading);
    obj->m_vVelocity.y = 0.001f;
}
