/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/CODE_80281A70.CPP, func_80281CA0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/particle.h"
#include "structs/collision.h"
#include "globals/scene.h"
#include "globals/camera.h"

/* Steps the particle system once: marks every particle on the three active
 * lists, then (outside a pause) advances the ones inside the visible bounds,
 * fires the impact events of the expired ones and frees them, and lets each
 * collected type 0x22 particle react to the others and each type 0x57
 * particle expire the non-0x57 ones within range. */

extern f32 gFrameIncrement;          /* frame time scale */
extern f32 gParticleTimeScale;
extern s32 gEngineApp_m_Settings_paused;

extern void CParticle__Update(CParticle *p);
extern s32 CParticle__IsUnderWater(CParticle *p);
extern void ImpactEvent_Send(CParticle *p, CParticle *source, s32 type, s32 id, CVector3 pos, CEventValue value);
extern void CParticle__SpawnChild(CParticle *p, s32 type, s32 alignment, s32 count);
extern void Sound__Play(s16 sound, CVector3 pos, s32 a, s32 b);
extern void CParticleSystem__Free(CParticleSystem *world, CParticle *p);
extern void CParticle__CheckProximity(CParticleSystem *world, CParticle *p, CParticle **others, s32 count);
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);    /* out = a - b */

void CParticleSystem__Update(CParticleSystem *world) {
    CParticle *colliders[128];
    CParticle *stepped[128];
    CEventValue value;
    CVector3 delta;
    CParticle *p;
    CParticle *node;
    CParticle *focus;
    CParticle **begin;
    CROMParticleEffect *effect;
    CDLight *light;
    s32 count;
    s32 idx;
    s32 pairCount;
    s32 i;
    s32 j;
    s32 event;
    u32 particleType;
    u32 eventType;
    u32 soundType;
    f32 savedStep;

    count = 0;
    for (idx = 0; idx < 3; idx++) {
        for (node = (world->m_ActiveList + idx)->m_pHead; node != 0; node = node->m_pNext) {
            node->m_dwFlags = (node->m_dwFlags & 0xFFEFFFFF) | 0x02000000;
            stepped[count++] = node;
        }
    }
    if (gEngineApp_m_Settings_paused != 0 || gScene.m_WarpState != 0) {
        return;
    }
    savedStep = gFrameIncrement;
    gFrameIncrement = savedStep * gParticleTimeScale;
    pairCount = 0;
    for (idx = 0; idx < count; idx++) {
        p = stepped[idx];
        if ((p->m_dwFlags & 0x02000100) != 0x02000100) {
            continue;
        }
        p->m_dwFlags &= 0xFFEFFF87;
        if (p->m_pDLight) {
            p->m_pDLight->m_bEnabled = 0;
        }
        if (p->m_cFrame < (f32)p->m_nFrames && p->m_nFrames != 1 && !(p->m_dwFlags & 0x1000)) {
            if (gCameraVisibleBox.m_vMax.x > p->header.m_vPos.x && gCameraVisibleBox.m_vMax.y > p->header.m_vPos.y && gCameraVisibleBox.m_vMax.z > p->header.m_vPos.z &&
                gCameraVisibleBox.m_vMin.x <= p->header.m_vPos.x && gCameraVisibleBox.m_vMin.y <= p->header.m_vPos.y && gCameraVisibleBox.m_vMin.z <= p->header.m_vPos.z) {
                CParticle__Update(p);
                light = p->m_pDLight;
                if (light) {
                    light->m_bEnabled = 1;
                    light->x = p->header.m_vPos.x;
                    light->y = p->header.m_vPos.y;
                    light->z = p->header.m_vPos.z;
                    light->m_ColorFade = p->m_Alpha * (1.0f / 255.0f);
                }
                p->m_cFrame += gFrameIncrement;
            } else {
                CParticleSystem__Free(world, p);
            }
        } else {
            effect = p->m_pEffect;
            event = CParticle__IsUnderWater(p) ? 12 : 10;
            particleType = effect->m_pImpact->m_ImpactParticleType[event];
            eventType = effect->m_pImpact->m_ImpactEventType[event];
            value = effect->m_pImpact->m_ImpactEventValue[event];
            soundType = effect->m_pImpact->m_ImpactSoundType[event];
            if (eventType != 0xFFFF) {
                ImpactEvent_Send(p, p, eventType, -1, p->header.m_vPos, value);
            }
            if (particleType != 0xFFFF) {
                CParticle__SpawnChild(p, particleType, p->m_Alignment, 1);
            }
            if (soundType != 0xFFFF) {
                Sound__Play(soundType, p->header.m_vPos, 0, -1);
            }
            if (!(p->m_dwFlags & 0x100)) {
                continue;
            }
            CParticleSystem__Free(world, p);
        }
        if (p->m_dwFlags & 0x100) {
            switch (p->header.m_nObjType) {
            case 12:
            case 34:
            case 87:
                colliders[pairCount++] = p;
                break;
            }
        }
    }
    if (pairCount != 0) {
        begin = colliders;
        for (i = 0; i < pairCount; i++) {
            focus = begin[i];
            switch (focus->header.m_nObjType) {
            case 34:
                CParticle__CheckProximity(world, focus, begin, pairCount);
                break;
            case 87:
                for (j = 0; j < pairCount; j++) {
                    p = begin[j];
                    if (p->header.m_nObjType != 87) {
                        CVector3__Subtract(&delta, &p->header.m_vPos, &focus->header.m_vPos);
                        if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z <= 65536.0f) {
                            p->m_nFrames = 1;
                        }
                    }
                }
                break;
            case 12:
                break;
            }
        }
    }
    gFrameIncrement = savedStep;
}
