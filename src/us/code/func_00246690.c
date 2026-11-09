#include "common.h"
#include "structs/vector.h"
#include "globals/scene.h"
#include "protos/game.h"

typedef struct SpawnObj {
    /* 0x000 */ u8 type;            /* 0 = level actor, 1 = game object, 2 = ..., 3 = particle */
    /* 0x001 */ u8 pad1[0x18 - 1];
    /* 0x018 */ CVector3 velocity;
    /* 0x024 */ u8 pad24[0x1C0 - 0x24];
    /* 0x1C0 */ s32 target;
} SpawnObj;


void GameObject_TraceMove(SpawnObj *obj, CVector3 pos, CVector3 *outPos, s32 *outRegion, s32 x, s32 y);
CQuaternion CGameObjectInstance__GetRotation(SpawnObj *obj);
CQuaternion func_002156D4(SpawnObj *obj);
s32 func_0021F1F8(SpawnObj *obj);

/* Spawns a particle of `kind` from `obj` at `at`, moving with it and
 * oriented like it. */
void Actor_SpawnParticle(SpawnObj *obj, s32 kind, CVector3 at) {
    CVector3 pos;
    CVector3 vel;
    CQuaternion rot;
    sptr region;
    s32 mode;
    s32 target;

    mode = 1;
    GameObject_TraceMove(obj, at, &pos, &region, 0, 1);
    target = 0;
    switch (obj->type) {
    case 0:
    case 2:
        vel.x = vel.y = vel.z = 0.0f;
        rot.x = rot.y = rot.z = 0.0f;
        rot.w = 1.0f;
        break;
    case 1:
        vel = obj->velocity;
        rot = CGameObjectInstance__GetRotation(obj);
        target = obj->target;
        if (func_0021F1F8(obj) == 5) {
            mode = 3;
        }
        break;
    case 3:
        vel = obj->velocity;
        rot = func_002156D4(obj);
        break;
    }
    CParticleSystem__Spawn(&gScene.m_ParticleSystem, obj, obj, NULL, target, kind, vel, rot, pos, region, -1, mode);
}
