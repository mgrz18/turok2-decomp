/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002149C0.c, func_002149C0, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/intelligence.h"
#include "structs/game.h"
#include "structs/collision.h"

typedef struct Actor {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ s32 region;
    /* 0x014 */ CIntelligence *def;
    /* 0x018 */ u8 pad18[0x54 - 0x18];
    /* 0x054 */ f32 eye;
    /* 0x058 */ u8 pad58[0xD4 - 0x58];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x1A8 - 0xD8];
    /* 0x1A8 */ void *owner;
    /* 0x1AC */ u8 pad1AC[0x52A - 0x1AC];
    /* 0x52A */ s16 alive;          /* players */
    /* 0x52C */ u8 pad52C[0xA08 - 0x52C];
    /* 0xA08 */ s16 character;      /* players */
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ f32 height;
} CameraInfo;

typedef struct {
    /* 0x00 */ u32 *flags;
    /* 0x04 */ u8 pad4[0x18 - 4];
} CharacterInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x37];
    /* 0x37 */ s8 unk37;
    /* 0x38 */ u8 pad38[0x68 - 0x38];
    /* 0x68 */ Actor *ignore;
    /* 0x6C */ u8 pad6C[0x78 - 0x6C];
    /* 0x78 */ Actor *forced;
    /* 0x7C */ u8 pad7C[4];
    /* 0x80 */ Actor *approachFrom;
    /* 0x84 */ u8 pad84[0xAC - 0x84];
    /* 0xAC */ CVector3 pos;
    /* 0xB8 */ s32 region;
} Plan;

extern u8 gScene[];
extern u8 gCollisionInfoLineOfSight[];
extern CCollisionResults gCollisionResults;  /* the last collision test (m_pInstance: what it hit) */
extern CharacterInfo gPlayerStateTable_tuning[];

s32 AI_UpdatePathHeading(Actor *self, Plan *plan, Actor *target);
s32 AI_IsInViewCone(Actor *viewer, Actor *target, f32 range, f32 across, f32 up);
s32 EnemyAI_GetCurrentBehavior(Actor *self, Plan *plan);
s32 EnemyAI_GetBehaviorIndex(Actor *self, Plan *plan);
void AI_HearNoise(Actor *self, Plan *plan, CVector3 *pos, s32 arg3);
s32 Cinema_PathPlaying(void);
s32 Cinema_GetScriptId(void);
Actor *func_00225848(u8 *scene, Actor *self);
Actor *func_0022594C(u8 *scene, Actor *self);
Actor *func_002259D0(u8 *scene, Actor *self);
Actor *CPlayerPool__FindNearest(CPlayerPool *players, CVector3 pos);
Actor *CPlayerPool__FindNearestOther(CPlayerPool *players, CVector3 pos, void *owner);
CameraInfo *GameObject_GetRegionInfo(Actor *player);
f32 AI_DistSqToPointAtHeight(Actor *a, CVector3 pos);
f32 AI_FlatDistSqToPoint(Actor *a, CVector3 pos);
s32 func_00267E74(Actor *a);
f32 GameObject_GetLeashRadius(Actor *a);
f32 GameObject_GetCollisionHeight(Actor *a);                           /* height */
s32 Collision_MoveInstance(Actor *a, CVector3 from, u8 *result);

#define ADD(t) if ((t) != NULL) { list[n] = (t); n++; }

/* The plan's best target: the nearest of its candidates (sides' leaders,
 * the actor it ignores, a nearby creature, the nearest player) that it may
 * attack and, with `checkSight`, can see. */
Actor *AI_FindBestTarget(Actor *self, Plan *plan, s32 checkSight, s32 flag) {
    Actor *list[128];
    CObjectHead saved;
    CVector3 eye;
    f32 range;
    f32 across;
    f32 up;
    f32 best;
    f32 d;
    f32 limit;
    f32 dist;
    Actor *bestA;
    Actor *t;
    CameraInfo *cam;
    s32 n;
    s32 i;
    s32 ok;
    s32 side;
    s32 r;

    if (plan->forced != NULL) {
        if (flag) {
            AI_UpdatePathHeading(self, plan, plan->forced);
        }
        return plan->forced;
    }
    if (self->def->m_Type == 1 && EnemyAI_GetCurrentBehavior(self, plan) != 2) {
        range = 2048.0f;
        across = 2.4434612f;
        up = 0.785398245f;
    } else {
        range = 2048.0f;
        across = 2.4434612f;
        up = 1.832596f;
    }
    n = 0;
    bestA = NULL;
    ok = 1;
    if (Cinema_PathPlaying()) {
        ok = 0;
        if (Cinema_GetScriptId() >= 10) {
            ok = Cinema_GetScriptId() < 61;
        }
    }
    if (ok) {
        if (flag) {
            if (self->def->m_Type == 1 && plan->approachFrom != NULL) {
                side = EnemyAI_GetBehaviorIndex(self, plan);
                if ((side == 0 && (self->def->m_Enemy.m_dwEnemyFlags & 0x40))
                    || (side == 1 && (self->def->m_Enemy.m_dwEnemyFlags & 0x80))
                    || (side == 2 && (self->def->m_Enemy.m_dwEnemyFlags & 0x100))) {
                    t = func_00225848(gScene, self);
                    ADD(t);
                }
                if (plan->approachFrom != NULL
                    && ((side == 0 && (self->def->m_Enemy.m_wTakeCoverAnimFlags & 0x1F))
                        || (side == 1 && (self->def->m_Enemy.m_wTakeCoverAnimFlags & 0x3E0))
                        || (side == 2 && (self->def->m_Enemy.m_wTakeCoverAnimFlags & 0x7C00)))) {
                    t = func_0022594C(gScene, self);
                    ADD(t);
                }
            }
            t = plan->ignore;
            ADD(t);
            t = func_002259D0(gScene, self);
            ADD(t);
        }
        if (self->flags & 0x300000) {
            t = CPlayerPool__FindNearestOther(&gEngineApp_m_Game.m_PlayerList, self->pos, self->owner);
        } else {
            t = CPlayerPool__FindNearest(&gEngineApp_m_Game.m_PlayerList, self->pos);
        }
        if (t != NULL && t->alive != 0 && t->character != 1) {
            cam = GameObject_GetRegionInfo(t);
            if (cam != NULL) {
                range = cam->height - 102.399994f;
            }
            if (self->def->m_Type == 1 && plan->unk37 == 0 && (*gPlayerStateTable_tuning[t->character].flags & 0x80)) {
                across *= 0.75f;
                up *= 0.75f;
                range *= 0.5f;
            }
            ADD(t);
        }
        best = 3.4028235e38f;
        i = 0;
        bestA = NULL;
        if (n > 0) {
            do {
                t = list[i];
                d = AI_DistSqToPointAtHeight(t, self->pos);
                if (d < best) {
                    if (flag) {
                        if (self->def->m_Type == 1 && plan->unk37 == 0) {
                            goto next;
                        }
                        if (func_00267E74(self)) {
                            dist = AI_FlatDistSqToPoint(t, plan->pos);
                            limit = GameObject_GetLeashRadius(self);
                            if (limit * GameObject_GetLeashRadius(self) <= dist) {
                                goto next;
                            }
                        }
                        if (AI_UpdatePathHeading(self, plan, t) == 0) {
                            goto next;
                        }
                    }
                    if (checkSight) {
                        saved = *(CObjectHead *)t;
                        t->pos.y = t->pos.y + GameObject_GetCollisionHeight(t);
                        t->pos.y = t->pos.y + t->eye;
                        eye = self->pos;
                        eye.y = eye.y + GameObject_GetCollisionHeight(self);
                        eye.y = eye.y + self->eye;
                        r = Collision_MoveInstance(t, eye, gCollisionInfoLineOfSight);
                        if ((Actor *)gCollisionResults.m_pInstance == self) {
                            r = 0;
                        }
                        *(CObjectHead *)t = saved;
                        if (r) {
                            goto next;
                        }
                        if (self->def->m_Type == 1) {
                            if ((self->def->m_dwTypeFlags & 0x8000) && AI_IsInViewCone(self, t, range, across, up) == 0) {
                                if (plan->unk37 == 0) {
                                    goto next;
                                }
                            } else {
                                AI_HearNoise(self, plan, &t->pos, 3);
                            }
                        }
                    }
                    best = d;
                    bestA = t;
                }
            next:
                i++;
            } while (i < n);
        }
    }
    if (flag && bestA == NULL && AI_UpdatePathHeading(self, plan, NULL) == 0) {
        plan->pos = self->pos;
        plan->region = self->region;
    }
    return bestA;
}
