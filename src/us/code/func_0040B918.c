#include "common.h"
#include "structs/game_object.h"

/* AI state: turn toward the point (0, 0, 512) and go to anim state 0x1C once within 51.2 of it. */

extern f32 AI_AngleToPoint(CGameObjectInstance *obj, CVector3 target);
extern void AI_TurnClamped(CGameObjectInstance *obj, void *ai, f32 angle, f32 turn);
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a - b */
extern f32 func_00298470(f32 x); /* sqrt */
extern void CAI__SetMode(CGameObjectInstance *obj, void *ai, s32 state);

void MotherBoss_Mode24_GoToCenter(CGameObjectInstance *obj, void *ai) {
    CVector3 d;

    d.x = 0.0f;
    d.y = 0.0f;
    d.z = 512.0f;
    AI_TurnClamped(obj, ai, 0.261799425f, AI_AngleToPoint(obj, d));
    CVector3__Subtract(&d, &d, &obj->header.m_vPos);
    if (func_00298470(d.x * d.x + d.y * d.y + d.z * d.z) < 51.199997f) {
        CAI__SetMode(obj, ai, 0x1C);
    }
}
