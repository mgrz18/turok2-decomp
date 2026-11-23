#include "common.h"
#include "structs/game_object.h"
#include "structs/world.h"
#include "structs/game.h"
#include "structs/collision.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/collision.h"
#include "structs/particle.h"

typedef struct InstanceHit {
    /* 0x00 */ CGameObjectInstance *inst;
    /* 0x04 */ f32 t;
} InstanceHit;


s32 Sweep_HitBox(Sweep *sweep, CBox *box, f32 *t);
void Sort_Quick(void *base, s32 count, s32 size, s32 (*cmp)(), s32 (*swap)());
s32 Collision_CompareInstanceHits();
s32 Collision_SwapInstanceHits();
void Sweep_GetVolumeBounds(Sweep *sweep, SweepBox *box);
void Sweep_TestInstance(Sweep *sweep, SweepBox *box, CGameObjectInstance *other);
void Sweep_TestBox(Sweep *sweep, CGameObjectInstance *other, CVector3 *start, CBox *box);

#define MIN(a, b) ({ f32 _m = (a); if (!(_m <= (b))) _m = (b); _m; })
#define MAX(a, b) ({ f32 _m = (a); if (!((b) <= _m)) _m = (b); _m; })

/* Sweep against the actors whose bounds the swept segment's box touches,
 * nearest first, until the sweep's range runs out. */
void Collision_SweepActors(Sweep *sweep) {
    CBox box;
    SweepBox volume;
    InstanceHit hits[0x80];
    f32 t;
    CVector3 *start;
    CCollisionInfo *def;
    CGameObjectInstance *inst;
    CGameObjectInstance *other;
    CScene *world;
    InstanceHit *hit;
    GameSettings *multi;
    s32 count;
    s32 nHits;
    s32 i;
    s32 j;

    def = sweep->def;
    if (def->m_InstanceBehavior == 0 || !(def->m_dwFlags & 0x40000000)) {
        return;
    }
    inst = (CGameObjectInstance *)sweep->inst;
    start = &inst->header.m_vPos;
    box.m_vMin.x = MIN(sweep->end.x, inst->header.m_vPos.x);
    box.m_vMax.x = MAX(sweep->end.x, inst->header.m_vPos.x);
    box.m_vMin.y = MIN(sweep->end.y, start->y);
    box.m_vMax.y = MAX(sweep->end.y, start->y);
    box.m_vMin.z = MIN(sweep->end.z, start->z);
    box.m_vMax.z = MAX(sweep->end.z, start->z);
    hit = hits;
    nHits = 0;
    world = &gScene;
    count = world->m_nMovers;
    for (i = 0; i < count && nHits < 0x80; i++) {
        other = (CGameObjectInstance *)world->m_pMovers[i];
        if (inst == other) {
            continue;
        }
        if (def->m_InstanceBehavior == 7 && (CGameObjectInstance *)gCollisionResultsP->m_pInstance == other) {
            continue;
        }
        if ((def->m_dwFlags & 0x20000000) && other->m_nTypeFlag == 0x64F) {
            continue;
        }
        if (inst->header.m_Type == 2 && ((CParticle *)inst)->m_pOwner == other) {
            if (((CParticle *)inst)->m_pEffect->m_dwFlags & 0x40) {
                continue;
            }
            if (gEngineApp_m_Settings.multiplayer) {
                continue;
            }
            switch (other->header.m_pIntelligence->m_Type) {
                case 1:
                    continue;
                case 4:
                    continue;
            }
        }
        if (!(box.m_vMax.x > other->m_Bounds.m_vMin.x) || !(box.m_vMin.x < other->m_Bounds.m_vMax.x) ||
            !(box.m_vMax.z > other->m_Bounds.m_vMin.z) || !(box.m_vMin.z < other->m_Bounds.m_vMax.z) ||
            !(box.m_vMax.y > other->m_Bounds.m_vMin.y) || !(box.m_vMin.y < other->m_Bounds.m_vMax.y)) {
            continue;
        }
        if (Sweep_HitBox(sweep, &other->m_Bounds, &t)) {
            hit->inst = other;
            hit->t = t;
            hit++;
            nHits++;
        }
    }
    if (nHits != 0) {
        Sort_Quick(hits, nHits, sizeof(InstanceHit), Collision_CompareInstanceHits, Collision_SwapInstanceHits);
        hit = hits;
        for (j = 0; j < nHits; j++, hit++) {
            if (!(hit->t < sweep->hit.fraction)) {
                break;
            }
            other = hit->inst;
            if ((other->m_dwFlags & 0x300000) && !(multi = &gEngineApp_m_Settings)->multiplayer) {
                Sweep_GetVolumeBounds(sweep, &volume);
                Sweep_TestInstance(sweep, &volume, other);
            } else {
                Sweep_TestBox(sweep, other, start, &box);
            }
        }
    }
}
