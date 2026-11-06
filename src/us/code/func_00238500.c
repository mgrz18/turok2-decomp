#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"

typedef struct ViewInfo {
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ CVector3 eye;
} ViewInfo;

typedef struct ShotActor {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ CPlayer *player;
} ShotActor;

extern u8 D_800F7078[];
extern u8 D_800F8928[];

void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);
s32 CParticleSystem__Spawn(void *table, void *actor, void *parent, void *slot, s32 a, s32 kind, CVector3 origin,
                  CQuaternion rot, CVector3 pos, s32 arg, s32 b, s32 c);
void CScene__StartRumbles(void *table, void *owner, s32 id, CVector3 pos, f32 a, f32 b);
void CPlayer__TransformByView(CPlayer *pl, CVector3 *v);
void GameObject_TraceMove(ShotActor *a, CVector3 pos, CVector3 *outPos, s32 *outArg, s32 x, s32 y);

/* Fires the player's shot of kind 0xE9 at `pos`, aimed along `dir` from the eye, and plays its sound. */
void func_00238500(void *unused, ShotActor *actor, CVector3 pos, CVector3 dir, s32 mode) {
    CVector3 v;
    CVector3 p;
    CVector3 origin;
    CQuaternion q;
    s32 arg;
    CPlayer *pl;

    pl = actor->player;
    v.x = -dir.x;
    v.y = dir.y;
    v.z = dir.z;
    CPlayer__TransformByView(pl, &v);
    v.x -= ((ViewInfo *)pl->m_pCamera)->eye.x;
    v.y -= ((ViewInfo *)pl->m_pCamera)->eye.y;
    v.z -= ((ViewInfo *)pl->m_pCamera)->eye.z;
    CQuaternion__FromDirection(&q, &v);
    GameObject_TraceMove(actor, pos, &p, &arg, 0, 1);
    p = pos;
    p.y += 12.0f;
    origin.x = 0.0f;
    origin.y = 0.0f;
    origin.z = 0.0f;
    CParticleSystem__Spawn(D_800F8928, actor, actor, NULL, 0, 0xE9, origin, q, p, arg, -1, mode == 5 ? 3 : 1);
    p.x = 0.0f;
    p.y = 0.0f;
    p.z = 0.0f;
    CScene__StartRumbles(D_800F7078, pl->m_ContState.m_pContState->rumblePool, 3, p, 10.24f, 1.0f);
}
