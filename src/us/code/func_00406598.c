#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/engine.h"

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

extern f32 AI_AngleToPoint(CGameObjectInstance *obj, CVector3 point);       /* angle to a point */
extern void AI_TurnClamped(CGameObjectInstance *obj, CAI *ai, f32 turnRate, f32 angle);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern f32 func_002982F0(f32 angle);                                      /* sin */
extern f32 func_002974C0(f32 angle);                                      /* cos */

extern s32 D_800F1C40[];
extern s32 D_800F1F50[];

void Primagen_UpdateMode18(CGameObjectInstance *obj, CAI *ai) {
    CVector3 point;
    f32 angle;

    if (gLevelScript.timer > 0.0f) {
        gLevelScript.timer -= gFrameIncrement;
    }
    if ((gLevelScript.bossMode == 3 || gLevelScript.bossMode == 4) && gLevelScript.timer <= 0.0f && ai->m_AnimRepeatedCount != 0) {
        obj->m_vVelocity.y = 0.0f;
        if (gLevelScript.bossState == 3) {
            D_800F1C40[0] |= 0x300;
            D_800F1F50[0] |= 0x300;
            CAI__SetMode(obj, ai, 0x19);
        } else {
            CAI__SetMode(obj, ai, 0x13);
        }
        return;
    }
    if (gLevelScript.bossMode == 8) {
        point = gLevelScript.point;
        angle = AI_AngleToPoint(obj, point);
        if (ABS(angle) <= 0.1f) {
            obj->m_vVelocity.y += 0.0102400007f; /* 0x3C27C5AD: one ulp above 0.01024f */
            obj->m_vVelocity.x += func_002982F0(obj->m_RotY) * -24.576f;
            obj->m_vVelocity.z += func_002974C0(obj->m_RotY) * -24.576f;
            if (ai->m_AnimRepeatedCount != 0) {
                CAI__SetMode(obj, ai, 0x17);
                return;
            }
        }
        AI_TurnClamped(obj, ai, 0.261799425f, angle);
    }
    if (gLevelScript.bossMode == 9) {
        if (gLevelScript.swayTimer <= 0.0f) {
            obj->m_vVelocity.y += gFrameIncrement * 0.001f;
            obj->m_vVelocity.x += -(gLevelScript.swayAmount * 10.24f)
                                * func_002982F0(obj->m_RotY + gLevelScript.swayPhase * 3.14159274f) * gFrameIncrement;
            obj->m_vVelocity.z += -(gLevelScript.swayAmount * 10.24f)
                                * func_002974C0(obj->m_RotY + gLevelScript.swayPhase * 3.14159274f) * gFrameIncrement;
        }
        if (ai->m_AnimRepeatedCount != 0) {
            gLevelScript.bossMode = 2;
            CAI__SetMode(obj, ai, 0x17);
        }
    }
}
