#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

extern f32 AI_AngleToPoint(CGameObjectInstance *obj, CVector3 point);       /* angle to a point */
extern void AI_TurnClamped(CGameObjectInstance *obj, CAI *ai, f32 turnRate, f32 angle);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern f32 func_002982F0(f32 angle);                                      /* sin */
extern f32 func_002974C0(f32 angle);                                      /* cos */

extern s32 D_800F1C40[];
extern s32 D_800F1F50[];
extern f32 D_800B6D28;          /* per-frame scale */

void Primagen_UpdateMode18(CGameObjectInstance *obj, CAI *ai) {
    CVector3 point;
    f32 angle;

    if (D_8012F608.timer > 0.0f) {
        D_8012F608.timer -= D_800B6D28;
    }
    if ((D_8012F608.bossMode == 3 || D_8012F608.bossMode == 4) && D_8012F608.timer <= 0.0f && ai->m_AnimRepeatedCount != 0) {
        obj->m_vVelocity.y = 0.0f;
        if (D_8012F608.bossState == 3) {
            D_800F1C40[0] |= 0x300;
            D_800F1F50[0] |= 0x300;
            CAI__SetMode(obj, ai, 0x19);
        } else {
            CAI__SetMode(obj, ai, 0x13);
        }
        return;
    }
    if (D_8012F608.bossMode == 8) {
        point = D_8012F608.point;
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
    if (D_8012F608.bossMode == 9) {
        if (D_8012F608.swayTimer <= 0.0f) {
            obj->m_vVelocity.y += D_800B6D28 * 0.001f;
            obj->m_vVelocity.x += -(D_8012F608.swayAmount * 10.24f)
                                * func_002982F0(obj->m_RotY + D_8012F608.swayPhase * 3.14159274f) * D_800B6D28;
            obj->m_vVelocity.z += -(D_8012F608.swayAmount * 10.24f)
                                * func_002974C0(obj->m_RotY + D_8012F608.swayPhase * 3.14159274f) * D_800B6D28;
        }
        if (ai->m_AnimRepeatedCount != 0) {
            D_8012F608.bossMode = 2;
            CAI__SetMode(obj, ai, 0x17);
        }
    }
}
