#include "common.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/vector.h"
#include "structs/game.h"
#include "structs/intelligence.h"
#include "globals/game.h"
#include "structs/game_object.h"

/* Initial health and level of an actor from its definition, scaled by difficulty. Round 28: on CAI
 * (the N64 view and the port's host-offset one of round 10 y3 joined; by N64 offset the host's
 * aggression landed inside earlier members and every enemy kept m_Aggression 0). */
void AI_InitHealth(CAI *dst, CIntelligence *def, CVector3 pos, CGameObjectInstance *actor) {
    GameSettings *settings;
    s32 level;
    f32 health;
    f32 a;
    f32 b;
    f32 t;

    level = 0;
    a = 100.0f;
    b = 128.0f;
    dst->m_VisBits = -1;
    health = a;
    if (def != NULL) {
        settings = &gEngineApp_m_Settings;
        level = def->m_nTexture;
        if (settings->multiplayer != 0 && def->m_Type != 1 && def->m_Type != 11) {
            if (settings->match.tag != 0) {
                level = settings->tagTextureSet;
            } else {
                level = settings->dmTextureSet;
            }
        }
        switch (def->m_Type) {
            case 1:
                health = def->m_Enemy.m_Health;
                a = def->m_Enemy.m_TranqHealth;
                b = def->m_Enemy.m_Aggression;
                break;
            case 4:
                health = def->m_Enemy.m_Health;
                break;
            case 7:
                health = def->m_Destructible.m_Health;
                break;
        }
        if (def->m_Type == 1 || def->m_Type == 4) {
            switch (gEngineApp_m_Settings.difficulty) {
                case 8:
                    t = health * 0.5f;
                    b = 0.0f;
                    if (t < 1.0f) {
                        t = 1.0f;
                    }
                    health = t;
                    break;
                case 32:
                    b = 255.0f;
                    health *= 1.25f;
                    break;
            }
        }
    }
    dst->m_Health = health;
    dst->m_TranqHealth = (s8) a; /* converted as a signed byte */
    dst->m_nTexture = level;
    dst->m_Aggression = (s32)b;
    dst->m_Health <<= 8;
    dst->m_InitialHealth = dst->m_Health;
}

typedef struct CAISetupCallbacks {
    void (*first)(CGameObjectInstance *, CAI *);
} CAISetupCallbacks;
void memset(void *dst, s32 value, sptr size); /* memset */
s32 CGameObjectInstance__DrawModel();
void CGameObjectInstance__CalculateOrientationMatrix();

/* Builds an object's AI: clears it (keeping m_pParent), runs the shared
 * initialiser, installs the two callbacks, seeds the position and heading
 * copies, the flinch nodes and the default speeds, runs the optional first
 * callback, and flags it when the object asks (bit 27). */
void CAI__Setup(CGameObjectInstance *obj, CAI *ai, CAISetupCallbacks *callbacks) {
    struct CGameObjectInstance *parent;
    f32 speed;
    struct CGameRegion *region;
    /* The shared -1 is pinned to $v1: GCC's local allocator always gives
     * it $v0 (it outranks the flags temporary), which stops sched2 from
     * hoisting the load of ai->m_dwFlags above these stores. */
    register s16 none asm("$3");

    parent = ai->m_pParent;
    memset(ai, 0, sizeof(CAI));
    ai->m_dwFlags = 0x40;
    ai->m_pParent = parent;
    AI_InitHealth(ai, obj->header.m_pIntelligence, obj->header.m_vPos, obj);
    ai->m_pfCalculateOrientationMatrix = CGameObjectInstance__CalculateOrientationMatrix;
    ai->m_pfDraw = CGameObjectInstance__DrawModel;
    ai->m_Mode = -1;
    ai->m_ModeLastFrame = -1;
    ai->m_pModeCurrent = NULL;
    ai->m_vPosLastFrame = obj->header.m_vPos;
    ai->m_vDesiredPos = obj->header.m_vPos;
    ai->m_vTargetOffset.x = 0;
    ai->m_vTargetOffset.y = 0;
    ai->m_vTargetOffset.z = 0;
    ai->m_RotYLastFrame = obj->m_RotY;
    ai->m_ConditionTimer = 0;
    ai->m_pTargetOverride = NULL;
    ai->m_TargetOverrideTime = 0;
    ai->m_pSightTarget = NULL;
    ai->m_pPathAvoidRegion = NULL;
    ai->m_pPathTarget = NULL;
    ai->m_PathRotY = obj->m_RotY;
    ai->m_PathHeight = obj->header.m_vPos.y;
    ai->m_InitialRotY = obj->m_RotY;
    ai->m_vInitialPos = obj->header.m_vPos;
    ai->m_pInitialRegion = obj->header.m_pCurrentRegion;
    ai->m_vLeashPos = obj->header.m_vPos;
    region = obj->header.m_pCurrentRegion;
    ai->m_pLeashRegion = region;
    if ((obj->header.m_pIntelligence->m_dwFlags & 4) && region != NULL) {
        ai->m_vLeashPos.y = CGameRegion__GetHeight(region, ai->m_vLeashPos.x, ai->m_vLeashPos.z);
    }
    speed = 1.0f;
    ai->m_AnimIndex = -1;
    ai->m_FlinchTime = 0x14;
    ai->m_PathTrack.m_nCurrentPathIndex = 0xFF;
    none = -1;
    ai->m_AnimType = none;
    ai->m_ModeLastFrame = none;
    ai->m_FlinchNode[0] = none;
    ai->m_FlinchNode[1] = none;
    ai->m_FlinchNode[2] = none;
    ai->m_FlinchNode[3] = none;
    ai->m_PathTrack.m_nCurrentPointIndex = none;
    ai->m_AnimRepeatedCount = 0;
    ai->m_DeathRegionTimer = 0;
    ai->m_OnFireTimer = 0;
    ai->m_ActionTime = 0;
    ai->m_FlinchCountdown = 0;
    ai->m_vFlinchAxis.y = 0;
    ai->m_vFlinchAxis.z = 0;
    ai->m_HeadTurn = 0;
    ai->m_HeadTilt = 0;
    ai->m_PathTrack.m_Mode = 0;
    ai->m_pAttachedLoopingSound = NULL;
    ai->m_AnimSpeed = speed;
    ai->m_AnimModeSpeed = speed;
    ai->m_dwFlags &= ~1;
    ai->m_vFlinchAxis.x = speed;
    if (callbacks != NULL && callbacks->first != NULL) {
        callbacks->first(obj, ai);
    }
    if (obj->m_dwFlags & 0x8000000) {
        ai->m_AnimRepeatedCount = 1;
    }
}

#include "globals/collision.h"
#include "protos/mtxf.h"

void CMtxF__TransformPoints(CMtxF mf, CVector3 *in, CVector3 *out, s32 count);
void Platform_IdleEnter(CGameObjectInstance *mover, CGameObjectInstance *actor);

/* Carries an actor standing on a moving platform along with it. */
void AI_RideMovingPlatform(CGameObjectInstance *actor, CAI *dst) {
    CVector3 vel;
    CVector3 pos;
    CVector3 delta;
    CMtxF mtx;
    CGameObjectInstance *mover;
    f32 turn;

    mover = GameObject_GetPlatformBelow(actor);
    if (mover != NULL) {
        if (mover->header.m_pIntelligence->m_Type == 2) {
            dst->m_dwFlags = (dst->m_dwFlags & ~0x80000) | 0x40000;
            pos = actor->header.m_vPos;
            GameObject_GetFrameDelta(mover, &vel);
            if (!(dst->m_dwFlags & 0x80) || vel.y > 0.0f) {
                CVector3__Add(&pos, &pos, &vel);
            }
            vel = actor->m_vVelocity;
            actor->m_vVelocity.x = 0.0f;
            actor->m_vVelocity.y = 0.0f;
            actor->m_vVelocity.z = 0.0f;
            CVector3__Subtract(&delta, &pos, &mover->header.m_vPos);
            turn = GameObject_GetFrameTurn(mover);
            CMtxF__SetRotateY(mtx, turn);
            CMtxF__TransformPoints(mtx, &delta, &delta, 1);
            CVector3__Add(&delta, &delta, &mover->header.m_vPos);
            CVector3__Subtract(&delta, &delta, &pos);
            actor->m_RotY += turn;
            CVector3__Add(&pos, &pos, &delta);
            Collision_MoveInstance(actor, pos, &gCollisionInfoPlatformRider);
            actor->m_vVelocity = vel;
            if (mover->header.m_Type == 1 && mover->header.m_pIntelligence->m_Type == 2) {
                Platform_IdleEnter(mover, actor);
            }
        } else {
            dst->m_dwFlags = (dst->m_dwFlags & ~0x40000) | 0x80000;
        }
    } else {
        dst->m_dwFlags &= ~0xC0000;
    }
}

#include "structs/region.h"
#include "structs/collision.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/scene.h"


void CAI__CallHurt(void *arg0, CAI *arg1, void *arg2);
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80213ED4_de_10.c, func_802166F4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
void DamageEvent_Set(DamageEvent *event, void *source, s32 damage, f32 radius, s32 flags, HitFrame *frame);

/* CGameRegion flags for the actor (water, ground...) and periodic damage from the region it stands in. */
void AI_UpdateRegionFlags(CGameObjectInstance *actor, CAI *dst) {
    CGameRegion *region;
    CRegionInfo *info;
    DamageEvent damage;
    f32 y;
    f32 top;
    s32 hurt;
    s32 kind;

    dst->m_dwFlags &= ~0x7F80;
    region = actor->header.m_pCurrentRegion;
    if (region == NULL) {
        return;
    }
    info = CScene__GetRegionInfo(&gScene, &region->m_nInfo);
    if (info == NULL) {
        return;
    }
    if (info->m_dwFlags & 0x1000000) {
        dst->m_dwFlags |= 0x800;
    }
    if (region->m_wFlags & 1) {
        if (info->m_dwFlags & 0x20000000) {
            dst->m_dwFlags |= 0x400;
        } else {
            if (actor->m_dwFlags & 0x300000) {
                y = actor->header.m_vPos.y;
                top = ((CPlayer *) actor->m_AI.m_pParent)->m_pCamera->m_vTCorners[0].y;
            } else {
                top = GameObject_GetCollisionHeight(actor) + actor->header.m_vPos.y;
                y = actor->header.m_vPos.y;
            }
            if (top <= GameObject_GetWaterHeight(&actor->header)) {
                dst->m_dwFlags |= 0x80;
            } else if (GameObject_GetWaterHeight(&actor->header) < y) {
                dst->m_dwFlags |= 0x100;
            } else {
                dst->m_dwFlags |= 0x200;
            }
        }
    }
    if (CGameRegion__IsSteep(region) && (region->m_wFlags & 0x10) && GameObject_IsFacingRegionNormal(actor)) {
        dst->m_dwFlags |= 0x1000;
    }
    if (CGameRegion__GetHeight(region, actor->header.m_vPos.x, actor->header.m_vPos.z) - actor->header.m_vPos.y < 15.36f) {
        if (region->m_wFlags & 0x100) {
            dst->m_dwFlags |= 0x2000;
        }
        if (region->m_wFlags & 0x80) {
            dst->m_dwFlags |= 0x4000;
        }
    }
    if (info->m_dwFlags & 0x80000) {
        hurt = 1;
        if (info->m_dwFlags & 4) {
            hurt = actor->header.m_pIntelligence->m_Type != 1;
        }
        if (dst->m_dwFlags & 0xC0000) {
            hurt = 0;
        }
        if (!GameObject_IsOnGround(actor)) {
            hurt = 0;
        }
        if (actor->m_pInstanceBelow != NULL) {
            hurt = 0;
        }
        if (hurt) {
            if (!(dst->m_dwFlags & 0x8000)) {
                dst->m_DeathRegionTimer += gFrameIncrement * (1.0f / 15);
                if (dst->m_DeathRegionTimer >= info->m_DamageInterval) {
                    kind = 0x100000;
                    if (info->m_dwFlags & 0x400000) {
                        kind = 0x1100000;
                    }
                    dst->m_DeathRegionTimer -= info->m_DamageInterval;
                    DamageEvent_Set(&damage, 0, info->m_DamageAmount << 8, 25.599998f, kind, 0);
                    CAI__CallHurt(actor, dst, &damage);
                }
            }
            if (dst->m_dwFlags & 0x8000) {
                dst->m_DeathRegionTimer = info->m_DamageInterval;
                dst->m_dwFlags &= ~0x8000;
            }
        } else {
            dst->m_dwFlags &= ~0x8000;
        }
    } else {
        dst->m_dwFlags |= 0x8000;
    }
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80213ED4.CPP, func_80213ED4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* A mode's animation (CAIModeInfo.unk_10): a function that picks it, a table of actions, or an id. */
typedef void *(*StateCallback)(CGameObjectInstance *, CAI *);
s32 CGameObjectInstance__SetAnim(CGameObjectInstance *obj, sptr anim, s32 arg2);
s32 IsCodePointer(StateCallback);
s32 IsDataPointer(uptr);
void *CGameObjectInstance__PickAvailableAction(CGameObjectInstance *obj, void *table, s32 arg2);

/* Runs the current mode's animation: on a mode change picks the mode's animation (through its
 * picker or its action table) and its speed; counts the cycles the animation completes. Round 28:
 * on CGameObjectInstance and CAI (the N64 views and the port's host-offset ones joined: at the N64
 * offsets the host missed the animation speeds, and no animation advanced). */
void CAI__UpdateModeAnim(CGameObjectInstance *src, CAI *dst) {
    CAIModeInfo *entry;
    StateCallback callback;
    f32 factor;
    entry = dst->m_pModeCurrent;
    if (entry != 0) {
        if (dst->m_ModeLastFrame != dst->m_Mode) {
            if (entry->m_AnimSpeed >= 0.0f) {
                dst->m_AnimModeSpeed = entry->m_AnimSpeed;
            }
            dst->m_ModeLastFrameFlags = dst->m_ModeFlags;
            dst->m_ModeFlags = entry->unk_1C;
            src->m_CurrentAnimation.m_bCycleCompleted = 0;
            callback = (StateCallback) entry->unk_10;
            if (callback != 0) {
                if (IsCodePointer(callback) != 0) {
#ifdef TARGET_PORT
                    /* The pickers return a 16-bit animation action id (MotherBoss_PickPhaseAnim and
                     * the rest return s16; the action tables are BE16, CGameObjectInstance__PickAvailableAction). MIPS hands the
                     * caller a sign-extended v0; x86-64 and arm64 leave the register's upper bits
                     * unspecified (Linux: 0x42BF55FA for 0x55FA), and PickRandomAnim then found no
                     * animation: the Mother's attack anim never played on Linux. */
                    callback = (StateCallback)(sptr)(s16)(sptr)callback(src, dst);
#else
                    callback = callback(src, dst);
#endif
                } else if (IsDataPointer((uptr)callback) != 0) {
                    callback = (StateCallback)CGameObjectInstance__PickAvailableAction(src, callback, dst->m_AnimIndex);
                }
                if (callback != 0) {
                    if (callback != (StateCallback)-1) {
                        dst->m_AnimIndex = CGameObjectInstance__PickRandomAnim(src, (sptr)callback, dst->m_AnimIndex);
                        dst->m_AnimType = (s16)callback;
                        if (src->m_CurrentAnimation.m_nAnimIndex == dst->m_AnimIndex) {
                            src->m_CurrentAnimation.m_AnimationFrame = 0.0f;
                        }
                    }
                    dst->m_AnimRepeatedCount = 0;
                    dst->m_dwFlags &= ~1U;
                }
            }
            dst->m_ModeLastFrame = (u8)dst->m_Mode;
        } else if ((dst->m_AnimIndex == -1) || (dst->m_AnimType == -1)) {
            dst->m_AnimRepeatedCount++;
            dst->m_dwFlags |= 1;
        } else if (src->m_CurrentAnimation.m_nAnimIndex == dst->m_AnimIndex) {
            if (!(dst->m_dwFlags & 1)) {
                s32 difference = src->m_CurrentAnimation.m_nFrames - entry->unk_18;
                double converted = difference;
                if (difference < 0) {
                    converted += 4294967296.0;
                }
                if ((f32)converted <= src->m_CurrentAnimation.m_AnimationFrame) {
                    dst->m_AnimRepeatedCount++;
                    dst->m_dwFlags |= 1;
                }
            }
            if (src->m_CurrentAnimation.m_bCycleCompleted != 0) {
                src->m_CurrentAnimation.m_bCycleCompleted = 0;
                if (!(dst->m_dwFlags & 1)) {
                    dst->m_AnimRepeatedCount++;
                }
                dst->m_dwFlags &= ~1U;
            }
        }
        if (dst->m_ModeFlags & 0x10) {
            factor = dst->m_AnimModeSpeed;
        } else {
            factor = dst->m_AnimSpeed * dst->m_AnimModeSpeed;
        }
        gFrameIncrement *= factor;
        CGameObjectInstance__SetAnim(src, dst->m_AnimIndex, (dst->m_ModeFlags >> 5) & 1);
        if ((dst->m_AnimIndex != -1) && (src->m_CurrentAnimation.m_nRequestedAnimIndex != dst->m_AnimIndex)) {
            dst->m_AnimRepeatedCount = 0;
            dst->m_dwFlags &= ~1U;
            src->m_CurrentAnimation.m_bCycleCompleted = 0;
        }
        if (src->m_dwFlags & 0x08000000) {
            dst->m_AnimRepeatedCount++;
        }
    }
}

#ifdef TARGET_PORT
#include "globals/ai.h"
/* gendata types a data symbol by the declarations of the C that uses it. The shared base mode
 * table is reached only through the header rows' m_Mode words (integers), so nothing used it:
 * it came out as N64 bytes and the scan below read 0x20-byte rows as host ones. This reference
 * makes gendata lay it out as host CAIModeInfo rows. */
CAIModeInfo *const gAIBaseModeTable_port = gAIBaseModeTable;
#endif

/* Switches the AI to mode, found in its chain of mode tables (each table's header row links the
 * next one in m_Mode, its rows end with m_Mode -1); runs the mode's enter function. Returns
 * whether the AI is in that mode. Round 28: the N64 view (Rage Wars' Source110/Dest/StateEntry)
 * and the port's version on CGameObjectInstance/CAI/CAIModeInfo joined. */
s32 CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode) {
    CAIModeInfo *node;
    CAIModeInfo *entry;
    CAIModeInfo *scan;
    s32 flags;

    if (gScene.m_Mode == 2) {
        return 1;
    }
    if (gScene.m_Mode == 4) {
        obj->m_dwFlags |= 0x100;
    }
    ai->m_ModeBefore = ai->m_Mode;
    obj->m_CurrentAnimation.m_bAnimActive = 1;
    obj->m_BlendAnimation.m_bAnimActive = 1;
    if ((ai->m_pModeCurrent != NULL) && (ai->m_Mode == mode)) {
        return 1;
    }
    node = ai->m_pModeTable;
    entry = NULL;
    ai->m_Mode = mode;
    *(s32 *) &ai->m_ModeTime = 0;
    while (node != NULL) {
        scan = node + 1;
        if (scan->m_Mode != -1) {
            while (scan->m_Mode != -1) {
                if (scan->m_Mode == mode) {
                    entry = scan;
                    node = NULL;
                    break;
                }
                scan = scan + 1;
            }
        }
        if (node != NULL) {
            node = (CAIModeInfo *) node->m_Mode;
        }
    }
    if (entry == NULL) {
        return 0;
    }
    ai->m_pModeCurrent = entry;
    obj->m_dwHotPointFlags &= 0x1F80007F;
    if ((obj->m_dwFlags & 0x08000000) == 0) {
        ai->m_AnimRepeatedCount = 0;
        ai->m_dwFlags &= ~1U;
    }
    if (ai->m_pAttachedLoopingSound != NULL) {
        AI_StopSound(obj, ai);
    }
    if (entry->unk_04 != 0) {
        ((void (*)(CGameObjectInstance *, CAI *)) entry->unk_04)(obj, ai);
    }
    flags = entry->unk_1C;
    ai->m_ModeFlags = flags;
    if (flags & 4) {
        obj->m_vVelocity.x = 0;
        obj->m_vVelocity.y = 0;
        obj->m_vVelocity.z = 0;
    }
    return ai->m_Mode == mode;
}

/* Whether target is within range of viewer and inside its horizontal (across) and vertical (up) view angles. */
s32 AI_IsInViewCone(CGameObjectInstance *viewer, CGameObjectInstance *target, f32 range, f32 across, f32 up) {
    CVector3 from;
    CVector3 to;
    CVector3 dir;
    CVector3 ahead;
    CVector3 forward;
    CVector3 flatDir;
    CVector3 flatForward;
    CVector3 pitchDir;
    CVector3 pitchForward;
    CQuaternion rotation;
    CMtxF matrix;
    f32 distance;
    f32 dirLength;
    f32 forwardLength;
    f32 angle;
    f32 zero;

    from = viewer->header.m_vPos;
    from.y += viewer->m_CollisionYOffset;
    from.y += GameObject_GetCollisionHeight(viewer) * 0.9f;
    to = target->header.m_vPos;
    to.y += target->m_CollisionYOffset;
    to.y += GameObject_GetCollisionHeight(target) * 0.9f;
    CVector3__Subtract(&dir, &to, &from);
    distance = func_00298470(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
    if (range < distance) {
        return 0;
    }
    rotation = CGameObjectInstance__GetRotation(viewer);
    CQuaternion__ToMatrix(&rotation, matrix);
    ahead.x = 0.0f;
    ahead.y = 0.0f;
    ahead.z = distance;
    CMtxF__TransformPoint(matrix, &ahead, &forward);
    across *= 0.5f;
    up *= 0.5f;
    flatDir.x = dir.x;
    flatDir.y = 0.0f;
    zero = *(volatile f32 *)&flatDir.y;
    flatDir.z = dir.z;
    dirLength = func_00298470(flatDir.x * flatDir.x + flatDir.z * flatDir.z);
    if (dirLength != zero) {
        if (across) { /* identical branches, as in Rage Wars: they set the register allocation */
            flatDir.x /= dirLength;
            flatDir.z /= dirLength;
        } else {
            flatDir.x /= dirLength;
            flatDir.z /= dirLength;
        }
    }
    flatForward.x = forward.x;
    flatForward.y = zero;
    flatForward.z = forward.z;
    forwardLength = func_00298470(flatForward.x * flatForward.x + flatForward.z * flatForward.z);
    if (forwardLength != zero) {
        flatForward.x /= forwardLength;
        flatForward.z /= forwardLength;
    }
    angle = Math_AcosLookup(flatDir.x * flatForward.x + flatDir.y * flatForward.y + flatDir.z * flatForward.z);
    if (angle < zero) {
        angle = -angle;
    }
    if (across < angle) {
        return 0;
    }
    *(volatile f32 *)&pitchDir.x = 0.0f;
    pitchDir.z = dirLength;
    pitchDir.y = dir.y;
    CVector3__Normalize(&pitchDir);
    *(volatile f32 *)&pitchForward.x = 0.0f;
    pitchForward.z = forwardLength;
    pitchForward.y = forward.y;
    CVector3__Normalize(&pitchForward);
    angle = Math_AcosLookup(pitchDir.x * pitchForward.x + pitchDir.y * pitchForward.y + pitchDir.z * pitchForward.z);
    if (up < ((angle < 0.0f) ? -angle : angle)) {
        return 0;
    }
    return 1;
}

#ifdef TARGET_PORT
sptr CScene__GetPathRegion(s32 ref);
#endif

sptr Path_FindFleeRegion(CGameObjectInstance *a, sptr b, CVector3 pos, sptr region, sptr d, CVector3 *out);
s32 Path_FindRegionPath(CGameObjectInstance *a, CVector3 pos, sptr region, sptr d, CVector3 *hit);
s32 CGameRegion__ContainsXZ(sptr region, f32 x, f32 z);

/* The point beside `target`, on the side away from `from`, where `self`
 * stands touching it. */
static __inline__ void Approach(CGameObjectInstance *self, CGameObjectInstance *target, CGameObjectInstance *from, CVector3 *outPos, sptr *outRegion) {
    CVector3 dir;
    CVector3 to;
    f32 dist;

    if (from != NULL) {
        CVector3__Subtract(&dir, &target->header.m_vPos, &from->header.m_vPos);
        dir.y = 0.0f;
        CVector3__Normalize(&dir);
    } else {
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 0.0f;
    }
    dist = GameObject_GetCollisionRadius(target) + GameObject_GetCollisionRadius(self) + 51.199997f;
    CVector3__Scale(&dir, &dir, dist);
    CVector3__Add(&to, &target->header.m_vPos, &dir);
    GameObject_TraceMove(&target->header, to, outPos, (CGameRegion **) outRegion, 0, 0);
}

s32 AI_UpdatePathHeading(CGameObjectInstance *self, CAI *plan, CGameObjectInstance *target) {
    CVector3 pos;
    CVector3 hit;
    sptr region;
    s32 mode;
    CPlayer *player;
    CPathPoint *loc;

    if (plan->m_Health == 0) {
        mode = 6;
    } else if (target == NULL) {
        mode = 3;
        if (plan->m_PathTrack.m_Mode != 0) {
            mode = 4;
        }
    } else {
        mode = 2;
        if (target != plan->m_pParent) {
            if (target->header.m_pIntelligence->m_Type == 5) {
                mode = 5;
            } else if (target->m_nTypeFlag == 0x64F) {
                mode = 7;
            } else {
                mode = 0;
                if (gScene.m_bStreamAll == 0) {
                    if ((!(target->m_dwFlags & 0x300000)
                         || (player = ((CPlayer *)target->m_AI.m_pParent),
                             (!(player->m_FleeTime > 0.0f) || (mode = 1, self->m_nTypeFlag == 0xCA && player->m_Data.m_CurrentWeapon == 0x14))
                             && (((CGameObjectInstance *)player->m_pCrossHairTarget) != self || (mode = 1, player->m_CrossHairMode != 2))))
                        && (!(self->m_dwHotPointFlags & 2) || (mode = 1, self->m_nTypeFlag == 0xCA))) {
                        mode = 0;
                    }
                }
            }
        }
    }
    switch (mode) {
    case 6:
        pos = self->header.m_vPos;
        region = ((sptr)self->header.m_pCurrentRegion);
        break;
    case 4:
        loc = CPathTrack__GetCurrentPoint(&plan->m_PathTrack);
        pos = loc->m_vPos;
#ifdef TARGET_PORT
        region = CScene__GetPathRegion(loc->m_Region); /* the region index (func_0042CD04.c) */
#else
        region = loc->m_Region;
#endif
        break;
    case 3:
        pos = plan->m_vLeashPos;
        region = (sptr) plan->m_pLeashRegion;
        break;
    case 7:
        Approach(self, target, plan->m_pSightTarget, &pos, &region);
        break;
    case 1:
        region = Path_FindFleeRegion(self, (sptr) plan->m_pPathAvoidRegion, target->header.m_vPos, ((sptr)target->header.m_pCurrentRegion), (sptr) plan->m_pModeCurrent->m_pData, &pos);
        if (region == 0) {
            do {
    default:
                pos = target->header.m_vPos;
                region = ((sptr)target->header.m_pCurrentRegion);
            } while (0);
        }
        break;
    }
    if (Path_FindRegionPath(self, pos, region, (sptr) plan->m_pModeCurrent->m_pData, &hit) != 0) {
        if (CGameRegion__ContainsXZ(((sptr)self->header.m_pCurrentRegion), hit.x, hit.z) != 0) {
            plan->m_PathHeight = hit.y;
        } else {
            plan->m_PathHeight = hit.y + 51.199997f;
        }
        CVector3__Subtract(&hit, &hit, &self->header.m_vPos);
        plan->m_PathRotY = CVector3__GetYaw(&hit) + 3.1415927f;
        return 1;
    }
    if (mode == 1) {
        plan->m_PathHeight = pos.y;
        plan->m_PathRotY = self->m_RotY + AI_AngleToPoint(self, pos);
        return 1;
    }
    return 0;
}

#include "globals/player.h"
#ifdef TARGET_PORT
s8 EnemyAI_GetCurrentBehavior(CGameObjectInstance *self, CAI *plan); /* ABI: returns s8 (the host leaves the upper bits of the return register undefined) */
s8 EnemyAI_GetBehaviorIndex(CGameObjectInstance *self, CAI *plan);
#else
s32 EnemyAI_GetCurrentBehavior(CGameObjectInstance *self, CAI *plan); /* N64: the callers take the whole register */
s32 EnemyAI_GetBehaviorIndex(CGameObjectInstance *self, CAI *plan);
#endif

void AI_HearNoise(CGameObjectInstance *self, CAI *plan, CVector3 *pos, s32 arg3);
s32 Cinema_PathPlaying(void);
s32 Cinema_GetScriptId(void);
CGameObjectInstance *CScene__FindNearestIdleThrowable(struct CScene *scene, CGameObjectInstance *self);
CGameObjectInstance *CScene__FindNearestCover(struct CScene *scene, CGameObjectInstance *self);
CGameObjectInstance *CScene__FindNearestIdleThrowableB(struct CScene *scene, CGameObjectInstance *self);
CGameObjectInstance *CPlayerPool__FindNearest(CPlayerPool *players, CVector3 pos);
CGameObjectInstance *CPlayerPool__FindNearestOther(CPlayerPool *players, CVector3 pos, void *owner);
#define ADD(t) if ((t) != NULL) { list[n] = (t); n++; }

/* The plan's best target: the nearest of its candidates (sides' leaders,
 * the actor it ignores, a nearby creature, the nearest player) that it may
 * attack and, with `checkSight`, can see. */
CGameObjectInstance *AI_FindBestTarget(CGameObjectInstance *self, CAI *plan, s32 checkSight, s32 flag) {
    CGameObjectInstance *list[128];
    CObjectHead saved;
    CVector3 eye;
    f32 range;
    f32 across;
    f32 up;
    f32 best;
    f32 d;
    f32 limit;
    f32 dist;
    CGameObjectInstance *bestA;
    CGameObjectInstance *t;
    CRegionInfo *cam;
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
    if (self->header.m_pIntelligence->m_Type == 1 && EnemyAI_GetCurrentBehavior(self, plan) != 2) {
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
            if (self->header.m_pIntelligence->m_Type == 1 && plan->m_pSightTarget != NULL) {
                side = EnemyAI_GetBehaviorIndex(self, plan);
                if ((side == 0 && (self->header.m_pIntelligence->m_Enemy.m_dwEnemyFlags & 0x40))
                    || (side == 1 && (self->header.m_pIntelligence->m_Enemy.m_dwEnemyFlags & 0x80))
                    || (side == 2 && (self->header.m_pIntelligence->m_Enemy.m_dwEnemyFlags & 0x100))) {
                    t = CScene__FindNearestIdleThrowable(&gScene, self);
                    ADD(t);
                }
                if (plan->m_pSightTarget != NULL
                    && ((side == 0 && (self->header.m_pIntelligence->m_Enemy.m_wTakeCoverAnimFlags & 0x1F))
                        || (side == 1 && (self->header.m_pIntelligence->m_Enemy.m_wTakeCoverAnimFlags & 0x3E0))
                        || (side == 2 && (self->header.m_pIntelligence->m_Enemy.m_wTakeCoverAnimFlags & 0x7C00)))) {
                    t = CScene__FindNearestCover(&gScene, self);
                    ADD(t);
                }
            }
            t = plan->m_pParent;
            ADD(t);
            t = CScene__FindNearestIdleThrowableB(&gScene, self);
            ADD(t);
        }
        if (self->m_dwFlags & 0x300000) {
            t = CPlayerPool__FindNearestOther(&gEngineApp_m_Game.m_PlayerList, self->header.m_vPos, self->m_AI.m_pParent);
        } else {
            t = CPlayerPool__FindNearest(&gEngineApp_m_Game.m_PlayerList, self->header.m_vPos);
        }
        if (t != NULL && ((CPlayer *) t)->m_Data.m_Lives != 0 && ((CPlayer *) t)->m_Mode != 1) {
            cam = (CRegionInfo *) GameObject_GetRegionInfo(&t->header);
            if (cam != NULL) {
                range = cam->m_FarClip - 102.399994f;
            }
            if (self->header.m_pIntelligence->m_Type == 1 && plan->m_Action == 0 && (gPlayerStateTable[((CPlayer *) t)->m_Mode].physics->flags & 0x80)) {
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
                d = AI_DistSqToPointAtHeight(t, self->header.m_vPos);
                if (d < best) {
                    if (flag) {
                        if (self->header.m_pIntelligence->m_Type == 1 && plan->m_Action == 0) {
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
                        t->header.m_vPos.y = t->header.m_vPos.y + GameObject_GetCollisionHeight(t);
                        t->header.m_vPos.y = t->header.m_vPos.y + t->m_CollisionYOffset;
                        eye = self->header.m_vPos;
                        eye.y = eye.y + GameObject_GetCollisionHeight(self);
                        eye.y = eye.y + self->m_CollisionYOffset;
                        r = Collision_MoveInstance(t, eye, &gCollisionInfoLineOfSight);
                        if ((CGameObjectInstance *)gCollisionResults.m_pInstance == self) {
                            r = 0;
                        }
                        *(CObjectHead *)t = saved;
                        if (r) {
                            goto next;
                        }
                        if (self->header.m_pIntelligence->m_Type == 1) {
                            if ((self->header.m_pIntelligence->m_dwTypeFlags & 0x8000) && AI_IsInViewCone(self, t, range, across, up) == 0) {
                                if (plan->m_Action == 0) {
                                    goto next;
                                }
                            } else {
                                AI_HearNoise(self, plan, &t->header.m_vPos, 3);
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
        plan->m_vLeashPos = self->header.m_vPos;
        plan->m_pLeashRegion = self->header.m_pCurrentRegion;
    }
    return bestA;
}

#ifdef TARGET_PORT
s8 EnemyAI_GetBehaviorIndex(CGameObjectInstance *self, CAI *plan); /* ABI: returns s8 (the host leaves the upper bits of the return register undefined) */
#else
s32 EnemyAI_GetBehaviorIndex(CGameObjectInstance *self, CAI *plan);
#endif

/* Picks what the plan aims at (its target, itself, a location or home
 * point, or the actor) and fills `out` with the kind, target, range, goal
 * position and its offset from the actor, in 3D and flattened. The caller
 * (AI_UpdateTargets) passes the plan's m_pPathTarget as `target`; the path
 * update below may change it, so it is read again from the plan into the
 * parameter. With the parameter reused that way and boost a local of its
 * own, the $s registers come out as in the ROM. */
void AI_GetPathTargetInfo(CGameObjectInstance *self, CAI *plan, CGameObjectInstance *target, CAITarget *out) {
    CVector3 pos;
    CVector3 probe;
    CVector3 away;
    CVector3 delta;
    sptr region;
    s32 boost;
    s32 kind;
    s32 mode;
    f32 range;
    f32 gap;
    f32 angle;
    f32 reach;
    CPlayer *player;
    CPathPoint *loc;

    boost = 0;
    if (self->header.m_pIntelligence->m_Type == 1) {
        mode = EnemyAI_GetBehaviorIndex(self, plan);
    } else {
        mode = -1;
    }
    if (plan->m_PathFindCount == gScene.m_Kind1Cursor) {
        if ((plan->m_dwFlags & 0x20000000) && plan->m_pTargetOverride == NULL) {
            if (AI_UpdatePathHeading(self, plan, plan->m_pPathTarget) == 0) {
                plan->m_dwFlags &= ~0x20000000;
            }
        } else {
            plan->m_pPathTarget = AI_FindBestTarget(self, plan, 0, 1);
        }
    }
    target = plan->m_pPathTarget;
    if (plan->m_Health == 0) {
        kind = 6;
    } else if (target == NULL) {
        kind = 3;
        if (plan->m_PathTrack.m_Mode != 0) {
            kind = 4;
        }
    } else {
        kind = 2;
        if (target != plan->m_pParent) {
            if (target->header.m_pIntelligence->m_Type == 5) {
                kind = 5;
            } else if (target->m_nTypeFlag == 0x64F) {
                kind = 7;
            } else {
                kind = 0;
                if (gScene.m_bStreamAll == 0) {
                    if ((!(target->m_dwFlags & 0x300000)
                         || (player = ((CPlayer *)target->m_AI.m_pParent),
                             (!(player->m_FleeTime > 0.0f) || (kind = 1, self->m_nTypeFlag == 0xCA && player->m_Data.m_CurrentWeapon == 0x14))
                             && (((CGameObjectInstance *)player->m_pCrossHairTarget) != self || (kind = 1, player->m_CrossHairMode != 2))))
                        && (!(self->m_dwHotPointFlags & 2) || (kind = 1, self->m_nTypeFlag == 0xCA))) {
                        kind = 0;
                    }
                }
            }
        }
    }
    if (target != NULL) {
        if (kind == 7) {
            Approach(self, target, plan->m_pSightTarget, &pos, &region);
        } else {
            pos = target->header.m_vPos;
            region = ((sptr)target->header.m_pCurrentRegion);
            if (mode == 1) {
                pos.y += target->m_CollisionYOffset + GameObject_GetCollisionHeight(target) * 0.9f;
            }
        }
        gap = GameObject_GetCollisionRadius(target) + GameObject_GetCollisionRadius(self);
        switch (kind) {
        case 2:
            gap += 102.399994f;
            break;
        case 5:
        case 7:
            gap = 0.0f;
            break;
        }
        boost = 1;
        if (plan->m_PathTrack.m_Mode == 1) {
            plan->m_PathTrack.m_Mode = 2;
        } else if (plan->m_PathTrack.m_Mode == 3) {
            plan->m_PathTrack.m_Mode = 4;
        }
    } else {
        gap = GameObject_GetCollisionRadius(self);
        switch (kind) {
        case 4:
            loc = CPathTrack__GetCurrentPoint(&plan->m_PathTrack);
            pos = loc->m_vPos;
            gap = 0.0f;
#ifdef TARGET_PORT
            region = CScene__GetPathRegion(loc->m_Region); /* the region index (func_0042CD04.c) */
#else
            region = loc->m_Region;
#endif
            if (plan->m_PathTrack.m_Mode == 2 || plan->m_PathTrack.m_Mode == kind) {
                boost = 1;
            }
            break;
        case 3:
            pos = plan->m_vLeashPos;
            region = (sptr) plan->m_pLeashRegion;
            boost = 1;
            break;
        case 6:
            pos = self->header.m_vPos;
            region = ((sptr)self->header.m_pCurrentRegion);
            if (mode == 2) {
                pos.y = GameObject_GetWaterHeight(&self->header);
            }
            break;
        }
    }
    CVector3__Add(&pos, &pos, &plan->m_vTargetOffset);
    if (gap > 0.0f) {
        CVector3__Subtract(&away, &self->header.m_vPos, &pos);
        angle = CVector3__GetYaw(&away);
        pos.x -= gap * func_002982F0(angle);
        pos.z -= gap * func_002974C0(angle);
    }
    if (mode == 2) {
        pos.y = (pos.y < GameObject_GetWaterHeight(&self->header)) ? pos.y : GameObject_GetWaterHeight(&self->header);
    }
    if (boost) {
        reach = GameObject_GetSweepRadius((u8 *) self);
        probe.x = self->header.m_vPos.x + reach * func_002982F0(plan->m_PathRotY);
        probe.y = plan->m_PathHeight;
        probe.z = self->header.m_vPos.z + reach * func_002974C0(plan->m_PathRotY);
        range = AI_FindClearHeading(self, probe, target, 1.5f);
        pos.y = plan->m_PathHeight;
    } else {
        range = AI_AngleToPoint(self, pos);
    }
    CVector3__Subtract(&delta, &pos, &self->header.m_vPos);
    out->m_Kind = kind;
    out->m_pTarget = target;
    out->m_Angle = range;
    out->m_vPos = pos;
    out->m_vDelta = delta;
    out->m_Dist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
    delta.y = 0.0f;
    pos.y = 0.0f;
    out->m_vFlatPos = pos;
    out->m_vFlatDelta = delta;
    out->m_FlatDist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}

/* What the plan aims at (the same classes as AI_UpdatePathHeading). */
static __inline__ s32 Kind(CGameObjectInstance *self, CAI *plan, CGameObjectInstance *target) {
    CPlayer *player;

    if (plan->m_Health == 0) {
        return 6;
    }
    if (target == NULL) {
        if (plan->m_PathTrack.m_Mode != 0) {
            return 4;
        }
        return 3;
    }
    if (target == plan->m_pParent) {
        return 2;
    }
    if (target->header.m_pIntelligence->m_Type == 5) {
        return 5;
    }
    if (target->m_nTypeFlag == 0x64F) {
        return 7;
    }
    if (gScene.m_bStreamAll == 0) {
        if (target->m_dwFlags & 0x300000) {
            player = ((CPlayer *)target->m_AI.m_pParent);
            if (player->m_FleeTime > 0.0f && (self->m_nTypeFlag != 0xCA || player->m_Data.m_CurrentWeapon != 0x14)) {
                return 1;
            }
            if (((CGameObjectInstance *)player->m_pCrossHairTarget) == self && player->m_CrossHairMode == 2) {
                return 1;
            }
        }
        if ((self->m_dwHotPointFlags & 2) && self->m_nTypeFlag != 0xCA) {
            return 1;
        }
    }
    return 0;
}

/* Picks the plan's target and fills `out` with its kind, the turn to face
 * it, its position, offset and distance, in 3D and flattened. */
void AI_GetSightTargetInfo(CGameObjectInstance *self, CAI *plan, CGameObjectInstance *unused, CAITarget *out) {
    CVector3 pos;
    CVector3 delta;
    f32 angle;
    s32 kind;
    CGameObjectInstance *target;
    CPathPoint *loc;

    if (plan->m_PathFindCount == gScene.m_Kind1Cursor && (!(plan->m_dwFlags & 0x40000000) || plan->m_pTargetOverride != NULL)) {
        plan->m_pSightTarget = AI_FindBestTarget(self, plan, 1, 0);
    }
    if (self->header.m_pIntelligence->m_Type == 1 && plan->m_Mode == 0xB && Kind(self, plan, plan->m_pPathTarget) == 4) {
        target = plan->m_pPathTarget;
        kind = 4;
    } else {
        target = plan->m_pSightTarget;
        kind = Kind(self, plan, target);
    }
    if (target != NULL) {
        pos = target->header.m_vPos;
        angle = AI_AngleToPoint(self, pos);
    } else {
        switch (kind) {
        case 4:
            loc = CPathTrack__GetCurrentPoint(&plan->m_PathTrack);
            pos = loc->m_vPos;
            if (loc->m_Flags & 1) {
                angle = loc->m_RotY - self->m_RotY;
            } else {
                angle = AI_AngleToPoint(self, pos);
            }
            break;
        case 3:
            pos = plan->m_vLeashPos;
            angle = plan->m_InitialRotY - self->m_RotY;
            break;
        case 6:
            pos = self->header.m_vPos;
            angle = 0.0f;
            break;
        }
    }
    Angle_Wrap(&angle);
    CVector3__Subtract(&delta, &pos, &self->header.m_vPos);
    out->m_Kind = kind;
    out->m_pTarget = target;
    out->m_Angle = angle;
    out->m_vPos = pos;
    out->m_vDelta = delta;
    out->m_Dist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
    delta.y = 0.0f;
    pos.y = 0;
    out->m_vFlatPos = pos;
    out->m_vFlatDelta = delta;
    out->m_FlatDist = func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}

/* out receives a CollisionInfo pointer (func_0026961C.c): pointer-sized (sptr). As an s32, the
 * host's 8-byte store overwrote the 4 bytes after it (round 15 d5: with -O2 and no zero-init they
 * were part of a saved register, and AI_GetPathTargetInfo crashed in level 53). */

/* AI_AngleToPoint: the angle from the actor's heading to a point. */
static __inline__ f32 angleTo(CGameObjectInstance *actor, CVector3 point) {
    f32 dx;
    f32 dz;
    f32 dist;
    f32 s;
    f32 c;
    f32 dot;
    f32 angle;

    dx = point.x - actor->header.m_vPos.x;
    dz = point.z - actor->header.m_vPos.z;
    dist = func_00298470(dx * dx + dz * dz);
    if (dist == 0.0f) {
        return 0.0f;
    }
    do {
        s = func_002982F0(actor->m_RotY);
    } while (0);
    c = -func_002974C0(actor->m_RotY);
    dot = (dx * -s + dz * c) / dist;
    dot = ((((dot) > (1.0f) ? (1.0f) : (dot))) < ((-1.0f)) ? ((-1.0f)) : (((dot) > (1.0f) ? (1.0f) : (dot))));
    angle = Math_AcosLookup(dot);
    if (!(dx * c + dz * s > 0.0f)) {
        angle = -angle;
    }
    return angle;
}

/* The heading for self toward pos: straight at it, or when pos is blocked,
 * the first free direction 30, 60, ... 150 degrees to either side (the side
 * tried first alternating between actors), turned 15 degrees further. */
f32 AI_FindClearHeading(CGameObjectInstance *self, CVector3 pos, CGameObjectInstance *target, f32 scale) {
    CVector3 probe;
    sptr out;
    f32 heading;
    f32 angle;
    f32 step;
    f32 extra;
    s32 i;
    s32 side;
    s32 first;

    angle = angleTo(self, pos);
    if ((self->m_dwFlags & 0x200) && GameObject_TestMove(self, pos, (CCollisionInfo **) &out)) {
        if (target != NULL && gCollisionResults.m_pInstance == target) {
            return angle;
        }
        probe.y = self->header.m_vPos.y;
        first = ((uptr) self % 0x288) & 1;
        for (i = 1; i < 6; i++) {
            step = i * 0.52359885f;
            for (side = 0; side < 2; side++) {
                heading = angle;
                if (side != first) {
                    heading = angle + step;
                    extra = 0.26179942f;
                } else {
                    heading = angle - step;
                    extra = -0.26179942f;
                }
                probe.x = self->header.m_vPos.x + func_002982F0(self->m_RotY + 3.1415927f + heading) * (scale * GameObject_GetCollisionRadius(self));
                probe.z = self->header.m_vPos.z + func_002974C0(self->m_RotY + 3.1415927f + heading) * (scale * GameObject_GetCollisionRadius(self));
                if (!GameObject_TestMove(self, probe, NULL)) {
                    heading += extra;
                    Angle_Wrap(&heading);
                    return heading;
                }
            }
        }
    }
    return angle;
}

s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon);   /* ammo for a weapon */
void CGameObjectInstance__LocalToWorld(CVector3 *out, CGameObjectInstance *a, CVector3 v);    /* v in a's frame */

/* AI_AngleToPoint: the signed angle from the actor's heading to a point. */
static __inline__ f32 AngleTo(CGameObjectInstance *actor, CVector3 point) {
    f32 dx;
    f32 dz;
    f32 dist;
    f32 s;
    f32 c;
    f32 dot;
    f32 angle;

    dx = point.x - actor->header.m_vPos.x;
    dz = point.z - actor->header.m_vPos.z;
    dist = func_00298470(dx * dx + dz * dz);
    if (dist == 0.0f) {
        return 0.0f;
    }
    do {
        s = func_002982F0(actor->m_RotY);
    } while (0);
    c = -func_002974C0(actor->m_RotY);
    dot = (dx * -s + dz * c) / dist;
    dot = ((((dot) > (1.0f) ? (1.0f) : (dot))) < ((-1.0f)) ? ((-1.0f)) : (((dot) > (1.0f) ? (1.0f) : (dot))));
    angle = Math_AcosLookup(dot);
    if (!(dx * c + dz * s > 0.0f)) {
        angle = -angle;
    }
    return angle;
}

/* Whether `self` sees `target` (a player holding a ready weapon, or a
 * creature in an attack pose) aiming at it: 0 if not, else the side (-1/1). */
s32 AI_IsAimedAt(CGameObjectInstance *self, sptr unused, CGameObjectInstance *target, f32 cone) {
    CVector3 dir;
    CVector3 facing;
    CVector3 fwd;
    CVector3 from;
    CVector3 to;
    f32 lo;
    f32 hi;
    f32 y;
    f32 angle;
    CPlayer *player;

    if (target == NULL) {
        return 0;
    }
    if (target->m_dwFlags & 0x300000) {
        player = ((CPlayer *)target->m_AI.m_pParent);
        if (CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon) <= 0) {
            return 0;
        }
        if (!(player->m_ContState.m_dwButtons & 0x2000)) {
            return 0;
        }
        goto body;
    }
    switch (target->header.m_pIntelligence->m_Type) {
    case 1:
        if (target->m_AI.m_AnimType < 0x10CC) {
            return 0;
        }
        goto body;
    case 4:
        if (target->m_AI.m_Mode != 0x3F) {
            return 0;
        }
        goto body;
    default:
        return 0;
    }
body:
    lo = self->header.m_vPos.y + self->m_CollisionYOffset;
    hi = lo + GameObject_GetCollisionHeight(self);
    to.x = target->header.m_vPos.x;
    y = target->header.m_vPos.y + target->m_CollisionYOffset + GameObject_GetCollisionHeight(target) * 0.9f;
    to.y = y;
    to.z = target->header.m_vPos.z;
    from.x = self->header.m_vPos.x;
    from.z = self->header.m_vPos.z;
    if (hi < y) {
        from.y = hi;
    } else if (y < lo) {
        from.y = lo;
    } else {
        from.y = y;
    }
    angle = (AngleTo(self, to) < 0.0f) ? -AngleTo(self, to) : AngleTo(self, to);
    if (1.2217306f < angle && angle < 1.9198624f) {
        return 0;
    }
    CVector3__Subtract(&dir, &from, &to);
    CVector3__Normalize(&dir);
    fwd.x = 0;
    fwd.y = 0;
    fwd.z = 1.0f;
    CGameObjectInstance__LocalToWorld(&facing, target, fwd);
    if (Math_AcosLookup(dir.x * facing.x + dir.y * facing.y + dir.z * facing.z) < cone * 0.5f) {
        if (dir.x * -facing.z + dir.z * facing.x < 0.0f) {
            return -1;
        }
        return 1;
    }
    return 0;
}

/* Maps an object's type id to a class 0-9 (3 by default). */
s32 Actor_GetTypeClass(u8 *obj) {
    switch (GameObject_GetTypeId((CGameObjectInstance *) obj)) {
    case 1: case 2:
        return 0;
    case 3: case 4:
        return 1;
    case 5: case 6:
        return 2;
    case 7: case 8:
        return 3;
    case 9: case 10:
    case 100:
    case 102:
    case 0x460: case 0x461: case 0x462: case 0x463:
        return 4;
    case 11: case 12:
        return 5;
    case 13: case 14:
        return 6;
    case 15: case 16:
        return 7;
    case 17: case 18:
        return 8;
    case 19: case 20:
        return 9;
    default:
        return 3;
    }
}

/* Fills a damage event. */
void DamageEvent_Set(DamageEvent *event, void *source, s32 damage, f32 radius, s32 flags, HitFrame *frame) {
    event->source = source;
    event->damage = damage;
    event->radius = radius;
    event->flags = flags;
    event->frame = frame;
}

/* Typed on CGameObjectInstance / CAI (m2c's Source/Dest views had the N64 offsets). */

/* The attached looping sound (only its state word). */
typedef struct {
    u8 pad0[0xC];
    s32 unkC;                   /* 0xC: -1 = finished */
} SoundSource;

void CAI__Update(CGameObjectInstance *src, CAI *dst) {
    SoundSource *sound = (SoundSource *) dst->m_pAttachedLoopingSound;

    if (sound != NULL && sound->unkC == -1) {
        AI_StopSound(src, dst);
    }
    if (dst->m_FreezeTime != 0) {
        dst->m_FreezeTime--;
        gFrameIncrement = 0.0f;
        return;
    }
    if (src->m_AI.m_pModeTable != 0) {
        if (src->header.m_pIntelligence->m_Type == 2) {
            dst->m_vPosLastFrame = src->header.m_vPos;
            dst->m_RotYLastFrame = src->m_RotY;
        }
        dst->m_vDesiredPos = src->header.m_vPos;
        dst->m_ModeTime += gFrameIncrement;
        AI_UpdateRegionFlags(src, dst);
        if (src->m_dwFlags & 0x10000) {
            AI_RideMovingPlatform(src, dst);
        }
        if (dst->m_pfStep != NULL) {
            ((void (*)(CGameObjectInstance *, CAI *)) dst->m_pfStep)(src, dst);
        }
        CAI__UpdateModeAnim(src, dst);
        if (src->header.m_pIntelligence->m_Type != 2) {
            dst->m_vPosLastFrame = src->header.m_vPos;
            dst->m_RotYLastFrame = src->m_RotY;
        }
    }
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002165F8.c, func_002165F8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

void AI_ComputeFrameMotion(CGameObjectInstance *a, CAI *b, CAIFrameMotion *o) {
    CVector3__Subtract(&o->m_vDelta, &a->header.m_vPos, &b->m_vPosLastFrame);
    o->m_vDir = o->m_vDelta;
    CVector3__Normalize(&o->m_vDir);
    o->m_Dist = func_00298470(o->m_vDelta.x * o->m_vDelta.x + o->m_vDelta.y * o->m_vDelta.y + o->m_vDelta.z * o->m_vDelta.z);
    o->m_vFlatDelta.y = 0;
    o->m_vFlatDelta.x = o->m_vDelta.x;
    o->m_vFlatDelta.z = o->m_vDelta.z;
    o->m_vFlatDir = o->m_vFlatDelta;
    CVector3__Normalize(&o->m_vFlatDir);
    o->m_FlatDist = func_00298470(o->m_vFlatDelta.x * o->m_vFlatDelta.x + o->m_vFlatDelta.y * o->m_vFlatDelta.y + o->m_vFlatDelta.z * o->m_vFlatDelta.z);
    o->m_Turn = a->m_RotY - b->m_RotYLastFrame;
}

extern void WeightedTable_Clear(WeightedTable *);
extern void WeightedTable_Add(WeightedTable *, s32, s32);
extern s16 WeightedTable_Pick(WeightedTable *);

s32 AI_PickAnimFromTable(CGameObjectInstance *owner, sptr unused, CEnemyAnimPick *table, s32 flags) {
    WeightedTable picker;
    s32 first;
    s16 pick;
    WeightedTable_Clear(&picker);
    first = table->m_Anim;
    while (table->m_Flags != -1) {
        if (table->m_Flags == 0 && picker.count != 0) {
            goto choose;
        }
        if ((flags & table->m_Flags) && CGameObjectInstance__PickRandomAnim(owner, table->m_Anim, -1) != -1) {
            WeightedTable_Add(&picker, (s16) table->m_Anim, 10);
        }
        table++;
    }
    if (picker.count == 0) {
        WeightedTable_Add(&picker, (s16) first, 10);
    }
choose:
    pick = WeightedTable_Pick(&picker);
    if (CGameObjectInstance__PickRandomAnim(owner, pick, -1) == -1) {
        return first;
    }
    return pick;
}

void AI_TurnClamped(void *arg0, sptr arg1, f32 arg2, f32 arg3) {
    f32 f0;
    f32 f1;
    f32 f2;
    f32 f3;
    f0 = arg2;
    f1 = arg3;
    f3 = f1;
    if (f0 < f1 || (f0 = -f0, f1 < f0)) {
        f1 = f0;
    }
    f1 = f1 * gFrameIncrement;
    f2 = f1;
    if (f1 < 0.0f) {
        f2 = -f1;
    }
    if (f3 < 0.0f) {
        if (-f3 < f2) {
            goto clamp;
        }
    } else if (f3 < f2) {
clamp:
        f1 = f3;
    }
    Actor_TurnBy(arg0, f1);
}

/* Turns the object toward `t` (around what blocks the way), at most `lim` a frame. */
void AI_TurnTowardTarget(CGameObjectInstance *o, CAI *ai, CGameObjectInstance *t, f32 lim) {
    f32 r, v, a;
    if (t != NULL) {
        r = AI_FindClearHeading(o, t->header.m_vPos, t, 1.5f);
        v = r;
        if (lim < r) r = lim;
        else if (r < -lim) r = -lim;
        r = r * gFrameIncrement;
        a = r;
        if (a < 0.0f) a = -a;
        if (v < 0.0f) {
            if (-v < a) goto blk;
        } else if (v < a) {
blk:
            r = v;
        }
        Actor_TurnBy(o, r);
    }
}

/* WRONG CUT: the ROM splits this function at 0x245E20 (func_00245E20 = its last 10 words,
   reached by branches from here). Compiled whole it is 0xF8 bytes; the first 0xD0 match
   AI_GetTargetKind and the tail matches func_00245E20 word for word. */

s32 AI_GetTargetKind(CGameObjectInstance *self, CAI *target, CGameObjectInstance *actor) {
    CPlayer *owner;

    if (target->m_Health == 0) {
        return 6;
    }
    if (actor == NULL) {
        if (target->m_PathTrack.m_Mode != 0) {
            return 4;
        }
        return 3;
    }
    if (actor == target->m_pParent) {
        return 2;
    }
    if (actor->header.m_pIntelligence->m_Type == 5) {
        return 5;
    }
    if (actor->m_nTypeFlag == 0x64F) {
        return 7;
    }
    if (gScene.m_bStreamAll == 0) {
        if (actor->m_dwFlags & 0x300000) {
            owner = ((CPlayer *)actor->m_AI.m_pParent);
            if (0.0f < owner->m_FleeTime) {
                if (self->m_nTypeFlag != 0xCA || owner->m_Data.m_CurrentWeapon != 0x14) {
                    return 1;
                }
            }
            if (owner->m_pCrossHairTarget == self && owner->m_CrossHairMode == 2) {
                return 1;
            }
        }
        if ((self->m_dwHotPointFlags & 2) && self->m_nTypeFlag != 0xCA) {
            return 1;
        }
    }
    return 0;
}

/* Whether the line between the two objects' eyes (0.8 of their height up)
 * hits a wall or floor region flagged 2. */
s32 AI_IsEyeLineBlocked(CGameObjectInstance *from, s32 unused, CGameObjectInstance *to) {
    CObjectHead saved;
    CVector3 target;

    saved = *(CObjectHead *) from;
    from->header.m_vPos.y += GameObject_GetCollisionHeight(from) * 0.8f;
    from->header.m_vPos.y += from->m_CollisionYOffset;
    target = to->header.m_vPos;
    target.y += GameObject_GetCollisionHeight(to) * 0.8f;
    target.y += to->m_CollisionYOffset;
    Collision_MoveInstance(from, target, &gCollisionInfoSight);
    *(CObjectHead *) from = saved;
    if (gCollisionResults.m_pWallRegion != NULL && (gCollisionResults.m_pWallRegion->m_wFlags & 2)) {
        return 1;
    }
    if (gCollisionResults.m_pFloorRegion != NULL && (gCollisionResults.m_pFloorRegion->m_wFlags & 2)) {
        return 1;
    }
    return 0;
}

struct CollisionInfo;

/* Whether the line from the top of `from` to the top of `target` is blocked,
 * by something other than `target` itself. */
s32 AI_IsLineOfSightBlocked(CGameObjectInstance *target, sptr unused, CGameObjectInstance *from) {
    CObjectHead saved;
    CVector3 to;
    s32 hit;

    saved = *(CObjectHead *) from;
    from->header.m_vPos.y += GameObject_GetCollisionHeight(from);
    from->header.m_vPos.y += from->m_CollisionYOffset;
    to = target->header.m_vPos;
    to.y += GameObject_GetCollisionHeight(target);
    to.y += target->m_CollisionYOffset;
    hit = Collision_MoveInstance(from, to, &gCollisionInfoLineOfSight);
    if (gCollisionResults.m_pInstance == target) {
        hit = 0;
    }
    *(CObjectHead *) from = saved;
    return hit;
}

typedef struct {
    /* 0x00 */ u8 pad0[0x78];
    /* 0x78 */ s32 unk78;
    /* 0x7C */ s32 unk7C;
} Target;

/* host body (round 10 y2): the CAI by member. The N64 view (0x78 the target, 0x7C its time) wrote
 * the target's low half and the time's bits over the host CAI's m_pParent: an enemy hurt by
 * another actor (EnemyAI_OnDamage: the Blind One's minions under the boss's own fire) got a
 * garbage parent and crashed in AI_FindBestTarget. The time stays a float's bits (the ROM's word). */
void AI_SetTargetOverride(sptr arg0, Target *arg1, sptr arg2, s32 arg3) {
    CAI *ai = (CAI *) arg1;

    ai->m_pTargetOverride = (struct CGameObjectInstance *) arg2;
    *(s32 *) &ai->m_TargetOverrideTime = arg3;
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00216D3C.c, func_00216D3C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct { u8 pad0[4]; CVector3 pos; } B04A; /* pos at 0x4 in Turok 2 (0x8 in Rage Wars) */
typedef struct { CAITarget a; CAITarget b; } B04C;

/* A separate function in Rage Wars; inlined here in Turok 2. */
static inline void fill(CAITarget *d, B04A *a) {
    CVector3 p = a->pos;
    CVector3 z;
    z.x = 0.0f;
    z.y = 0.0f;
    z.z = 0.0f;
    d->m_Kind = 3;
    d->m_pTarget = 0;
    d->m_Angle = 0;
    d->m_vPos = p;
    d->m_vDelta = z;
    d->m_Dist = 0;
    z.y = 0.0f;
    p.y = 0.0f;
    d->m_vFlatPos = p;
    d->m_vFlatDelta = z;
    d->m_FlatDist = 0;
}

void AI_UpdateTargets(B04A *a, CAI *b, B04C *c, s32 f1, s32 f2) {
    f32 t;
    if (b->m_TargetOverrideTime > 0.0f) {
        t = b->m_TargetOverrideTime - gFrameIncrement;
        b->m_TargetOverrideTime = t;
        if (t <= 0.0f) {
            b->m_pTargetOverride = 0;
            b->m_TargetOverrideTime = 0.0f;
        }
    }
    if (f1 != 0)
        AI_GetPathTargetInfo((CGameObjectInstance *) a, b, b->m_pPathTarget, &c->b);
    else
        fill(&c->b, a);
    if (f2 != 0) {
        AI_GetSightTargetInfo((CGameObjectInstance *) a, b, b->m_pSightTarget, &c->a);
        return;
    }
    fill(&c->a, a);
}

/* The signed angle from the actor's heading to `point`, in the XZ plane. */
f32 AI_AngleToPoint(CGameObjectInstance *actor, CVector3 point) {
    f32 dx;
    f32 dz;
    f32 dist;
    f32 s;
    f32 c;
    f32 dot;
    f32 angle;

    dx = point.x - actor->header.m_vPos.x;
    dz = point.z - actor->header.m_vPos.z;
    dist = func_00298470(dx * dx + dz * dz);
    if (dist == 0.0f) {
        return 0.0f;
    }
    do {
        s = func_002982F0(actor->m_RotY);
    } while (0);
    c = -func_002974C0(actor->m_RotY);
    dot = (dx * -s + dz * c) / dist;
    dot = ((((dot) > (1.0f) ? (1.0f) : (dot))) < ((-1.0f)) ? ((-1.0f)) : (((dot) > (1.0f) ? (1.0f) : (dot))));
    angle = Math_AcosLookup(dot);
    if (dx * c + dz * s > 0.0f) {
        return angle;
    }
    return -angle;
}

/* Turns the object by arg1 and wraps its angle. */
void Actor_TurnBy(CGameObjectInstance *arg0, f32 arg1) {
    arg0->m_RotY = (f32) (arg0->m_RotY + arg1);
    Angle_Wrap(&arg0->m_RotY);
}

#include "globals/pickup.h"

/* Drops, once (the first time `bit` is set in *flags), a pickup for each mask bit of m1 in
 * sDropPickups and of m2 in sDropPickups2, around the object. */
void AI_DropPickupsOnce(CInstanceHeader *o, u32 *flags, u32 bit, u32 m1, u32 m2) {
    CDropPickup *e;
    if (!(*flags & bit)) {
        *flags |= bit;
        for (e = sDropPickups; e->m_Mask != 0; e++) {
            if (m1 & e->m_Mask)
                GameObject_DropPickupAt(o, o->m_vPos, e->m_Id, Random_Blend(450.0f, 600.0f));
        }
        for (e = sDropPickups2; e->m_Mask != 0; e++) {
            if (m2 & e->m_Mask)
                GameObject_DropPickupAt(o, o->m_vPos, e->m_Id, Random_Blend(450.0f, 600.0f));
        }
    }
}

/* Two words set to the same value (a current and an initial value). */
typedef struct {
    /* 0x00 */ s32 value;
    /* 0x04 */ s32 initial;
} ValuePair;

void ValuePair_Set(ValuePair *arg0, s32 arg1) {
    arg0->value = arg1;
    arg0->initial = arg1;
}

#include "m2c_macros.h"

typedef struct { s16 *base; s16 *p; } S;

s16 AI_NextInCycle(S *arg0) {
    s16 v = *arg0->p++;

    if (*arg0->p == -1) {
        arg0->p = arg0->base;
    }
    return v;
}

/* Calls the AI's hurt callback, if any. */
/* ABI (docs/port/ABI.md section 5, abicheck indirect missing_args): every caller passes (actor, ai,
 * damage event) and MIPS hands the hurt callback those three registers (pass-through); the host
 * passes them, with the actor as a full pointer. */
void CAI__CallHurt(void *arg0, CAI *arg1, void *arg2) {
    void (*cb)(void *, CAI *, void *) = (void (*)(void *, CAI *, void *))arg1->m_pfHurt;

    if (cb != NULL) {
        cb(arg0, arg1, arg2);
    }
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00217224.c, func_00217224.
 * The same code in Turok 2, on CGameObjectInstance (Rage Wars' B04Obj view: the position at 0x4,
 * the height at 0x54, m_CollisionYOffset). */

/* The squared distance from the object's collision centre (its position raised by
 * m_CollisionYOffset) to `v`. */
f32 AI_DistSqToPointAtHeight(CGameObjectInstance *o, CVector3 v) {
    CVector3__Subtract(&v, &v, &o->header.m_vPos);
    v.y = v.y - o->m_CollisionYOffset;
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80213ED4_22.CPP, func_80217290_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* Turok 2: actor position at 0x4 (Rage Wars 0x8). */
/* Squared horizontal (XZ) distance from the actor to point. */
f32 AI_FlatDistSqToPoint(CGameObjectInstance *arg0, CVector3 point)
{
    f32 dx = point.x - arg0->header.m_vPos.x;
    f32 dz = point.z - arg0->header.m_vPos.z;
    return dx * dx + dz * dz;
}

#include "structs/particle.h"

/* Spawns a particle of `kind` from `obj` at `at`, moving with it and oriented like it: a game
 * object (type 1) gives its velocity, rotation and sight target (m_AI.m_pSightTarget), a particle
 * (type 3) its velocity and ground rotation. One body for the N64 and the port since round 29
 * (the N64 view, SpawnObj, had the velocity at 0x18 and the target at 0x1C0, which missed the
 * host members, so the port had its own copy). */
void Actor_SpawnParticle(CInstanceHeader *obj, s32 kind, CVector3 at, s32 unused) {
    CVector3 pos;
    CVector3 vel;
    CQuaternion rot;
    CGameRegion *region;
    s32 mode;
    sptr target;

    mode = 1;
    GameObject_TraceMove(obj, at, &pos, &region, 0, 1);
    target = 0;
    switch (obj->m_Type) {
    case 0:
    case 2:
        vel.x = vel.y = vel.z = 0.0f;
        rot.x = rot.y = rot.z = 0.0f;
        rot.w = 1.0f;
        break;
    case 1:
        vel = ((CGameObjectInstance *) obj)->m_vVelocity;
        rot = CGameObjectInstance__GetRotation((CGameObjectInstance *) obj);
        target = (sptr) ((CGameObjectInstance *) obj)->m_AI.m_pSightTarget;
        if (CGameObjectInstance__GetGeometryKind(obj) == 5) {
            mode = 3;
        }
        break;
    case 3:
        vel = ((CParticle *) obj)->m_vVelocity;
        rot = Actor_GetGroundRotation(obj);
        break;
    }
    CParticleSystem__Spawn(&gScene.m_ParticleSystem, obj, obj, NULL, target, kind, vel, rot, pos, region, -1, mode);
}

void func_00246854(s32 arg0, s32 arg1, CVector3 arg2) {
}

#include "structs/level_actor.h"
#include "structs/sound.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002172D0.c, func_002172D0.
 * The same code in Turok 2, on Turok 2's types: the AI's looping sound is m_pAttachedLoopingSound
 * (a SndSource; its id is SndSource.sound, 0xC). One body for the N64 and the port since round 29
 * (the N64 view read the N64 CAI 0xEC, so the port had its own copy). The owner of a level actor's
 * sounds (type 0, a CStaticObject) is its word at 0x80, m_LitKey, as the ROM reads it. */
void AI_StartSound(CInstanceHeader *src, CAI *owner, s32 id) {
    sptr s0 = -1;

    if (owner->m_pAttachedLoopingSound != NULL) {
        if (((SndSource *) owner->m_pAttachedLoopingSound)->sound.w == id) {
            return;
        }
        LoopSounds_Stop(LoopSounds_Get(), owner->m_pAttachedLoopingSound);
    }
    switch (src->m_Type) {
    case 1:
    case 2:
        s0 = (sptr) src;
        break;
    case 0:
        s0 = ((CStaticObject *) src)->m_LitKey;
        break;
    }
    owner->m_pAttachedLoopingSound = LoopSounds_Start(LoopSounds_Get(), id, &src->m_vPos, &src->m_vPos, s0);
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/TEAM/80217388.CPP, func_80217388_de.
 * The same code in Turok 2, on Turok 2's types (one body for the N64 and the port since round 29):
 * stops the AI's looping sound and every sound the object owns (a static object, type 0, owns its
 * sounds through its word at 0x80, see AI_StartSound). */
void AI_StopSound(CGameObjectInstance *obj, CAI *ai) {
    sptr owner = 0;

    switch (obj->header.m_Type) {
    case 1:
    case 2:
        owner = (sptr) obj;
        break;
    case 0:
        owner = ((CStaticObject *) obj)->m_LitKey;
        break;
    }
    if (ai->m_pAttachedLoopingSound != NULL) {
        Sound__StopOwner(owner);
        if (ai->m_pAttachedLoopingSound != NULL) {
            LoopSounds_Stop(LoopSounds_Get(), ai->m_pAttachedLoopingSound);
            ai->m_pAttachedLoopingSound = NULL;
        }
    }
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0021740C.c, func_0021740C.
 * The same code in Turok 2, on Turok 2's types: the new position goes to GameObject_TraceMove as
 * one CVector3 by value (round 29: Rage Wars' view passed three floats, which the port had to
 * route through GameObject_TraceMove_f3). */

/* Pushes `b` out of `a`'s way, away from `c` (or in place without one): to the sum of the two
 * collision radii (plus 51.2) from `c`, traced from b's position into *outPos / *outRegion. */
void AI_PushAway(CGameObjectInstance *a, s32 unused, CGameObjectInstance *b, CGameObjectInstance *c,
                 CVector3 *outPos, CGameRegion **outRegion) {
    CVector3 d;
    CVector3 r;
    if (c != NULL) {
        CVector3__Subtract(&d, &b->header.m_vPos, &c->header.m_vPos);
        d.y = 0;
        CVector3__Normalize(&d);
    } else {
        d.x = 0;
        d.y = 0;
        d.z = 0;
    }
    CVector3__Scale(&d, &d, GameObject_GetCollisionRadius(b) + GameObject_GetCollisionRadius(a) + 51.199997f);
    CVector3__Add(&r, &b->header.m_vPos, &d);
    GameObject_TraceMove(&b->header, r, outPos, outRegion, 0, 0);
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002174E4.c, func_002174E4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct VecSrc {
    u8 pad0[4]; /* pos at 0x4 */
    CVector3 v;
} VecSrc;

void AI_SetTargetPoint(VecSrc *src, s32 unused, CAITarget *dst) {
    CVector3 a;
    CVector3 b;
    a = src->v;
    b.x = 0;
    b.y = 0;
    b.z = 0;
    dst->m_Kind = 3;
    dst->m_pTarget = 0;
    dst->m_Angle = 0;
    dst->m_vPos = a;
    dst->m_vDelta = b;
    dst->m_Dist = 0;
    b.y = 0;
    a.y = 0;
    dst->m_vFlatPos = a;
    dst->m_vFlatDelta = b;
    dst->m_FlatDist = 0;
}

#include "structs/pickup.h"

/* Drops a pickup for each entry of the 0-ended table `e` whose mask meets `mask`. */
void AI_DropPickupsByMask(CInstanceHeader *o, s32 a1, u32 mask, CDropPickup *e) {
    for (; e->m_Mask != 0; e++) {
        if (mask & e->m_Mask) {
            GameObject_DropPickupAt(o, o->m_vPos, e->m_Id, Random_Blend(4.5e2f, 6.0e2f));
        }
    }
}
