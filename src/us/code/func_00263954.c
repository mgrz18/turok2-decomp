#include "common.h"
#include "structs/vector.h"
#include "globals/scene.h"

s32 CParticleSystem__Spawn(void *table, void *actor, void *parent, void *slot, s32 a, s32 kind, CVector3 origin,
                  CQuaternion rot, CVector3 pos, s32 arg, s32 b, s32 c);

void Generator_SpawnEffect(u8 *actor, u8 *state, CVector3 pos, s32 arg, s32 kind) {
    *(s32 *) (state + 0x110) -= CParticleSystem__Spawn(gScene.m_ParticleSystem, actor, actor, state + 0x10C, 0, kind,
                                              *(CVector3 *) (actor + 0x18), *(CQuaternion *) (actor + 0x40), pos,
                                              arg, -1, 0);
}
