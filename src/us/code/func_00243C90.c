/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002149C0.c, func_002149C0, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/vector.h"
#include "structs/intelligence.h"
#include "structs/game.h"
#include "structs/collision.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/collision.h"
#include "globals/player.h"


typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ f32 height;
} CameraInfo;


s32 AI_UpdatePathHeading(CPlayer *self, CAI *plan, CPlayer *target);
s32 AI_IsInViewCone(CPlayer *viewer, CPlayer *target, f32 range, f32 across, f32 up);
#ifdef TARGET_PORT
s8 EnemyAI_GetCurrentBehavior(CPlayer *self, CAI *plan); /* ABI: returns s8 (the host leaves the upper bits of the return register undefined) */
#else
s32 EnemyAI_GetCurrentBehavior(CPlayer *self, CAI *plan);
#endif
#ifdef TARGET_PORT
s8 EnemyAI_GetBehaviorIndex(CPlayer *self, CAI *plan); /* ABI: returns s8 (the host leaves the upper bits of the return register undefined) */
#else
s32 EnemyAI_GetBehaviorIndex(CPlayer *self, CAI *plan);
#endif
void AI_HearNoise(CPlayer *self, CAI *plan, CVector3 *pos, s32 arg3);
s32 Cinema_PathPlaying(void);
s32 Cinema_GetScriptId(void);
CPlayer *CScene__FindNearestIdleThrowable(u8 *scene, CPlayer *self);
CPlayer *CScene__FindNearestCover(u8 *scene, CPlayer *self);
CPlayer *CScene__FindNearestIdleThrowableB(u8 *scene, CPlayer *self);
CPlayer *CPlayerPool__FindNearest(CPlayerPool *players, CVector3 pos);
CPlayer *CPlayerPool__FindNearestOther(CPlayerPool *players, CVector3 pos, void *owner);
CameraInfo *GameObject_GetRegionInfo(CPlayer *player);
f32 AI_DistSqToPointAtHeight(CPlayer *a, CVector3 pos);
f32 AI_FlatDistSqToPoint(CPlayer *a, CVector3 pos);
s32 GameObject_IsLeashed(CPlayer *a);
f32 GameObject_GetLeashRadius(CPlayer *a);
f32 GameObject_GetCollisionHeight(CPlayer *a);                           /* height */
s32 Collision_MoveInstance(CPlayer *a, CVector3 from, u8 *result);

#define ADD(t) if ((t) != NULL) { list[n] = (t); n++; }

/* The plan's best target: the nearest of its candidates (sides' leaders,
 * the actor it ignores, a nearby creature, the nearest player) that it may
 * attack and, with `checkSight`, can see. */
CPlayer *AI_FindBestTarget(CPlayer *self, CAI *plan, s32 checkSight, s32 flag) {
    CPlayer *list[128];
    CObjectHead saved;
    CVector3 eye;
    f32 range;
    f32 across;
    f32 up;
    f32 best;
    f32 d;
    f32 limit;
    f32 dist;
    CPlayer *bestA;
    CPlayer *t;
    CameraInfo *cam;
    s32 n;
    s32 i;
    s32 ok;
    s32 side;
    s32 r;

    if (plan->m_pTargetOverride != NULL) {
        if (flag) {
            AI_UpdatePathHeading(self, plan, plan->m_pTargetOverride);
        }
        return plan->m_pTargetOverride;
    }
    if (self->m_CharacterActor.header.m_pIntelligence->m_Type == 1 && EnemyAI_GetCurrentBehavior(self, plan) != 2) {
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
            if (self->m_CharacterActor.header.m_pIntelligence->m_Type == 1 && plan->m_pSightTarget != NULL) {
                side = EnemyAI_GetBehaviorIndex(self, plan);
                if ((side == 0 && (self->m_CharacterActor.header.m_pIntelligence->m_Enemy.m_dwEnemyFlags & 0x40))
                    || (side == 1 && (self->m_CharacterActor.header.m_pIntelligence->m_Enemy.m_dwEnemyFlags & 0x80))
                    || (side == 2 && (self->m_CharacterActor.header.m_pIntelligence->m_Enemy.m_dwEnemyFlags & 0x100))) {
                    t = CScene__FindNearestIdleThrowable(&gScene, self);
                    ADD(t);
                }
                if (plan->m_pSightTarget != NULL
                    && ((side == 0 && (self->m_CharacterActor.header.m_pIntelligence->m_Enemy.m_wTakeCoverAnimFlags & 0x1F))
                        || (side == 1 && (self->m_CharacterActor.header.m_pIntelligence->m_Enemy.m_wTakeCoverAnimFlags & 0x3E0))
                        || (side == 2 && (self->m_CharacterActor.header.m_pIntelligence->m_Enemy.m_wTakeCoverAnimFlags & 0x7C00)))) {
                    t = CScene__FindNearestCover(&gScene, self);
                    ADD(t);
                }
            }
            t = plan->m_pParent;
            ADD(t);
            t = CScene__FindNearestIdleThrowableB(&gScene, self);
            ADD(t);
        }
        if (self->m_CharacterActor.m_dwFlags & 0x300000) {
            t = CPlayerPool__FindNearestOther(&gEngineApp_m_Game.m_PlayerList, self->m_CharacterActor.header.m_vPos, self->m_CharacterActor.m_AI.m_pParent);
        } else {
            t = CPlayerPool__FindNearest(&gEngineApp_m_Game.m_PlayerList, self->m_CharacterActor.header.m_vPos);
        }
        if (t != NULL && t->m_Data.m_Lives != 0 && t->m_Mode != 1) {
            cam = GameObject_GetRegionInfo(t);
            if (cam != NULL) {
                range = cam->height - 102.399994f;
            }
            if (self->m_CharacterActor.header.m_pIntelligence->m_Type == 1 && plan->m_Action == 0 && (gPlayerStateTable[t->m_Mode].physics->flags & 0x80)) {
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
                d = AI_DistSqToPointAtHeight(t, self->m_CharacterActor.header.m_vPos);
                if (d < best) {
                    if (flag) {
                        if (self->m_CharacterActor.header.m_pIntelligence->m_Type == 1 && plan->m_Action == 0) {
                            goto next;
                        }
                        if (GameObject_IsLeashed(self)) {
                            dist = AI_FlatDistSqToPoint(t, plan->m_vLeashPos);
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
                        t->m_CharacterActor.header.m_vPos.y = t->m_CharacterActor.header.m_vPos.y + GameObject_GetCollisionHeight(t);
                        t->m_CharacterActor.header.m_vPos.y = t->m_CharacterActor.header.m_vPos.y + t->m_CharacterActor.m_CollisionYOffset;
                        eye = self->m_CharacterActor.header.m_vPos;
                        eye.y = eye.y + GameObject_GetCollisionHeight(self);
                        eye.y = eye.y + self->m_CharacterActor.m_CollisionYOffset;
                        r = Collision_MoveInstance(t, eye, &gCollisionInfoLineOfSight);
                        if ((CPlayer *)gCollisionResults.m_pInstance == self) {
                            r = 0;
                        }
                        *(CObjectHead *)t = saved;
                        if (r) {
                            goto next;
                        }
                        if (self->m_CharacterActor.header.m_pIntelligence->m_Type == 1) {
                            if ((self->m_CharacterActor.header.m_pIntelligence->m_dwTypeFlags & 0x8000) && AI_IsInViewCone(self, t, range, across, up) == 0) {
                                if (plan->m_Action == 0) {
                                    goto next;
                                }
                            } else {
                                AI_HearNoise(self, plan, &t->m_CharacterActor.header.m_vPos, 3);
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
        plan->m_vLeashPos = self->m_CharacterActor.header.m_vPos;
        plan->m_pLeashRegion = self->m_CharacterActor.header.m_pCurrentRegion;
    }
    return bestA;
}
