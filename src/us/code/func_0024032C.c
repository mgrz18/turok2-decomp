#include "common.h"
#include "structs/game_object.h"
#include "structs/world.h"
#include "structs/game.h"
#include "structs/collision.h"

/* An instance that can be hit: an actor (or the instance sweeping). */
typedef struct HitInst {
    /* 0x000 */ u8 type;
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ s32 *def;
    /* 0x018 */ u8 pad18[0xA0];
    /* 0x0B8 */ u16 typeId;
    /* 0x0BA */ u8 padBA[2];
    /* 0x0BC */ CBox bounds;
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x20];
    /* 0x0F8 */ s32 *particleDef;
    /* 0x0FC */ u8 padFC[0x10];
    /* 0x10C */ struct HitInst *owner;
} HitInst;

typedef struct InstanceHit {
    /* 0x00 */ HitInst *inst;
    /* 0x04 */ f32 t;
} InstanceHit;

extern CScene gScene;
extern CCollisionResults *gCollisionResultsP;
extern u8 gEngineApp_m_Settings_multiplayer;

s32 Sweep_HitBox(Sweep *sweep, CBox *box, f32 *t);
void Sort_Quick(void *base, s32 count, s32 size, s32 (*cmp)(), s32 (*swap)());
s32 Collision_CompareInstanceHits();
s32 Collision_SwapInstanceHits();
void Sweep_GetVolumeBounds(Sweep *sweep, SweepBox *box);
void Sweep_TestInstance(Sweep *sweep, SweepBox *box, HitInst *other);
void Sweep_TestBox(Sweep *sweep, HitInst *other, CVector3 *start, CBox *box);

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
    HitInst *inst;
    HitInst *other;
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
    inst = (HitInst *)sweep->inst;
    start = &inst->pos;
    box.m_vMin.x = MIN(sweep->end.x, inst->pos.x);
    box.m_vMax.x = MAX(sweep->end.x, inst->pos.x);
    box.m_vMin.y = MIN(sweep->end.y, start->y);
    box.m_vMax.y = MAX(sweep->end.y, start->y);
    box.m_vMin.z = MIN(sweep->end.z, start->z);
    box.m_vMax.z = MAX(sweep->end.z, start->z);
    hit = hits;
    nHits = 0;
    world = &gScene;
    count = world->m_nMovers;
    for (i = 0; i < count && nHits < 0x80; i++) {
        other = (HitInst *)world->m_pMovers[i];
        if (inst == other) {
            continue;
        }
        if (def->m_InstanceBehavior == 7 && (HitInst *)gCollisionResultsP->m_pInstance == other) {
            continue;
        }
        if ((def->m_dwFlags & 0x20000000) && other->typeId == 0x64F) {
            continue;
        }
        if (inst->type == 2 && inst->owner == other) {
            if (*inst->particleDef & 0x40) {
                continue;
            }
            if (gEngineApp_m_Settings_multiplayer) {
                continue;
            }
            switch (*other->def) {
                case 1:
                    continue;
                case 4:
                    continue;
            }
        }
        if (!(box.m_vMax.x > other->bounds.m_vMin.x) || !(box.m_vMin.x < other->bounds.m_vMax.x) ||
            !(box.m_vMax.z > other->bounds.m_vMin.z) || !(box.m_vMin.z < other->bounds.m_vMax.z) ||
            !(box.m_vMax.y > other->bounds.m_vMin.y) || !(box.m_vMin.y < other->bounds.m_vMax.y)) {
            continue;
        }
        if (Sweep_HitBox(sweep, &other->bounds, &t)) {
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
            if ((other->flags & 0x300000) && !(multi = &gEngineApp_m_Settings)->multiplayer) {
                Sweep_GetVolumeBounds(sweep, &volume);
                Sweep_TestInstance(sweep, &volume, other);
            } else {
                Sweep_TestBox(sweep, other, start, &box);
            }
        }
    }
}
