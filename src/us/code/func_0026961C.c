#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"

typedef struct CollisionInfo {
    /* 0x00 */ s32 flags;
} CollisionInfo;

extern CollisionInfo D_800F1B00;
extern CollisionInfo D_800F1B60;
extern CollisionInfo D_800F1B80;
extern CollisionInfo D_800F1BE0;

s32 func_00267EE8(CGameObjectInstance *actor);
s32 func_00267F64(CGameObjectInstance *actor);
s32 func_00267FA8(CGameObjectInstance *actor);
s32 func_00239F00(CGameObjectInstance *actor, CVector3 pos, CollisionInfo *info);

/* Tests moving an actor to pos with the collision settings that suit it, leaving it where it was; returns the collision result. */
s32 func_0026961C(CGameObjectInstance *actor, CVector3 pos, CollisionInfo **outInfo) {
    CObjectHead saved;
    CollisionInfo *info;
    s32 alt;
    s32 hit;

    if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x300000)) {
        info = &D_800F1BE0;
    } else {
        alt = func_00267EE8(actor);
        if (!func_00267F64(actor)) {
            info = alt ? &D_800F1B60 : &D_800F1BE0;
        } else {
            info = !func_00267FA8(actor) ? &D_800F1B80 : &D_800F1B00;
        }
    }
    if (outInfo != NULL) {
        *outInfo = info;
    }
    saved = *(CObjectHead *) actor;
    hit = func_00239F00(actor, pos, info);
    *(CObjectHead *) actor = saved;
    return hit;
}
