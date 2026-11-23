#include "common.h"
#include "structs/vector.h"
#include "globals/engine.h"
#include "structs/particle.h"

/* Particle wobble: swings the velocity sideways (and then up/down) along
 * sine waves whose amplitude ramps up over the template's first m_Ramp frames
 * of the particle's age. */

extern f32 func_002982F0(f32);                              /* sinf */
extern void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a x b */
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 s);    /* out = in * s */
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a - b */
extern void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a + b */

void CParticle__Wobble(CParticle *p, f32 scale) {
    CVector3 up;
    CVector3 axis;
    CVector3 dir;
    CVector3 before;
    CVector3 after;
    CVector3 side;
    f32 ramp;
    f32 k;

    k = 1.0f;
    ramp = p->m_pEffect->m_pOscillation->m_Ramp;
    if (p->m_cFrame < ramp) {
        k = p->m_cFrame / ramp;
    }
    k *= scale;
    dir = p->m_vDirection;
    if (p->m_WobbleAmplitude[0] > 0.0f) {
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        CVector3__Cross(&axis, &up, &dir);
        CVector3__Scale(&before, &axis, func_002982F0(p->m_WobblePhase[0]) * p->m_WobbleAmplitude[0] * k);
        p->m_WobblePhase[0] += p->m_WobbleFreq[0] * k * gFrameIncrement;
        CVector3__Scale(&after, &axis, func_002982F0(p->m_WobblePhase[0]) * p->m_WobbleAmplitude[0] * k);
        CVector3__Subtract(&p->m_vVelocity, &p->m_vVelocity, &before);
        CVector3__Add(&p->m_vVelocity, &p->m_vVelocity, &after);
    } else {
        p->m_WobblePhase[0] += p->m_WobbleFreq[0] * k * gFrameIncrement;
    }
    if (p->m_WobbleAmplitude[1] > 0.0f) {
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        CVector3__Cross(&side, &up, &dir);
        CVector3__Cross(&axis, &side, &dir);
        CVector3__Scale(&before, &axis, func_002982F0(p->m_WobblePhase[1]) * p->m_WobbleAmplitude[1] * k);
        p->m_WobblePhase[1] += p->m_WobbleFreq[1] * k * gFrameIncrement;
        CVector3__Scale(&after, &axis, func_002982F0(p->m_WobblePhase[1]) * p->m_WobbleAmplitude[1] * k);
        CVector3__Subtract(&p->m_vVelocity, &p->m_vVelocity, &before);
        CVector3__Add(&p->m_vVelocity, &p->m_vVelocity, &after);
    } else {
        p->m_WobblePhase[1] += p->m_WobbleFreq[1] * k * gFrameIncrement;
    }
}
