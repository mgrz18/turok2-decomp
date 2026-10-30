#include "common.h"
#include "structs/game_object.h"

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

/* Boss module state, viewed from 0x8012F608. */
typedef struct {
    /* 0x000 */ s32 stage;
    /* 0x004 */ u8 pad004[0xC];
    /* 0x010 */ f32 swayPhase;      /* 0x8012F618 */
    /* 0x014 */ f32 swayAmount;     /* 0x8012F61C */
    /* 0x018 */ u8 pad018[0x3B8];
    /* 0x3D0 */ s32 state;          /* 0x8012F9D8 */
    /* 0x3D4 */ s32 mode;           /* 0x8012F9DC */
    /* 0x3D8 */ u8 pad3D8[0x4];
    /* 0x3DC */ f32 timer;          /* 0x8012F9E4 */
    /* 0x3E0 */ f32 swayTimer;      /* 0x8012F9E8 */
    /* 0x3E4 */ u8 pad3E4[0x5C];
    /* 0x440 */ CVector3 point;     /* 0x8012FA48 */
} BossModule;

extern f32 func_00246310(CGameObjectInstance *obj, CVector3 point);       /* angle to a point */
extern void func_00245BAC(CGameObjectInstance *obj, CAI *ai, f32 turnRate, f32 angle);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);
extern f32 func_002982F0(f32 angle);                                      /* sin */
extern f32 func_002974C0(f32 angle);                                      /* cos */

extern s32 D_800F1C40[];
extern s32 D_800F1F50[];
extern f32 D_800B6D28;          /* per-frame scale */
extern BossModule D_8012F608;

void func_00406598(CGameObjectInstance *obj, CAI *ai) {
    CVector3 point;
    f32 angle;

    if (D_8012F608.timer > 0.0f) {
        D_8012F608.timer -= D_800B6D28;
    }
    if ((D_8012F608.mode == 3 || D_8012F608.mode == 4) && D_8012F608.timer <= 0.0f && ai->m_AnimRepeatedCount != 0) {
        obj->m_vVelocity.y = 0.0f;
        if (D_8012F608.state == 3) {
            D_800F1C40[0] |= 0x300;
            D_800F1F50[0] |= 0x300;
            func_00243414(obj, ai, 0x19);
        } else {
            func_00243414(obj, ai, 0x13);
        }
        return;
    }
    if (D_8012F608.mode == 8) {
        point = D_8012F608.point;
        angle = func_00246310(obj, point);
        if (ABS(angle) <= 0.1f) {
            obj->m_vVelocity.y += 0.0102400007f; /* 0x3C27C5AD: one ulp above 0.01024f */
            obj->m_vVelocity.x += func_002982F0(obj->m_RotY) * -24.576f;
            obj->m_vVelocity.z += func_002974C0(obj->m_RotY) * -24.576f;
            if (ai->m_AnimRepeatedCount != 0) {
                func_00243414(obj, ai, 0x17);
                return;
            }
        }
        func_00245BAC(obj, ai, 0.261799425f, angle);
    }
    if (D_8012F608.mode == 9) {
        if (D_8012F608.swayTimer <= 0.0f) {
            obj->m_vVelocity.y += D_800B6D28 * 0.001f;
            obj->m_vVelocity.x += -(D_8012F608.swayAmount * 10.24f)
                                * func_002982F0(obj->m_RotY + D_8012F608.swayPhase * 3.14159274f) * D_800B6D28;
            obj->m_vVelocity.z += -(D_8012F608.swayAmount * 10.24f)
                                * func_002974C0(obj->m_RotY + D_8012F608.swayPhase * 3.14159274f) * D_800B6D28;
        }
        if (ai->m_AnimRepeatedCount != 0) {
            D_8012F608.mode = 2;
            func_00243414(obj, ai, 0x17);
        }
    }
}
