#include "common.h"
#include "structs/vector.h"
#include "structs/particle.h"

extern f32 HalfToFloat(s32 half);                        /* half float -> f32 */
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 s);   /* out = in * s */
extern void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a + b */
extern f32 func_00298470(f32);                             /* sqrtf */

/* Accelerates obj->m_vVelocity along obj->m_vDirection, clamping the speed to the parent limit. */
void CParticle__Accelerate(CParticle *obj) {
    CVector3 delta;
    CVector3 dir;
    CVector3 *pdir;
    CVector3 *vel;
    f32 accel;
    f32 speed;
    f32 maxSpeed;

    accel = HalfToFloat(obj->m_pEffect->m_pMotion->m_Accel);
    if (accel != 0.0f) {
        pdir = &dir;
        *pdir = obj->m_vDirection;
        CVector3__Scale(&delta, pdir, accel);
        vel = &obj->m_vVelocity;
        CVector3__Add(vel, vel, &delta);
        speed = func_00298470(obj->m_vVelocity.x * obj->m_vVelocity.x + obj->m_vVelocity.y * obj->m_vVelocity.y + obj->m_vVelocity.z * obj->m_vVelocity.z);
        maxSpeed = HalfToFloat(obj->m_pEffect->m_pMotion->m_MaxSpeed);
        if (accel > 0.0f) {
            if (maxSpeed < speed) {
                CVector3__Scale(vel, pdir, maxSpeed);
            }
        } else {
            if (speed < maxSpeed) {
                CVector3__Scale(vel, pdir, maxSpeed);
            }
        }
    }
}
