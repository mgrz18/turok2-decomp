/* WRONG CUT: splat's func_0025AD88 starts two orphan words early (0x25AD88 `sw $t0, 0x20($sp)`,
 * 0x25AD8C `bnez $v0, .L0025B284`, whose delay slot is this function's first instruction).
 * The real function is AI_TurnHeadToTarget (0x2E8 bytes); this C matches it from 0x25AD90 on. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "globals/ai.h"
#include "protos/iset.h"
#include "protos/cache.h"
#include "protos/enemy_ai.h"

/* AI head tracking: turns the head bones toward the target (or the player in state 0xC). */

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s8 neckBone;
    /* 0x19 */ s8 headBone;
    /* 0x1A */ s8 boneCount;
} AnimObjectInfo;
extern f32 CVector3__GetPitch(CVector3 *v);
extern void Math_EaseToward(f32 *value, f32 target, f32 rate);

void AI_TurnHeadToTarget(CGameObjectInstance *actor, CAI *ai, f32 pitchOffset) {
    CVector3 from;
    CVector3 to;
    CVector3 delta;
    f32 value;
    sptr *res;
    AnimObjectInfo *info;
    s32 count;
    f32 yaw;
    f32 pitch;
    f32 n;
    CGameObjectInstance *target;

    res = CCache__Request(0, actor->m_rpObject, actor->m_rpObjectHeader, 0, 0, 0, "anim object info", 1);
    if (res == NULL) {
        return;
    }
    info = CISet__GetBlock(*res, 0);
    if (info->headBone != -1 && info->neckBone != -1 && (count = info->boneCount) != 0) {
        if ((ai->m_ModeFlags & 2) && ai->m_Health != 0 && (ai->m_pSightTarget != NULL || ai->m_Mode == 0xC)) {
            from.x = actor->header.m_vPos.x;
            from.y = actor->header.m_vPos.y + GameObject_GetCollisionHeight(actor) * 0.9f + actor->m_CollisionYOffset;
            from.z = actor->header.m_vPos.z;
            if (ai->m_Mode == 0xC) {
                to = gEnemyAlertPoint;
            } else {
                target = ai->m_pSightTarget;
                to.x = target->header.m_vPos.x;
                to.y = target->header.m_vPos.y + GameObject_GetCollisionHeight(target) * 0.9f + target->m_CollisionYOffset;
                to.z = target->header.m_vPos.z;
            }
            yaw = -AI_AngleToPoint(actor, to);
            if (yaw < -1.5707965f) {
                yaw = -1.5707965f;
            } else if (yaw > 1.5707965f) {
                yaw = 1.5707965f;
            }
            CVector3__Subtract(&delta, &to, &from);
            pitch = CVector3__GetPitch(&delta);
            if (pitch < -1.5707965f) {
                pitch = -1.5707965f;
            } else if (pitch > 1.5707965f) {
                pitch = 1.5707965f;
            }
        } else {
            pitch = yaw = 0.0f;
        }
        n = count;
        value = ai->m_HeadTurn * n;
        Math_EaseToward(&value, yaw, 0.25f);
        ai->m_HeadTurn = value / n;
        value = ai->m_HeadTilt * n;
        Math_EaseToward(&value, pitch + pitchOffset, 0.25f);
        ai->m_HeadTilt = value / n;
    }
    CCache__Unlock(0, res);
}

#include "structs/player.h"
#include "globals/game.h"

/* Picks an actor's death animation from what killed it (weapon, projectile, hit direction). */


typedef struct {
    /* 0x00 */ u8 pad0[0x65];
    /* 0x65 */ u8 direction;
} HitPoint;
typedef struct {
    /* 0x00 */ CGameObjectInstance *source;
    /* 0x04 */ u8 pad4[0xC - 0x4];
    /* 0x0C */ s32 flags;
    /* 0x10 */ HitPoint *point;
} HitInfo;
extern s32 Random_Next(void); /* rand */

s32 EnemyAI_PickDeathMode(CGameObjectInstance *actor, sptr unused, HitInfo *hit) {
    CEnemyIntelligence *anims;
    CGameObjectInstance *src;
    s32 front = 0;
    s32 side = 0;
    s32 back = 0;
    s32 blast = 0;

    anims = &actor->header.m_pIntelligence->m_Enemy;
    if (gEngineApp_m_Settings.unk17 != 0 && (src = hit->source) != NULL) {
        switch (src->header.m_Type) {
            case 2:
                switch (src->header.m_nObjType) {
                    case 0xC:
                    case 0x22:
                    case 0x23:
                    case 0x24:
                    case 0x57:
                    case 0x58:
                        blast = 1;
                        break;
                    case 0x2:
                    case 0x6:
                    case 0x7:
                    case 0x8:
                    case 0x9:
                    case 0x1A:
                    case 0x4F:
                    case 0x68:
                        back = side = front = 1;
                        break;
                }
                break;
            case 1:
                if ((src->m_dwFlags & 0x300000) && ((CPlayer *)src->m_AI.m_pParent)->m_Data.m_CurrentWeapon == 1) {
                    back = side = front = 1;
                }
                break;
        }
    }
    if (blast && anims->m_BlastDeathModel != -1) {
        return 0x37;
    }
    if (Random_Next() % 1000 < 500) {
        back = side = front = 0;
    }
    if (hit->point != NULL) {
        switch (hit->point->direction & 3) {
            case 0:
                if (front && anims->m_FrontDeathModel != -1) {
                    return 0x3B;
                }
                break;
            case 1:
                if (side && anims->m_SideDeathModel != -1) {
                    return 0x38;
                }
                break;
            case 2:
                if (back) {
                    if (!(hit->point->direction & 4)) {
                        if (anims->m_BackDeathModel != -1) {
                            return 0x39;
                        }
                    } else if (anims->m_BackDeathModel2 != -1) {
                        return 0x3A;
                    }
                }
                break;
        }
    }
    if (gEngineApp_m_Settings.unk17 != 0 && (hit->flags & 0x800000) && anims->m_ExplodeDeathModel != -1) {
        if (CGameObjectInstance__PickRandomAnim(actor, anims->m_GibModel + 1, -1) != -1) {
            return 0x35;
        }
        return 0x36;
    }
    return 0x34;
}

#include "structs/intelligence.h"
#include "structs/collision.h"
#include "globals/scene.h"

typedef struct Object Object;
/* An object or a particle: the first byte tells which. */
#ifdef TARGET_PORT
#include "structs/particle.h"
struct Object {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py; round 8 w2), the
 * particle owner at CParticle's: each field in its own member of one union, placed by
 * __builtin_offsetof of the member it reads */
#define T2_HOFF_Object(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_kind[T2_HOFF_Object(header.m_Type)]; u8 kind; }; /* N64 0x0 */
        struct { u8 _hp_particleType[T2_HOFF_Object(header.m_nObjType)]; u16 particleType; }; /* N64 0x2 */
        struct { u8 _hp_pos[T2_HOFF_Object(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_def[T2_HOFF_Object(header.m_pIntelligence)]; CIntelligence *def; }; /* N64 0x14 */
        struct { u8 _hp_typeId[T2_HOFF_Object(m_nTypeFlag)]; u16 typeId; }; /* N64 0xB8 */
        struct { u8 _hp_flags[T2_HOFF_Object(m_dwFlags)]; u32 flags; }; /* N64 0xD4 */
        struct { u8 _hp_particleOwner[__builtin_offsetof(CParticle, m_pOwner)]; Object *particleOwner; }; /* N64 0x10C: a particle's (kind 2) */
        struct { u8 _hp_ai[T2_HOFF_Object(m_AI)]; CAI ai; }; /* N64 0x140 */
        struct { u8 _hp_fxFlags[T2_HOFF_Object(m_dwHotPointFlags)]; u32 fxFlags; }; /* N64 0x27C */
    };
};
#else
struct Object {
    /* 0x000 */ u8 kind;            /* 1: object, 2: particle */
    /* 0x001 */ u8 pad1;
    /* 0x002 */ u16 particleType;
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ CIntelligence *def;
    /* 0x018 */ u8 pad18[0xB8 - 0x18];
    /* 0x0B8 */ u16 typeId;
    /* 0x0BA */ u8 padBA[0xD4 - 0xBA];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x10C - 0xD8];
    /* 0x10C */ Object *particleOwner;
    /* 0x110 */ u8 pad110[0x140 - 0x110];
    /* 0x140 */ CAI ai;
    /* 0x27C */ u32 fxFlags;
};
#endif
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define AI_SetTargetOverride AI_SetTargetOverride_f
#endif

void AI_SetTargetOverride(Object *obj, CAI *ai, Object *owner, f32 time);
void Sound__PlayPitched(s16 sound, CVector3 pos, s32, sptr, f32 volume);
s32 Random_Next(void);
void TotemDefense_AddKill(void);
s32 EnemyAnim_PickDeath(CGameObjectInstance *actor, CAI *state);
/* An object's owner (0x1A8) lies inside its AI block. */
#define OWNER(o) ((*(Object **)&((CGameObjectInstance *)(o))->m_AI.m_pParent))

/* AI damage: retarget the attacker, take the health, play the hurt sound and
 * pick the death, fall or flinch reaction; a knock back turns the actor to
 * face the push when the source was near enough. */
void EnemyAI_OnDamage(Object *obj, CAI *ai, DamageEvent *dmg) {
    CVector3 delta;
    s32 pushed;
    Object *attacker;
    Object *source;
    Object *owner;
    Object *newTarget;
    s32 left;
    CEnemyIntelligence *info;
    CIntelligence *def;
    f32 volume;
    s32 index;
    s32 health;
    s32 dead;
    s32 justDied;
    s32 anim;
    s32 node;
    sptr slot;
    HitFrame *region;

    attacker = NULL;
    pushed = 0;
    def = obj->def;
    volume = 1.0f;
    index = ai->m_Enemy.m_BehaviorIndex;
    info = &def->m_Enemy;
    if (ai->m_Mode == 0x3D) {
        EnemyAI_NukedFrozenRemove(obj, ai);
        return;
    }
    source = dmg->source;
    if (source != NULL) {
        switch (source->kind) {
            case 1:
                if ((source->flags & 0x300000) && (u32)(gScene_m_LevelID - 7100) <= 400) {
                    newTarget = OWNER(source);
                    if (obj->ai.m_pSightTarget != newTarget) {
                        AI_SetTargetOverride(obj, ai, newTarget, 60.0f);
                        ai->m_dwFlags &= ~0x40000000;
                    }
                }
                attacker = source;
                AI_HearNoise(obj, ai, &source->pos, 3);
                break;
            case 2:
                if (source->particleType == 10) {
                    if ((def->m_Enemy.m_dwFlags & 0x80) && ai->m_Health != 0) {
                        CAI__SetMode(obj, ai, 0x1F);
                    }
                    if (info->m_dwFlags & 0x40000) {
                        dmg->damage += 0x3200;
                        ai->m_OnFireTimer += 60.0f;
                    }
                }
                owner = source->particleOwner;
                if (owner != NULL) {
                    if ((owner->flags & 0x300000) && (u32)(gScene_m_LevelID - 7100) <= 400) {
                        newTarget = OWNER(owner);
                        if (ai->m_pSightTarget != newTarget) {
                            AI_SetTargetOverride(obj, ai, newTarget, 60.0f);
                            ai->m_dwFlags &= ~0x40000000;
                        }
                    }
                    AI_HearNoise(obj, ai, &source->pos, 3);
                    if (owner->kind == 1) {
                        attacker = owner;
                    }
                }
                break;
            default:
                AI_HearNoise(obj, ai, &source->pos, 3);
                break;
        }
        if (!gScene.m_bStreamAll && attacker != NULL && attacker->typeId != obj->typeId) {
            if (attacker->def->m_Type == 1) {
                ai->m_pSightTarget = attacker;
                ai->m_pPathTarget = attacker;
                ai->m_dwFlags |= 0x20000000;
                ai->m_dwFlags |= 0x40000000;
            } else {
                ai->m_dwFlags &= ~0x20000000;
                ai->m_dwFlags &= ~0x40000000;
            }
        }
    } else {
        AI_HearNoise(obj, ai, &gEnemyAlertPoint, 3);
    }
    health = ai->m_Health;
    left = health - dmg->damage;
    if (left < 0) {
        left = 0;
    }
    ai->m_Health = left;
    dead = left == 0;
    justDied = 0;
    if (dead) {
        justDied = health != 0;
    }
    if ((health != 0 || (ai->m_Mode != 0x3C && !ai->m_AnimRepeatedCount)) && info->m_PainSound != 0 && ai->m_LastHurtSFXTimer >= 11) {
        if (gEngineApp_m_Settings.cheatFlags & 0x40) {
            volume = 2.5f;
        } else if (gEngineApp_m_Settings.cheatFlags & 0x20) {
            volume = 0.35f;
        }
        Sound__PlayPitched(info->m_PainSound, obj->pos, 0, -1, volume);
        ai->m_LastHurtSFXTimer = 0;
    }
    ai->m_Aggression = (ai->m_Aggression + dmg->damage < 255) ? 255 : ai->m_Aggression + dmg->damage;
    if ((dmg->flags & 0x400000) && Random_Next() % 1000 < 500) {
        dmg->flags = 0x800000;
    }
    if ((dmg->flags & 0x100000) && Random_Next() % 1000 < 500) {
        dmg->flags = 0x200000;
    }
    ai->m_dwFlags &= 0xFC0FFFFF;
    ai->m_dwFlags |= dmg->flags;
    if (justDied) {
        AI_StopSound(obj, ai);
        obj->fxFlags |= 8;
        anim = EnemyAI_PickDeathMode(obj, ai, dmg);
        if (anim == 0x34 && (dmg->flags & 0x400000)) {
            pushed = 1;
        }
        CAI__SetMode(obj, ai, anim);
        TotemDefense_AddKill();
    } else if (dead) {
        AI_StopSound(obj, ai);
        if (ai->m_Mode != 0x35 && ai->m_Mode != 0x3C && ai->m_AnimRepeatedCount && (dmg->flags & 0xC00000)) {
            ai->m_dwFlags = (ai->m_dwFlags & 0xFC0FFFFF) | 0x400000;
            node = EnemyAnim_PickDeath(obj, ai);
            if (node != -1) {
                slot = CGameObjectInstance__PickRandomAnim(obj, node, -1);
                if (slot != -1) {
                    pushed = 1;
                    ai->m_AnimType = node;
                    ai->m_AnimIndex = slot;
                    ai->m_AnimRepeatedCount = 0;
                    ai->m_ModeTime = 0;
                }
            }
        }
    } else {
        region = dmg->frame;
        if (region != NULL && !(ai->m_Mode >= 0x1C && ai->m_Mode < 0x23) && (region->flags & 3) == 2) {
            if (!(region->flags & 4)) {
                if ((index == 0 && (obj->def->m_Enemy.m_wLeftArmWoundAnimFlags & 0x1F)) || (index == 1 && (obj->def->m_Enemy.m_wLeftArmWoundAnimFlags & 0x3E0))
                    || (index == 2 && (obj->def->m_Enemy.m_wLeftArmWoundAnimFlags & 0x7C00))) {
                    CAI__SetMode(obj, ai, 0x24);
                    return;
                }
            } else {
                if ((index == 0 && (obj->def->m_Enemy.m_wRightArmWoundAnimFlags & 0x1F)) || (index == 1 && (obj->def->m_Enemy.m_wRightArmWoundAnimFlags & 0x3E0))
                    || (index == 2 && (obj->def->m_Enemy.m_wRightArmWoundAnimFlags & 0x7C00))) {
                    CAI__SetMode(obj, ai, 0x25);
                    return;
                }
            }
        }
        if (ai->m_AnimRepeatedCount && (ai->m_Mode == 0x11 || ai->m_Mode == 0x12)) {
            CAI__SetMode(obj, ai, 4);
        } else if (ai->m_Mode != 0x33 && (dmg->flags & 0xC00000) && (info->m_dwFlags & 0x20000)) {
            CAI__SetMode(obj, ai, 0x33);
            pushed = 1;
        }
    }
    if (pushed && source != NULL && dmg->radius > 0.0f) {
        CVector3__Subtract(&delta, &obj->pos, &source->pos);
        if (func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z) < dmg->radius) {
            CVector3__GetYaw(&delta);
        }
    }
}

#include "structs/model.h"
#include "globals/engine.h"
#include "structs/light.h"

/* AI draw callback: draws the actor faded in or out (dying / teleporting states). */


extern void CGameObjectInstance__DrawModel(CGameObjectInstance *actor, CAI *ai, CDrawArgs *params);
extern void Render_DrawModelImmediate(void *arg0, s32 renderMode, sptr model, sptr matrix, s32 projected, void *lights, sptr segment, s32 group, s32 arg8);

void EnemyAI_DrawFaded(CGameObjectInstance *actor, CAI *ai, CDrawArgs *params) {
    u8 alpha;

    if (ai->m_Mode == 0x20) {
        CGameObjectInstance__DrawModelImmediate(actor, ai, params);
        /* the port (round 9 x1): N64 byte 0x1B is the low byte of the big-endian m_nModel; on the
         * host that byte is in m_nModel's neighbour (the totem defense's teleporting enemies crashed
         * drawing with a model index read from the wrong byte) */
        Render_DrawModelShared(params->m_hModel, actor->m_pmtDrawMtxs, 1, &actor->m_Lights[gEvenOdd], 0, (s8) params->m_nModel, -1);
        return;
    }
    if (ai->m_Enemy.m_Timer > 0.0f) {
        if (ai->m_Mode == 0x19) {
            alpha = ai->m_AnimRepeatedCount * 255 / 4;
        } else {
            alpha = 255;
            if (ai->m_Enemy.m_Timer < 30.0f) {
                alpha = ai->m_Enemy.m_Timer * 8.5f;
            }
        }
        if (alpha != 255) {
            gBlendState = 1;
            gBlendState_m_Color[3] = 255 - alpha;
            CGameObjectInstance__DrawModelImmediate(actor, ai, params);
        }
        if (alpha != 0) {
            gBlendState_m_Color[3] = alpha;
            gBlendState = 1;
            Render_DrawModelImmediate(((u8 *) &gScene.m_Geom[4]), -1, params->m_hModel, actor->m_pmtDrawMtxs, 1, &actor->m_Lights[gEvenOdd], 0,
                          params->m_nModel, actor->m_AI.m_nTexture);
        }
        gBlendState = 0;
    } else {
        CGameObjectInstance__DrawModel(actor, ai, params);
    }
}

#include "protos/scene.h"

void EnemyAI_Update(CGameObjectInstance *obj, CAI *ai);
void EnemyAI_OnHit(CGameObjectInstance *actor, CAI *ai, CParticle *proj);
void EnemyAI_CheckGrab(CGameObjectInstance *arg0, CAI *arg1);
extern void EnemyAI_HeadNodeCallback();

void EnemyAI_Init(CGameObjectInstance *actor, CAI *ai) {
    CEnemyIntelligence *info;
    s32 mode2;
    s32 one; /* the mode indices live in variables: constants do not match */
    s32 two;

    actor->m_dwFlags |= 0x02000000;
    actor->m_dwFlags |= 0x04000000;
    info = &actor->header.m_pIntelligence->m_Enemy;
    /* read through a one-byte struct: the plain u8 load is scheduled earlier than the ROM's */
    if (((struct { u8 value; } *) &gEngineApp_m_Settings_multiplayer)->value == 0) {
        actor->m_dwFlags |= 0x10000000;
    }
    switch (actor->m_nTypeFlag) {
    case 0x44E:
    case 0x450:
    case 0x451:
    case 0x453:
    case 0x454:
    case 0x455:
    case 0x456:
        return;
    }
    ai->m_pModeTable = gEnemyGroundModeTable;
    ai->m_pfStep = EnemyAI_Update;
    ai->m_pfCalculateOrientationMatrix = EnemyAI_CalcOrientationMatrix;
    ai->m_pfDraw = EnemyAI_DrawFaded;
    if (actor->m_nTypeFlag == 0x136) {
        ai->m_pfHit = NULL;
        ai->m_pfCollision = NULL;
        ai->m_pfHurt = NULL;
        CScene__SetInstanceSavedBit(&gScene, actor, 0);
    } else {
        ai->m_pfHit = EnemyAI_OnHit;
        ai->m_pfCollision = EnemyAI_CheckGrab;
        ai->m_pfHurt = EnemyAI_OnDamage;
    }
    ai->m_Enemy.m_Pitch = 0;
    ai->m_Enemy.m_Roll = 0;
    ai->m_Enemy.m_pGrabbed = NULL;
    ai->m_Enemy.m_TurnRate = 0;
    ai->m_Enemy.m_AttackDelay = 0;
    ai->m_Enemy.unk18 = 0;
    ai->m_Enemy.m_WanderTimer = 0;
    ai->m_Enemy.m_BaseScale = 1.0f;
    ai->m_Enemy.m_IdleTimer = Random_Blend(60.0f, 120.0f);
    ai->m_Enemy.m_Timer = 0;
    ai->m_Enemy.m_Timer2 = 150.0f;
    ai->m_Enemy.m_Behavior[0] = info->m_Behaviors[0];
    one = 1;
    ai->m_Enemy.m_Behavior[one] = info->m_Behaviors[one];
    ai->m_Enemy.m_Behavior[2] = mode2 = info->m_UnderwaterBehavior;
    if (ai->m_Enemy.m_Behavior[0]) {
        ai->m_Enemy.m_BehaviorIndex = 0;
    } else if (ai->m_Enemy.m_Behavior[one]) {
        ai->m_Enemy.m_BehaviorIndex = one;
    } else {
        two = 2;
        if (mode2) {
            ai->m_Enemy.m_BehaviorIndex = two;
        } else {
            ai->m_Enemy.m_BehaviorIndex = 0;
        }
    }
    ai->m_pfEffects = EnemyAI_HeadNodeCallback;
    if (!(actor->m_dwFlags & 0x80000) && (info->m_dwFlags & 0x1000)) {
        actor->m_dwFlags &= ~0x2000;
        actor->m_dwFlags &= ~0x100;
    }
    if (!(info->m_dwFlags & 0x80000)) {
        actor->m_dwFlags |= 0x01000000;
    }
}

/* AI init dispatcher: the 0x44E..0x456 types have their own init in the paged
 * VM code; everything else gets the generic init. */


extern void MantidQueen_InitAI(CGameObjectInstance *actor, CAI *ai);
extern void MotherBoss_Init(CGameObjectInstance *actor, CAI *ai);
extern void Primagen_InitAI(CGameObjectInstance *actor, CAI *ai);
extern void BlindOne_BossInit(CGameObjectInstance *actor, CAI *ai);
extern void BlindOne_Group1Init(CGameObjectInstance *actor, CAI *ai);
extern void BlindOne_Group2Init(CGameObjectInstance *actor, CAI *ai);
extern void BlindOne_Group3Init(CGameObjectInstance *actor, CAI *ai);

void EnemyAI_InitDispatch(CGameObjectInstance *actor, CAI *ai) {
    u32 *defFlags = &actor->header.m_pIntelligence->m_dwTypeFlags;

    switch (actor->m_nTypeFlag) {
    case 0x451:
        MantidQueen_InitAI(actor, ai);
        break;
    case 0x44E:
        MotherBoss_Init(actor, ai);
        break;
    case 0x453:
        Primagen_InitAI(actor, ai);
        break;
    case 0x450:
        BlindOne_BossInit(actor, ai);
        break;
    case 0x454:
        BlindOne_Group1Init(actor, ai);
        break;
    case 0x455:
        BlindOne_Group2Init(actor, ai);
        break;
    case 0x456:
        BlindOne_Group3Init(actor, ai);
        break;
    default:
        if (actor->m_nTypeFlag == 0x467) {
            CAI__SetMode(actor, ai, 0x31);
        } else if (actor->m_dwFlags & 0x80000) {
            CAI__SetMode(actor, ai, 2);
        } else {
            CAI__SetMode(actor, ai, 0);
        }
        ai->m_Action = 0;
        break;
    }
    if (!(*defFlags & 0x8000)) {
        ai->m_Action = 1;
        AI_HearNoise(actor, ai, &actor->header.m_vPos.x, 3);
    }
    if (CScene__GetInstanceSavedBit(&gScene, actor) == 1) {
        actor->m_dwFlags &= ~0x100;
    }
}

#include "globals/math.h"

void Projectile_PlaceAtMuzzle(CGameObjectInstance *self, CAI *ai);
extern f32 Math_DecayToZero(f32 angle, f32 step);
extern void CMtxF__SetRotateZ(CMtxF *m, f32 angle);
extern void CMtxF__MultiplyAffine(CMtxF *out, CMtxF *a, CMtxF *b);
extern void CMtxF__ScaleAxes(CMtxF *m, f32 x, f32 y, f32 z);
extern void CVector3__ClampLength(CVector3 *v, f32 limit);
extern void CMtxF__Translate(CMtxF *m, f32 x, f32 y, f32 z);
extern void CMtxF__SwapYZRows(CMtxF *m);

void EnemyAI_CalcOrientationMatrix(CGameObjectInstance *actor, CAI *ai) {
    CMtxF look;
    CMtxF rotMtx;
    CQuaternion spin;
    CQuaternion rot;
    f32 sin;

    if (ai->m_ModeFlags & 0x10000) {
        Projectile_PlaceAtMuzzle(actor, ai);
    }
    if (ai->m_Health == 0) {
        ai->m_Enemy.m_Pitch = Math_DecayToZero(ai->m_Enemy.m_Pitch, 0.0872664749f);
        ai->m_Enemy.m_Roll = Math_DecayToZero(ai->m_Enemy.m_Roll, 0.0872664749f);
    }
    CMtxF__SetRotateZ(&look, ai->m_Enemy.m_Roll);
    CMtxF__RotateX(&look, ai->m_Enemy.m_Pitch);
    sin = func_002982F0(actor->m_RotY * 0.5f);
    spin.x = 0.0f;
    spin.y = sin;
    spin.z = 0.0f;
    gQuaternionSin = sin;
    spin.w = func_002974C0(actor->m_RotY * 0.5f);
    CQuaternion__Multiply(&rot, &spin, &actor->m_qGround);
    CQuaternion__ToMatrix(&rot, &rotMtx);
    CMtxF__MultiplyAffine((CMtxF *) &actor->m_mfOrient, &look, &rotMtx);
    if (gEngineApp_m_Settings.cheatFlags & 0x40) {
        CMtxF__ScaleAxes((CMtxF *) &actor->m_mfOrient, actor->m_vScale.x * 0.5f, actor->m_vScale.y * 0.5f, actor->m_vScale.z * 0.5f);
    } else {
        CMtxF__ScaleAxes((CMtxF *) &actor->m_mfOrient, actor->m_vScale.x, actor->m_vScale.y, actor->m_vScale.z);
    }
    CVector3__ClampLength(&actor->header.m_vPos, 20000.0f);
    CMtxF__Translate((CMtxF *) &actor->m_mfOrient, actor->header.m_vPos.x, actor->header.m_vPos.y, actor->header.m_vPos.z);
    CMtxF__SwapYZRows((CMtxF *) &actor->m_mfOrient);
}

/* AI idle-sound callback: while the actor moves fast the timer stays reset;
 * otherwise it counts down and, once idle in state 4, plays the ambient
 * animation (0x26 or 0x28 depending on the mode) every 30-75 frames. */

typedef struct {
    /* 0x00 */ u8 pad0[0x60];
    /* 0x60 */ f32 speed;
} Motion;

void EnemyAI_IdleSoundCallback(CGameObjectInstance *actor, CAI *ai, Motion *motion) {
    s32 modeIndex = ai->m_Enemy.m_BehaviorIndex;
    f32 speed;

    if (!(ai->m_ModeFlags & 0x10000) && (actor->m_dwFlags & 0x400)) {
        return;
    }
    speed = motion->speed;
    if (speed < 0.0f ? -speed > 102.399994f : speed > 102.399994f) {
        ai->m_Enemy.m_IdleTimer = 30.0f;
        if (ai->m_Enemy.m_Behavior[1] && modeIndex == 0) {
            CAI__SetMode(actor, ai, 0x26);
        }
        return;
    }
    ai->m_Enemy.m_IdleTimer -= gFrameIncrement;
    if (ai->m_Enemy.m_IdleTimer < 0.0f) {
        ai->m_Enemy.m_IdleTimer = 0.0f;
    }
    if (ai->m_Health && ai->m_Mode == 4 && ai->m_Enemy.m_IdleTimer == 0.0f) {
        ai->m_Enemy.m_IdleTimer = Random_Blend(30.0f, 75.0f);
        if (modeIndex == 0) {
            if (ai->m_Enemy.m_Behavior[1]) {
                CAI__SetMode(actor, ai, 0x26);
            }
        } else if (modeIndex == 1) {
            if (ai->m_Enemy.m_Behavior[0]) {
                CAI__SetMode(actor, ai, 0x28);
            }
        }
    }
}

s32 EnemyAI_TickTimers(CGameObjectInstance *arg0, CAI *arg1) {
    f32 temp_f0;
    f32 temp_f0_2;
    s8 temp_v1;

    if (arg0->header.m_pIntelligence->m_dwTypeFlags & 0x100000) {
        temp_f0 = arg1->m_Enemy.m_Timer - gFrameIncrement;
        arg1->m_Enemy.m_Timer = temp_f0;
        if (temp_f0 < 0.0f) {
            temp_f0_2 = arg1->m_Enemy.m_Timer2 - gFrameIncrement;
            arg1->m_Enemy.m_Timer = 0.0f;
            arg1->m_Enemy.m_Timer2 = temp_f0_2;
            if (temp_f0_2 < 0.0f) {
                arg1->m_Enemy.m_Timer2 = 0.0f;
            }
        }
        if ((arg1->m_Health != 0) && (arg1->m_Enemy.m_Timer <= 0.0f)) {
            if (arg1->m_Enemy.m_Timer2 <= 0.0f) {
                temp_v1 = arg1->m_Mode;
                if ((temp_v1 == 4) || (temp_v1 == 0xE) || (temp_v1 == 0xF) || (temp_v1 == 0x10) || (temp_v1 == 0x11) || (temp_v1 == 0x12) || (temp_v1 == 0xA)) {
                    CAI__SetMode(arg0, arg1, 0x19);
                    return 1;
                }
                return 0;
            }
            goto block_15;
        }
        return 0;
    }
block_15:
    return 0;
}

/* AI attack-wait callback: state 0 counts the delay down; once it has run out
 * (more than 30 frames left means an attack is due) it plays the attack
 * animation for the current mode if the definition allows it. State 1 counts
 * the cool-down and re-picks a region. */

extern s16 CScene__FindNearestPathPoint(void *level, s32 id, CVector3 *pos);

static __inline__ s32 canAttack(CAI *ai, CEnemyIntelligence *info, s32 state) {
    if (state == 0x20 || state == 0x3D) {
        return 0;
    }
    switch (ai->m_Enemy.m_BehaviorIndex) {
    case 0:
        return (info->m_wAlertAnimFlags & 0x1F) != 0;
    case 1:
        return (info->m_wAlertAnimFlags & 0x3E0) != 0;
    case 2:
        return (info->m_wAlertAnimFlags & 0x7C00) != 0;
    }
    return 0;
}

void EnemyAI_AttackWait(CGameObjectInstance *actor, CAI *ai) {
    s32 state;

    if (ai->m_Health == 0) {
        return;
    }
    state = ai->m_Mode;
    if (state == 0) {
        return;
    }
    switch (ai->m_Action) {
    case 0:
        if (ai->m_ActionTime > 30.0f) {
            if (canAttack(ai, &actor->header.m_pIntelligence->m_Enemy, state)) {
                CAI__SetMode(actor, ai, 0xC);
            }
            gEnemyAlertPoint = gEnemyLastNoisePos;
            ai->m_Action = 1;
        } else if (ai->m_ActionTime > 0.0f) {
            ai->m_ActionTime -= gFrameIncrement;
            if (ai->m_ActionTime < 0.0f) {
                ai->m_ActionTime = 0.0f;
            }
        }
        ai->m_Enemy.m_AttackDelay = 0;
        break;
    case 1:
        ai->m_ActionTime -= gFrameIncrement;
        if (ai->m_ActionTime <= 0.0f) {
            ai->m_ActionTime = 0.0f;
            ai->m_Action = 0;
            if (ai->m_PathTrack.m_Mode) {
                ai->m_PathTrack.m_nCurrentPointIndex = CScene__FindNearestPathPoint(&gScene, ai->m_PathTrack.m_nCurrentPathIndex, &actor->header.m_vPos);
            }
        }
        break;
    }
}

#include "structs/ai_behavior.h"
#include "structs/game.h"

/* Typed on CGameObjectInstance / CAI (m2c's Object/AI views had the N64 offsets:
 * the behaviour index at 0x133 was m_Enemy.m_BehaviorIndex, the current mode row at 0x30). */

/* The attached looping sound (only its id). */
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 id;
} Sound;
void *CPlayerPool__FindNearest(CPlayerPool *players, CVector3 pos);

/* AI update: age, scale the frame time, run the current mode handler (its
 * enter callback when the animation changes), count down the timers, start
 * idle animations and the swim/land switches, run the behaviour, and keep
 * the burning effect and its sound going while burnTime lasts. */
void EnemyAI_Update(CGameObjectInstance *obj, CAI *ai) {
    CGameObjectInstance *target;
    CEnemyBehaviorInfo *handler;
    s32 index;
    s32 mode;
    CAIModeInfo *anim;
    s32 state;
    s32 swimming;

    if (!(obj->m_dwFlags & 0x2000)) {
        obj->m_dwFlags &= ~0x100;
        return;
    }
    target = ai->m_pSightTarget;
    if (target != NULL && target->header.m_pIntelligence->m_Type == 1 && (target->m_AI.m_Mode == 0x3C || !(target->m_dwFlags & 0x100))) {
        ai->m_dwFlags &= ~0x20000000;
        ai->m_dwFlags &= ~0x40000000;
    }
    if (ai->m_LastHurtSFXTimer < 100) {
        ai->m_LastHurtSFXTimer++;
    }
    gFrameIncrement *= gEnemyTimeScale;
    CPlayerPool__FindNearest(&gEngineApp_m_Game.m_PlayerList, obj->header.m_vPos);
    mode = ai->m_Enemy.m_Behavior[ai->m_Enemy.m_BehaviorIndex];
    index = ai->m_Enemy.m_BehaviorIndex;
    handler = gEnemyBehaviorInfo[mode];
    swimming = mode == 2;
    if (handler != NULL) {
        switch (index) {
            case 0:
                anim = handler->m_pGroundModeTable;
                break;
            case 1:
                anim = handler->m_pAirModeTable;
                break;
            case 2:
                anim = handler->m_pUnderwaterModeTable;
                break;
            default:
                anim = NULL;
                break;
        }
        if (ai->m_pModeTable != anim) {
            ai->m_pModeTable = anim;
            if (handler->m_pfSetup != NULL) {
                handler->m_pfSetup(obj, ai);
            }
        }
        if (handler->m_pfUpdate != NULL) {
            handler->m_pfUpdate(obj, ai);
        }
    }
    if (!(ai->m_Mode == 0x11 || ai->m_Mode == 0x12)) {
        ai->m_Enemy.m_AttackDelay -= gFrameIncrement;
        if (ai->m_Enemy.m_AttackDelay < 0.0f) {
            ai->m_Enemy.m_AttackDelay = 0.0f;
        }
    }
    index = ai->m_Enemy.m_BehaviorIndex;
    if (obj->header.m_pIntelligence->m_dwTypeFlags & 0x40) {
        ai->m_VarF32[6] -= gFrameIncrement;
        if (ai->m_VarF32[6] < 0.0f) {
            ai->m_VarF32[6] = 0.0f;
        }
        if (ai->m_Health && ai->m_VarF32[6] <= 0.0f) {
            state = ai->m_Mode;
            if (state == 4 || state == 0xE || state == 0xF || state == 0x10 || state == 0x11 || state == 0x12
                || state == 0xA) {
                ai->m_VarF32[6] = Random_Blend(75.0f, 150.0f);
                CAI__SetMode(obj, ai, 0x17);
            }
        }
    }
    EnemyAI_TickTimers(obj, ai);
    if (ai->m_Health) {
        if (index != 2 && (ai->m_dwFlags & 0x80) && ai->m_Mode != 0x27 && ai->m_Mode != 0x29 && ai->m_Enemy.m_Behavior[2] != 0) {
            if (index == 0) {
                CAI__SetMode(obj, ai, 0x27);
            } else if (index == 1) {
                CAI__SetMode(obj, ai, 0x29);
            }
        }
        if (ai->m_Health && index == 2 && !(ai->m_dwFlags & 0x80) && ai->m_Mode != 0x2A && ai->m_Mode != 0x2B
            && ai->m_Enemy.m_Behavior[0] != 0) {
            CAI__SetMode(obj, ai, 0x2A);
        }
    }
    if (ai->m_pModeCurrent != NULL && ai->m_pModeCurrent->m_pfCallback != NULL) {
        ai->m_pModeCurrent->m_pfCallback(obj, ai);
    }
    if (ai->m_Health && swimming) {
        ai->m_vDesiredPos.x = ai->m_vInitialPos.x;
        ai->m_vDesiredPos.z = ai->m_vInitialPos.z;
        obj->m_dwFlags &= ~0x20000;
    }
    AI_TurnHeadToTarget(obj, ai, 0.0f);
    if (ai->m_OnFireTimer > 0.0f) {
        ai->m_OnFireTimer -= gFrameIncrement;
        obj->m_dwHotPointFlags |= 2;
        Sound__Play(0x3D1, obj->header.m_vPos, 0, -1);
        if (ai->m_Health) {
            AI_StartSound(obj, ai, 0x3D2);
        } else if (ai->m_pAttachedLoopingSound != NULL && ((Sound *) ai->m_pAttachedLoopingSound)->id != 0x3D2) {
            AI_StopSound(obj, ai);
        }
    } else {
        obj->m_dwHotPointFlags &= ~2;
    }
    if (ai->m_Mode == 6) {
        obj->m_dwFlags |= 0x800000;
    } else {
        obj->m_dwFlags &= ~0x800000;
    }
}

#include "globals/collision.h"
#include "structs/particle.h"
/* AI hit callback: a projectile (or its owner) close enough lengthens the
 * wait and becomes the last-heard position; then the projectile type picks a
 * reaction animation (flinch, burn, freeze, shock...) the definition allows. */


#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* A noise at pos within 3072 units lengthens the wait and is remembered. */
static __inline__ void hearNoise(CGameObjectInstance *actor, CAI *ai, CVector3 *pos) {
    s32 mode;
    f32 dist;
    f32 add;

    mode = ai->m_Enemy.m_Behavior[ai->m_Enemy.m_BehaviorIndex];
    if (mode >= 0) {
        if (mode < 3) {
            dist = CVector3__Distance(&actor->header.m_vPos, pos);
            if (dist < 3072.0f) {
                add = (3072.0f - dist) * 0.048828125f;
                if (ai->m_ActionTime < add) {
                    ai->m_ActionTime += add;
                    if (add < ai->m_ActionTime) {
                        ai->m_ActionTime = add;
                    }
                    gEnemyLastNoisePos = *pos;
                }
            }
        }
    }
}

/* level and delay are function-wide variables shared by the three reactions
 * (their register choices depend on it). */
void EnemyAI_OnHit(CGameObjectInstance *actor, CAI *ai, CParticle *proj) {
    CEnemyIntelligence *info;
    CParticle *owner;
    s32 v;
    f32 t;
    f32 level;
    f32 delay;

    info = &actor->header.m_pIntelligence->m_Enemy;
    if (ai->m_Health == 0) {
        return;
    }
    owner = proj->m_pOwner;
    if (owner != NULL) {
        hearNoise(actor, ai, &owner->header.m_vPos);
    } else {
        hearNoise(actor, ai, &proj->header.m_vPos);
    }
    switch (proj->header.m_nObjType) {
    case 3:
        if ((info->m_dwFlags & 0x200) && ai->m_Mode != 0x3D) {
            if (--ai->m_TranqHealth == 0) {
                ai->m_TranqHealth += info->m_TranqHealth;
                if (ai->m_Mode == 0x1D) {
                    v = ai->m_Aggression;
                    v += 10;
                    if (v >= 0x100) {
                        v = 0xFF;
                    }
                    ai->m_Aggression = v;
                    level = ai->m_Aggression;
                    level *= (1.0f / 255);
                    delay = level * -120.0f + 240.0f;
                    ai->m_ConditionTimer += delay;
                } else {
                    CAI__SetMode(actor, ai, 0x1D);
                }
            }
        }
        break;
    case 0x128:
        proj->m_Scale = 0.0f;
    case 4:
    case 0x1CC:
    case 0x1CD:
    case 0x1CE:
    case 0x1CF:
    case 0x1D0:
    case 0x1D1:
    case 0x1D2:
        if ((info->m_dwFlags & 0x100) && ai->m_Mode != 0x3D) {
            if (ai->m_Mode == 0x20) {
                v = ai->m_Aggression;
                v += 10;
                if (v >= 0x100) {
                    v = 0xFF;
                }
                ai->m_Aggression = v;
                ai->m_ConditionTimer = MIN(ai->m_ConditionTimer += 15.0f, 300.0f);
            } else {
                CAI__SetMode(actor, ai, 0x20);
                ai->m_ConditionTimer = proj->m_Scale * 300.0f + 15.0f;
            }
        }
        break;
    case 10:
        if (info->m_dwFlags & 0x80) {
            if (ai->m_Mode == 0x1F) {
                v = ai->m_Aggression;
                v += 10;
                if (v >= 0x100) {
                    v = 0xFF;
                }
                ai->m_Aggression = v;
                level = ai->m_Aggression;
                level *= (1.0f / 255);
                delay = level * -45.0f + 105.0f;
                ai->m_ConditionTimer += delay;
            } else {
                CAI__SetMode(actor, ai, 0x1F);
            }
        }
        break;
    case 11:
    case 0x129:
        if (gCollisionResults.m_pInstance != 0 && (gCollisionResults.m_Frame.flags & 3) == 1) {
            if (info->m_dwFlags & 0x400) {
                Sound__Play(0x1AB, actor->header.m_vPos, 0, -1);
                if (ai->m_Mode == 0x21) {
                    v = ai->m_Aggression;
                    v += 20;
                    if (v >= 0x100) {
                        v = 0xFF;
                    }
                    ai->m_Aggression = v;
                    level = ai->m_Aggression;
                    level *= (1.0f / 255);
                    delay = level * -15.0f + 75.0f;
                    ai->m_ConditionTimer += delay;
                } else {
                    ai->m_vBoreCollNormal = gCollisionResults.m_vNormal;
                    CAI__SetMode(actor, ai, 0x21);
                }
            } else if (info->m_dwFlags & 0x2000) {
                Sound__Play(0x213, actor->header.m_vPos, 0, -1);
                CAI__SetMode(actor, ai, 0x22);
            }
        }
        break;
    case 0x14:
        t = ai->m_OnFireTimer;
        t += 30.0f;
        if (!(t <= 120.0f)) {
            t = 120.0f;
        }
        ai->m_OnFireTimer = t;
        break;
    case 0:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0xF:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x15:
    case 0x17:
        break;
    }
}

/* Picks an attack for the current mode: every attack the definition enables
 * for this mode and whose range test passes goes into a weighted random
 * chooser; returns the chosen animation, or 0. */

typedef struct {
    /* 0x00 */ s16 count;
    /* 0x02 */ u8 pad2[0x68 - 0x2];
} Chooser;
extern void WeightedTable_Clear(Chooser *c);
extern void WeightedTable_Add(Chooser *c, s32 id, s32 weight);
extern s16 WeightedTable_Pick(Chooser *c);

s16 EnemyAI_PickAttack(CGameObjectInstance *actor, CAI *ai, CAITargets *t) {
    Chooser chooser;
    CEnemyIntelligence *info = &actor->header.m_pIntelligence->m_Enemy;
    s32 melee;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 mode;

    if (t->m_Path.m_Kind != 5) {
    melee = 0;
    a = 0;
    b = 0;
    c = 0;
    d = 0;
    mode = ai->m_Enemy.m_BehaviorIndex;
    if ((u32)t->m_Sight.m_Kind < 2) { /* read unsigned here */
        if (mode == 0) {
            melee = info->m_wCombatAnimFlags & 0x1F;
            a = info->m_wLeapAnimFlags & 0x1F;
            b = info->m_wDartAnimFlags & 0x1F;
            c = info->m_wProjectileAnimFlags & 0x1F;
            d = info->m_wWeaponAnimFlags & 0x1F;
        } else if (mode == 1) {
            melee = info->m_wCombatAnimFlags & 0x3E0;
            a = info->m_wLeapAnimFlags & 0x3E0;
            b = info->m_wDartAnimFlags & 0x3E0;
            c = info->m_wProjectileAnimFlags & 0x3E0;
            d = info->m_wWeaponAnimFlags & 0x3E0;
        } else if (mode == 2) {
            melee = info->m_wCombatAnimFlags & 0x7C00;
            a = info->m_wLeapAnimFlags & 0x7C00;
            b = info->m_wDartAnimFlags & 0x7C00;
            c = info->m_wProjectileAnimFlags & 0x7C00;
            d = info->m_wWeaponAnimFlags & 0x7C00;
        }
    }
    WeightedTable_Clear(&chooser);
    if (t->m_Path.m_Kind != 7) {
        if (melee && t->m_Path.m_pTarget && t->m_Path.m_Dist <= info->m_AttackCombatRadius) {
            WeightedTable_Add(&chooser, 0xE, 100);
        }
        if (a && t->m_Path.m_pTarget && info->m_AttackLeapRadius - 20.48f <= t->m_Path.m_Dist && t->m_Path.m_Dist <= info->m_AttackLeapRadius + 20.48f) {
            WeightedTable_Add(&chooser, 0xF, 100);
        }
        if (b && t->m_Path.m_pTarget && info->m_AttackDartRadius - 20.48f <= t->m_Path.m_Dist && t->m_Path.m_Dist <= info->m_AttackDartRadius + 20.48f) {
            WeightedTable_Add(&chooser, 0x10, 100);
        }
    }
    if (c && ai->m_Enemy.m_AttackDelay <= 0.0f && t->m_Sight.m_Dist <= info->m_AttackProjectileRadius) {
        WeightedTable_Add(&chooser, 0x12, 100);
    }
    if (d && ai->m_Enemy.m_AttackDelay <= 0.0f && t->m_Sight.m_Dist <= info->m_AttackWeaponRadius) {
        WeightedTable_Add(&chooser, 0x11, 100);
    }
    if (chooser.count != 0) {
        goto pick;
    }
    }
    return 0;
pick:
    return WeightedTable_Pick(&chooser);
}

void EnemyAI_TryDodge(CGameObjectInstance *actor, CAI *ai) {
    f32 skill;
    s32 chance;
    s32 side;
    s32 mode;
    s32 mode2;

    mode = ai->m_Enemy.m_BehaviorIndex;
    mode2 = mode;
    if ((mode == 0 && (actor->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags & 0x1F)) || (mode == 1 && (actor->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags & 0x3E0))
        || (mode2 == 2 && (actor->header.m_pIntelligence->m_Enemy.m_wEvadeAnimFlags & 0x7C00))) {
        skill = ai->m_Aggression * (1.0f / 255);
        chance = skill * (skill * 600.0f) + 100.0f;
        if (Random_Next() % 10000 < chance) {
            side = AI_IsAimedAt(actor, ai, ai->m_pSightTarget, 0.261799425f);
            if (side == -1) {
                CAI__SetMode(actor, ai, 8);
            } else if (side == 1) {
                CAI__SetMode(actor, ai, 9);
            }
        }
    }
}

/* AI chase update: turns toward the target and runs or walks at it; flying
 * modes (1, 2) also climb toward its height and bank into the turn. In range,
 * it attacks (path node 4 / special 7) or lets EnemyAI_PickAttack pick a melee. */

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

extern f32 Math_StepClamped(f32 value, f32 delta, f32 limit);

void EnemyMode_Move_Update(CGameObjectInstance *actor, CAI *ai) {
    CAITargets t;
    CAIFrameMotion dir;
    s32 runAnim;
    s32 walkAnim;
    s32 dodge;
    s32 mode;
    s32 anim;
    f32 maxSpeed;
    f32 angle;
    f32 turn;
    f32 range;
    f32 bank;

    actor->m_dwFlags |= 0x20000;
    actor->m_dwFlags |= 0x20000000;
    AI_UpdateTargets(actor, ai, &t, 1, 1);
    runAnim = 4;
    if (t.m_Path.m_Kind == 1 && actor->header.m_pIntelligence->m_Enemy.m_wFleeAnimFlags) {
        dodge = 0;
        runAnim = 6;
    } else {
        dodge = 0;
    }
    walkAnim = 4;
    if (ai->m_Action == 0) {
        walkAnim = 3;
    }
    AI_ComputeFrameMotion(actor, ai, &dir);
    mode = ai->m_Enemy.m_BehaviorIndex;
    if (mode == 1 || mode == 2) {
        maxSpeed = 81.92f;
        if (mode == 1) {
            maxSpeed = 163.84f;
        }
        if (t.m_Path.m_Dist > 10.24f) {
            turn = 0.261799425f;
            if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
                turn = 1.0471977f;
            }
            AI_TurnClamped(actor, ai, turn, t.m_Path.m_Angle);
        }
        if (t.m_Path.m_Kind == 4) {
            CAI__SetMode(actor, ai, walkAnim);
        } else {
            CAI__SetMode(actor, ai, runAnim);
        }
        actor->m_vVelocity.y = Math_StepClamped(actor->m_vVelocity.y, t.m_Path.m_vDelta.y, dir.m_Dist * 16.0f);
        if (maxSpeed < actor->m_vVelocity.y) {
            actor->m_vVelocity.y = maxSpeed;
        } else if (actor->m_vVelocity.y < -maxSpeed) {
            actor->m_vVelocity.y = -maxSpeed;
        }
        if (mode == 2) {
            bank = -actor->m_vVelocity.y * 0.030543264f;
            if (bank > 0.43633235f) {
                bank = 0.43633235f;
            } else if (bank < -0.43633235f) {
                bank = -0.43633235f;
            }
            ai->m_Enemy.m_Pitch = Math_Approach(ai->m_Enemy.m_Pitch, bank, 0.069813177f);
        }
    } else if (!(ai->m_ModeFlags & 0x400)) {
        angle = 0.261799425f;
        if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
            angle = 1.0471977f;
        }
        if (t.m_Path.m_Dist < 204.79999f) {
            angle *= 3.0f;
        }
        if (ai->m_Mode == 3) {
            angle *= 0.5f;
        }
        AI_TurnClamped(actor, ai, angle, t.m_Path.m_Angle);
    } else {
        turn = 0.261799425f;
        if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
            turn = 1.0471977f;
        }
        AI_TurnClamped(actor, ai, turn, t.m_Sight.m_Angle);
    }
    if (t.m_Sight.m_pTarget) {
        angle = 0.261799425f;
        if (actor->m_nTypeFlag == 0x259) {
            angle = 1.5707965f;
        }
        if (ABS(t.m_Sight.m_Angle) <= angle) {
            anim = EnemyAI_PickAttack_w(actor, ai, &t);
            if (anim != 0) {
                ai->m_ModeLastFrame = -1;
                CAI__SetMode(actor, ai, anim);
                return;
            }
        }
    }
    switch (t.m_Path.m_Kind) {
    case 7:
        range = 30.72f;
        break;
    case 4:
        range = 61.44f;
        if (walkAnim == 3) {
            range = 30.72f;
        }
        break;
    default:
        range = 102.399994f;
        break;
    }
    if (ai->m_ModeFlags & 0x400) {
        range *= 2.0f;
    }
    if (ai->m_Mode != 5 || ai->m_AnimRepeatedCount != 0) {
        if (t.m_Path.m_Dist <= range) {
            switch (t.m_Path.m_Kind) {
            case 4:
                if (CPathTrack__GetCurrentPoint(&ai->m_PathTrack)->m_Flags & 1) {
                    CAI__SetMode(actor, ai, 0xB);
                } else {
                    CPathTrack__Advance(&ai->m_PathTrack);
                }
                break;
            case 7:
                CAI__SetMode(actor, ai, 0xD);
                if (actor->m_dwFlags & 0x400) {
                    ai->m_vDesiredPos.x += t.m_Path.m_vDelta.x * 0.5f;
                    ai->m_vDesiredPos.z += t.m_Path.m_vDelta.z * 0.5f;
                }
                break;
            default:
                CAI__SetMode(actor, ai, 0xA);
                break;
            }
        } else if (t.m_Path.m_Kind == 4) {
            CAI__SetMode(actor, ai, walkAnim);
        } else if (dodge && ABS(t.m_Path.m_Angle) > 2.3561947f) {
            CAI__SetMode(actor, ai, 5);
        } else {
            CAI__SetMode(actor, ai, runAnim);
        }
    }
    EnemyAI_TryDodge(actor, ai);
    EnemyAI_IdleSoundCallback(actor, ai, &t);
}

#include "globals/enemy.h"

s32 EnemyAnim_InteractiveIdle(CGameObjectInstance *actor, CAI *ai) {
    CPathPoint *loc = CPathTrack__GetCurrentPoint(&ai->m_PathTrack);
    s32 animA = -1;
    s32 mode = ai->m_Enemy.m_BehaviorIndex;
    s32 animB = -1;

    if (loc->m_AnimFlagsA != 0) {
        switch (mode) {
        case 0:
            animA = AI_PickAnimFromTable(actor, (sptr) ai, sEnemyAnimIdle_AF6F0, loc->m_AnimFlagsA);
            break;
        case 1:
            animA = AI_PickAnimFromTable(actor, (sptr) ai, sEnemyAnimIdle_AF720, loc->m_AnimFlagsA);
            break;
        case 2:
            animA = AI_PickAnimFromTable(actor, (sptr) ai, sEnemyAnimIdle_AF750, loc->m_AnimFlagsA);
            break;
        }
    }
    if (loc->m_AnimFlagsB != 0) {
        switch (mode) {
        case 0:
            animB = AI_PickAnimFromTable(actor, (sptr) ai, sEnemyAnimIdle_AF780, loc->m_AnimFlagsB);
            break;
        case 1:
            animB = AI_PickAnimFromTable(actor, (sptr) ai, sEnemyAnimIdle_AF7B0, loc->m_AnimFlagsB);
            break;
        case 2:
            animB = AI_PickAnimFromTable(actor, (sptr) ai, sEnemyAnimIdle_AF7E0, loc->m_AnimFlagsB);
            break;
        }
    }
    if (animA != -1 && animB != -1) {
        if (Random_Next() % 1000 >= 501) {
            return animA;
        }
        return animB;
    }
    if (animA != -1) {
        return animA;
    }
    if (animB != -1) {
        return animB;
    }
    return 0;
}

static __inline__ s8 canTaunt(CAI *ai, CEnemyIntelligence *info) {
    if (ai->m_Enemy.m_Behavior[0] && (info->m_wComboEndAnimFlags & 0x1F)) {
        return 1;
    }
    if (ai->m_Enemy.m_Behavior[1] && (info->m_wComboEndAnimFlags & 0x3E0)) {
        return 1;
    }
    if (ai->m_Enemy.m_Behavior[2] && (info->m_wComboEndAnimFlags & 0x7C00)) {
        return 1;
    }
    return 0;
}

void EnemyMode_Attack_Update(CGameObjectInstance *actor, CAI *ai) {
    CAITargets t;
    f32 angle;
    f32 skill;

    AI_UpdateTargets(actor, ai, &t, 0, 1);
    angle = 0.261799425f;
    if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
        angle = 1.0471977f;
    }
    AI_TurnClamped(actor, ai, angle, t.m_Sight.m_Angle);
    if ((ai->m_dwFlags & 0x4000000) && canTaunt(ai, &actor->header.m_pIntelligence->m_Enemy)) {
        CAI__SetMode(actor, ai, 0x13);
        ai->m_dwFlags |= 0x4000000;
        ai->m_ModeLastFrame = ai->m_Mode;
        ai->m_AnimModeSpeed = ai->m_pModeCurrent->m_AnimSpeed;
    } else if (ai->m_AnimType >= 4500 && ai->m_AnimType < 4555) {
        skill = ai->m_Aggression;
        skill = skill * (1.0f / 255);
        skill = skill * 5.0f + 1.0f; /* hits needed: 1..6 */
        if (ai->m_AnimRepeatedCount >= (s32) skill) {
            CAI__SetMode(actor, ai, 0xA);
        }
    } else if (ai->m_AnimRepeatedCount != 0) {
        EnemyMode_Move_Update(actor, ai);
    }
    if (actor->m_nTypeFlag == 0x259 && EnemyAI_PickAttack_w(actor, ai, &t) == 0xE) {
        CAI__SetMode(actor, ai, 0xE);
    }
    EnemyAI_TryDodge(actor, ai);
}

void EnemyMode_Combo_Update(CGameObjectInstance *actor, CAI *ai) {
    CAITargets t;
    f32 angle;

    AI_UpdateTargets(actor, ai, &t, 0, 1);
    angle = 0.261799425f;
    if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
        angle = 1.0471977f;
    }
    AI_TurnClamped(actor, ai, angle * 2.0f, t.m_Sight.m_Angle);
    if (ai->m_AnimType >= 4400 && ai->m_AnimType < 4555) {
        ai->m_dwFlags |= 0x4000000;
    }
    ai->m_ConditionTimer -= gFrameIncrement;
    if (ai->m_AnimRepeatedCount != 0) {
        if (!(ai->m_dwFlags & 0x4000000)) {
            CAI__SetMode(actor, ai, 0xA);
            actor->m_dwFlags |= 0x20000;
        } else if (ai->m_ConditionTimer <= 0.0f) {
            CAI__SetMode(actor, ai, 0x14);
            actor->m_dwFlags |= 0x20000;
        } else {
            ai->m_dwFlags &= ~0x4000000;
            ai->m_ModeLastFrame = -1;
        }
    }
    if (ai->m_ConditionTimer > 15.0f) {
        EnemyAI_TryDodge(actor, ai);
    }
}

#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif

/* Picks the death animation for an AI actor (modes 0x33/0x34): a special one
 * when the region under it is flagged, then the explosive, violent, moving or
 * normal death pickers (the m_w*DeathAnimFlags tables) allowed by the mode
 * bits (20-25), the index (0..2) choosing which 5-bit group of the def's
 * death-flag words enables it. Not an attack picker, as this comment once
 * said. */



typedef struct {
    /* 0x00 */ u8 pad00[0x44];
    /* 0x44 */ s32 flags;
    /* 0x48 */ u8 pad48[0xA];
    /* 0x52 */ u16 flags2;
} TargetInfo;
s32 EnemyAnim_NormalDeath_B0120(void *, void *);
s32 EnemyAnim_NormalDeath_B0150(void *, void *);
s32 EnemyAnim_NormalDeath_B0180(void *, void *);
s32 EnemyAnim_MovingDeath_B01B0(void *, void *);
s32 EnemyAnim_MovingDeath_B01E0(void *, void *);
s32 EnemyAnim_MovingDeath_B0210(void *, void *);
s32 EnemyAnim_ViolentDeath_B0240(void *, void *);
s32 EnemyAnim_ViolentDeath_B0270(void *, void *);
s32 EnemyAnim_ViolentDeath_B02A0(void *, void *);
s32 EnemyAnim_ExplosiveDeath_B02D0(void *, void *);
s32 EnemyAnim_ExplosiveDeath_B0300(void *, void *);
s32 EnemyAnim_ExplosiveDeath_B0330(void *, void *);
s32 EnemyAnim_NormalDeath_B0360(void *, void *);

s32 EnemyAnim_PickDeath(CGameObjectInstance *actor, CAI *state) {
    s32 result = -1;
    s32 flags = state->m_dwFlags;
    s32 *def = actor->header.m_pIntelligence;
    s32 index = state->m_Enemy.m_BehaviorIndex;
    s32 mode = flags & 0x03F00000;
    sptr target = actor->header.m_pCurrentRegion;
    u16 *masks = (u16 *)((u8 *)def + 0x14); /* [0x3C] close, [0x3D] mid, [0x3E] far */
    s32 rest = flags & ~0x03F00000;
    TargetInfo *t;

    if (*def != 1) {
        return 0;
    }
    if (target != 0) {
        t = CScene__GetRegionInfo(&gScene, target);
        if (t != NULL && ((t->flags & 0x400000) || (t->flags2 & 0xC0))) {
            result = EnemyAnim_NormalDeath_B0360(actor, state);
            if (result != -1) {
                return result;
            }
        }
    }
    if (mode & 0xC00000) {
        if (index == 0 && (masks[0x3E] & 0x1F)) {
            result = EnemyAnim_ExplosiveDeath_B02D0(actor, state);
        } else if (index == 1 && (masks[0x3E] & 0x3E0)) {
            result = EnemyAnim_ExplosiveDeath_B0300(actor, state);
        } else if (index == 2 && (masks[0x3E] & 0x7C00)) {
            result = EnemyAnim_ExplosiveDeath_B0330(actor, state);
        }
        if (CGameObjectInstance__PickRandomAnim(actor, result, -1) == -1) {
            mode = 0x200000;
        }
    }
    if (mode & 0x200000) {
        if (index == 0 && (masks[0x3D] & 0x1F)) {
            result = EnemyAnim_ViolentDeath_B0240(actor, state);
        } else if (index == 1 && (masks[0x3D] & 0x3E0)) {
            result = EnemyAnim_ViolentDeath_B0270(actor, state);
        } else if (index == 2 && (masks[0x3D] & 0x7C00)) {
            result = EnemyAnim_ViolentDeath_B02A0(actor, state);
        }
        if (CGameObjectInstance__PickRandomAnim(actor, result, -1) == -1) {
            mode = 0x100000;
        }
    }
    if (result == -1) {
        if (index == 0) {
            if ((masks[0x3C] & 0x1F) && state->m_AnimType >= 2000 && state->m_AnimType < 2025) {
                result = EnemyAnim_MovingDeath_B01B0(actor, state);
            } else {
                result = EnemyAnim_NormalDeath_B0120(actor, state);
            }
        } else if (index == 1) {
            if ((masks[0x3C] & 0x3E0) && state->m_AnimType >= 2100 && state->m_AnimType < 2125) {
                result = EnemyAnim_MovingDeath_B01E0(actor, state);
            } else {
                result = EnemyAnim_NormalDeath_B0150(actor, state);
            }
        } else if (index == 2) {
            if ((masks[0x3C] & 0x7C00) && state->m_AnimType >= 2200 && state->m_AnimType < 2225) {
                result = EnemyAnim_MovingDeath_B0210(actor, state);
            } else {
                result = EnemyAnim_NormalDeath_B0180(actor, state);
            }
        }
    }
    state->m_dwFlags = rest | mode;
    return result;
}

/* Index-2 attack follow-up: in the 7020-7041 animation band it chains to the
 * next move; otherwise it finishes the current one. */

s32 EnemyAnim_NormalDeath_B0390(void *, void *);
s32 EnemyAnim_NormalDeath_B03C0(void *, void *);

void EnemyMode_Death_Update(CGameObjectInstance *actor, CAI *state) {
    CEnemyIntelligence *info = &actor->header.m_pIntelligence->m_Enemy;
    s32 anim;

    if (state->m_Enemy.m_BehaviorIndex == 2) {
        switch (state->m_AnimType) {
        case 7020:
        case 7025:
        case 7030:
        case 7035:
        case 7040:
            if (state->m_AnimRepeatedCount == 0) {
                return;
            }
            anim = EnemyAnim_NormalDeath_B0390(actor, state);
            break;
        case 7021:
        case 7026:
        case 7031:
        case 7036:
        case 7041:
            if (!GameObject_IsOnGround(actor)) {
                return;
            }
            anim = EnemyAnim_NormalDeath_B03C0(actor, state);
            break;
        default:
            goto finish;
        }
        state->m_AnimIndex = CGameObjectInstance__PickRandomAnim(actor, anim, -1);
        state->m_AnimType = anim;
        state->m_AnimRepeatedCount = 0;
        state->m_dwFlags &= ~1;
        return;
    }
finish:
    if (state->m_AnimRepeatedCount != 0) {
        if (state->m_Mode == 0x35) {
            CAI__SetMode(actor, state, 0x36);
        } else {
            state->m_dwFlags |= 0x20;
            actor->m_dwFlags &= ~0x01000000;
            AI_DropPickupsOnce(actor, state, 4, info->m_DropMask, info->m_DropMask2);
            actor->m_dwFlags &= ~0x20000;
            if (state->m_ModeTime > 90.0f) {
                CAI__SetMode(actor, state, 0x3C);
            }
        }
        if (info->m_dwFlags & 0x10000) {
            actor->m_dwFlags &= ~0x100;
            actor->m_dwFlags &= ~0x2000;
            AI_StopSound(actor, state);
            if (actor->m_dwFlags & 0x80000) {
                CListedInstance__Free(actor);
            }
        }
    } else {
        state->m_ModeTime = 0.0f;
    }
}

s8 EnemyAI_GetBehaviorIndex(sptr arg0, CAI *ai) {
    return ((s8 *) &ai->m_VarBuffer[9])[3];
}

void EnemyAI_SetBehaviorIndex(s32 arg0, CAI *ai, s8 arg2) {
    ((s8 *) &ai->m_VarBuffer[9])[3] = arg2;
}

s8 EnemyAI_GetBehavior(s32 arg0, CAI *arg1, s32 arg2) {
    return arg1->m_Enemy.m_Behavior[arg2];
}

/* The current mode entry. */
s8 EnemyAI_GetCurrentBehavior(sptr arg0, CAI *arg1) {
    return arg1->m_Enemy.m_Behavior[arg1->m_Enemy.m_BehaviorIndex];
}

void EnemyAI_SetBehavior(s32 arg0, CAI *arg1, s32 arg2, s8 arg3) {
    arg1->m_Enemy.m_Behavior[arg2] = arg3;
}

/* Byte arg2 of the AI variable buffer, read from its byte 0x24 on (m_VarBuffer[9]). */
s8 EnemyAI_GetBehaviorAt(s32 arg0, CAI *arg1, s32 arg2) {
#ifdef TARGET_PORT
    /* the port (round 9 x1): by member. The host CAI's variable buffer is not at N64 0x10C and
     * CEnemyAIVars holds a pointer (m_pGrabbed) before m_Behavior: N64 0x130 is m_Behavior[0]. */
    return ((s8 *) arg1->m_Enemy.m_Behavior)[arg2];
#else
    return *(s8 *) ((sptr) arg1 + arg2 + 0x130);
#endif
}

/* pointer-wide on the host (round 8 w2): the grabbed throwable (m_Enemy.m_pGrabbed) */
sptr EnemyAI_GetGrabbed(sptr arg0, CAI *arg1) {
    return (sptr) arg1->m_Enemy.m_pGrabbed;
}

void EnemyAI_SetGrabbed(sptr arg0, CAI *arg1, sptr arg2) {
    /* arg2 is the grabbed actor: pointer-wide m_pGrabbed, its CAI at the host offset */
    arg1->m_Enemy.m_pGrabbed = (CGameObjectInstance *) arg2;
    if (arg2 != 0) {
        Throwable_SetModeAnim(arg2, (sptr) &((CGameObjectInstance *) arg2)->m_AI, arg0);
    }
}

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s8 min;
    /* 0x19 */ s8 max;
} Range0025E4B8;
typedef struct {
    /* 0x00 */ CGameObjectInstance *owner;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ Range0025E4B8 *range;
    /* 0x18 */ u8 pad18[0x90 - 0x18];
    /* 0x90 */ s32 value;
} Effect0025E4B8;
#ifdef TARGET_PORT
#include "structs/pose_host.h"
/* Host body: the port's pose context (structs/pose_host.h) in place of the N64 one; the matrix
 * pointer-wide. */
void EnemyAI_HeadNodeCallback(sptr arg0, T2PoseContext *ctx) {
    Range0025E4B8 *range = (Range0025E4B8 *) ctx->info;
    s32 bone = ctx->bone;

    if ((bone >= range->min) && (range->max >= bone)) {
        CMtxF__RotateZ((void *) arg0, ctx->obj->m_AI.m_HeadTurn);
        CMtxF__RotateX((void *) arg0, ctx->obj->m_AI.m_HeadTilt);
    }
}
#else
void EnemyAI_HeadNodeCallback(s32 arg0, Effect0025E4B8 *arg1) {
    s32 temp_v1;
    Range0025E4B8 *temp_a0;

    temp_a0 = arg1->range;
    temp_v1 = arg1->value;
    if ((temp_v1 >= temp_a0->min) && (temp_a0->max >= temp_v1)) {
        CMtxF__RotateZ((void *) arg0, arg1->owner->m_AI.m_HeadTurn);
        CMtxF__RotateX((void *) arg0, arg1->owner->m_AI.m_HeadTilt);
    }
}
#endif

/* On CAI's members. Round 8: an N64 view of CAI (AlertState) read the behaviour table and its
 * index at 0x130/0x133 (m_Enemy.m_Behavior[], m_BehaviorIndex), elsewhere on the host, so a heard
 * or seen player never raised m_ActionTime: enemies stayed unalerted (m_Action 0),
 * AI_FindBestTarget skipped them for path targets and they never chose a melee attack. */
void AI_HearNoise(CGameObjectInstance *actor, CAI *ai, CVector3 *pos, s32 kind) {
    s32 mode = ai->m_Enemy.m_Behavior[ai->m_Enemy.m_BehaviorIndex];
    f32 strength;
    f32 dist;
    f32 alert;

    if (mode >= 0) {
    if (mode < 3) {
        switch (kind) {
        case 1:
            strength = 45.0f;
            break;
        case 2:
            strength = 105.0f;
            break;
        case 3:
            strength = 150.0f;
            break;
        case 0:
        default:
            return;
        }
        dist = CVector3__Distance(&actor->header.m_vPos, pos);
        if (dist < 3072.0f) {
            strength = strength * (1.0f / 3072.0f) * (3072.0f - dist);
            alert = ai->m_ActionTime;
            if (alert < strength) {
                ai->m_ActionTime = alert + strength;
                if (strength < alert + strength) {
                    ai->m_ActionTime = strength;
                }
                gEnemyLastNoisePos = *pos;
            }
        }
    }
    }
}

#include "m2c_macros.h"

void EnemyMode_WaitForTrigger_Enter(void) {

}

void EnemyMode_WaitForTrigger_Update(CGameObjectInstance *arg0, sptr arg1) {
    CIntelligence *temp_a2;

    temp_a2 = arg0->header.m_pIntelligence;
    if ((temp_a2->m_Type == 1) && !(temp_a2->m_dwTypeFlags & 0x800)) {
        CAI__SetMode(arg0, arg1, 2);
    }
}

void EnemyMode_StartDelay_Enter(void) {

}

void EnemyMode_StartDelay_Update(sptr arg0, CAI *arg1) {
    f32 temp_f0;

    temp_f0 = arg1->m_ConditionTimer - gFrameIncrement;
    arg1->m_ConditionTimer = temp_f0;
    if (temp_f0 <= 0.0f) {
        CAI__SetMode(arg0, arg1, 2);
    }
}

/* the port (round 9 x1): the CAI by member: N64 0x130/0x131 are m_Enemy.m_Behavior[0]/[1] (the
 * host's variable buffer is elsewhere and CEnemyAIVars holds a pointer before them) */
typedef CAI State;
#define unk130 m_Enemy.m_Behavior[0]
#define unk131 m_Enemy.m_Behavior[1]

s32 EnemyAnim_Idle_AF720(CGameObjectInstance *actor, State *state);
s32 EnemyAnim_Idle_AF750(CGameObjectInstance *actor, State *state);

s32 EnemyAnim_PickStart(CGameObjectInstance *actor, State *state) {
    CEnemyIntelligence *ov = &actor->header.m_pIntelligence->m_Enemy;

    if (ov->m_StartAnim != 0) {
        if (actor->m_nTypeFlag == 0x1F9 || actor->m_nTypeFlag == 0x385) {
            switch (ov->m_StartAnim) {
                case 0x2B16:
                case 0x2B20:
                case 0x2B2A:
                case 0x2B34:
                case 0x2B3E:
                    return ov->m_StartAnim + 1;
                default:
                    return ov->m_StartAnim;
            }
        }
        return ov->m_StartAnim;
    }
    if (state->unk130) {
        return EnemyAnim_Idle_AF6F0(actor, state);
    }
    if (state->unk131) {
        return EnemyAnim_Idle_AF720(actor, state);
    }
    return EnemyAnim_Idle_AF750(actor, state);
}

void EnemyMode_Start_Enter(CGameObjectInstance *obj) {
    u16 temp_a0;

    temp_a0 = obj->header.m_pIntelligence->m_Enemy.m_StartSound;
    if (temp_a0 != 0) {
        Sound__Play((s16) temp_a0, obj->header.m_vPos, 0, -1);
    }
}

/* Like EnemyAI_GetModeTurnRate: the turn rate, pi/3 in modes 1-2, else pi/12. */
static __inline__ f32 TurnRate(CGameObjectInstance *self, CAI *ai) {
    f32 rate = 0.261799425f;

    if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
        rate = 1.0471977f;
    }
    return rate;
}

/* AI callback: turns toward the target (pi/3 at a time in modes 1-2, else
 * pi/12) and goes to mode 4 when done. */
void EnemyMode_Start_Update(CGameObjectInstance *self, CAI *ai) {
    CEnemyIntelligence *sub = &self->header.m_pIntelligence->m_Enemy;
    CAITargets t;

    AI_UpdateTargets(self, ai, &t, 0, 1);
    AI_TurnClamped(self, ai, TurnRate(self, ai), t.m_Sight.m_Angle);
    if (sub->m_StartAnim == 0 || ai->m_AnimRepeatedCount != 0) {
        /* FAKEMATCH: the identical branches are merged by cross-jumping; the
         * extra use of self only gives it the register priority it has in the
         * ROM ($s0 for self, $s1 for ai). */
        if (self->header.m_pIntelligence != NULL) {
            CAI__SetMode(self, ai, 4);
        } else {
            CAI__SetMode(self, ai, 4);
        }
    }
}

void EnemyMode_Death_Enter(CGameObjectInstance *arg0, CAI *arg1) {
    s32 temp_a0;
    CEnemyIntelligence *sub;

    arg1->m_Health = 0;
    arg1->m_ModeLastFrame = -1;
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x20000);
    temp_a0 = arg0->header.m_pIntelligence->m_Type;
    if (((temp_a0 == 1) || (temp_a0 == 4)) && ((u32) ((u8) arg1->m_Mode - 0x35) < 7U)) {
        arg1->m_dwFlags |= 0x20;
    }
    sub = &arg0->header.m_pIntelligence->m_Enemy;
    if (!(arg0->m_dwFlags & 0x80000)) {
        if (!(sub->m_dwFlags & 0x20)) {
            CScene__SetInstanceSavedBit(&gScene, arg0, 1);
        }
        GameObject_FireTriggers(arg0, 1, arg0);
    }
}
void EnemyAI_DrawFadeOut(sptr arg0, CAI *ai, sptr arg2);
void CGameRegion__FireTriggers(struct CGameRegion *region, s32 flags, CGameObjectInstance *obj);

void EnemyMode_DeathMelt_Enter(CGameObjectInstance *obj, CAI *ai) {
    ai->m_pfDraw = EnemyAI_DrawFadeOut;
    ai->m_dwFlags |= 0x20;
    obj->m_dwHotPointFlags = 0;
    obj->m_dwFlags &= ~0x01000000;
    obj->m_dwFlags &= ~0x00010000;
    AI_StopSound(obj, ai);
    CGameRegion__FireTriggers(obj->header.m_pCurrentRegion, 0x80, obj);
}

void EnemyMode_DeathMelt_Update(CGameObjectInstance *obj, CAI *ai) {
    f32 t;
    f32 squash;

    ai->m_OnFireTimer = 0;
    obj->m_dwHotPointFlags &= ~2;
    AI_StopSound(obj, ai);
    t = obj->m_AI.m_ModeTime;
    if (t > 30.0f) {
        squash = 0.01f;
        obj->m_dwFlags &= ~0x100;
        obj->m_dwFlags &= ~0x2000;
        AI_StopSound(obj, ai);
        if (obj->m_dwFlags & 0x80000) {
            CListedInstance__Free(obj);
        }
    } else {
        squash = t * (0.01f / 30.0f);
    }
    obj->m_vScale.x *= 1.0f + squash;
    obj->m_vScale.y *= 1.0f - squash;
    obj->m_vScale.z *= 1.0f + squash;
}

s32 EnemyAI_UsesRideTables(CGameObjectInstance *arg0, CAI *arg1) {
    sptr temp_v1;

    temp_v1 = (sptr) arg1->m_pModeTable;
    if ((temp_v1 == (sptr) gEnemyStationaryGroundModeTable) || (temp_v1 == (sptr) gEnemyStationaryAirModeTable) || (temp_v1 == (sptr) gEnemyStationaryUnderwaterModeTable)) {
        return 0;
    }
    return (u16) arg0->m_nTypeFlag != 0x259;
}

f32 EnemyAI_GetTypeTurnRate(CGameObjectInstance *obj) {
    f32 var_f0;

    var_f0 = 1.5707965f;
    if ((u16) obj->m_nTypeFlag != 0x259) {
        var_f0 = 0.26179942f;
    }
    return var_f0;
}

f32 EnemyAI_GetModeTurnRate(sptr arg0, CAI *arg1) {
    f32 var_f0;

    var_f0 = 0.26179942f;
    if ((u32) ((u8) arg1->m_ModeBefore - 1) < 2U) {
        var_f0 = 1.0471977f;
    }
    return var_f0;
}

f32 AI_LerpByAggression(sptr arg0, u8 *arg1, f32 arg2, f32 arg3) {
    return arg2 + arg1[0xC8] * 0.003921569f * (arg3 - arg2);
}

void EnemyAI_Mount(CGameObjectInstance *obj, CAI *ai, CPlayer *player) {
    ai->m_Enemy.m_pGrabbed = (CGameObjectInstance *) player; /* pointer-wide (m_VarBuffer[2] is a u32) */
    if (player != NULL) {
        Throwable_SetModeAnim(player, &player->m_CharacterActor.m_AI, obj);
    }
    player->m_vOnEnemyOriginalScale = obj->m_vScale;
    obj->m_dwFlags &= ~0x20000000;
    CScene__SetInstanceObjectType(&gScene, obj, player->m_OnEnemyObjectType + 1, ai->m_AnimType);
    CAI__SetMode(obj, ai, 0x2C);
}

void EnemyAI_Dismount(CGameObjectInstance *obj, CAI *ai, CPlayer *player) {
    obj->m_AI.m_VarBuffer[2] = 0;
    obj->m_dwFlags |= 0x20000;
    obj->m_dwFlags |= 0x10000;
    ai->m_pfDraw = EnemyAI_DrawFaded;
    ai->m_pfCalculateOrientationMatrix = EnemyAI_CalcOrientationMatrix;
    obj->m_vScale = player->m_vOnEnemyOriginalScale;
    obj->m_dwFlags |= 0x20000000;
    CAI__SetMode(obj, ai, 10);
    CScene__SetInstanceObjectType(&gScene, obj, player->m_OnEnemyObjectType, ai->m_AnimType);
}

/* Host body (C23): the N64 code reads the draw args' model handle (0x0C), the actor's draw
 * matrices (0x98) and lights (0x110 + gEvenOdd * 0x18) and the low byte of m_nModel (0x1B, big
 * endian) at N64 offsets, with the actor and the args as s32. */
void EnemyAI_DrawNode(sptr arg0, sptr arg1, sptr arg2) {
    CGameObjectInstance *actor = (CGameObjectInstance *) arg0;
    CDrawArgs *args = (CDrawArgs *) arg2;

    CGameObjectInstance__DrawModelImmediate(arg0, arg1, arg2);
    Render_DrawModelShared(args->m_hModel, (sptr) actor->m_pmtDrawMtxs, 1, (sptr) &actor->m_Lights[gEvenOdd], 0,
                           (s8) args->m_nModel, -1);
}

typedef struct {
    /* 0x0 */ f32 alpha;
    /* 0x4 */ f32 alphaMax;
} FillAlpha;

void EnemyAI_DrawWithBlend(CGameObjectInstance *obj, CAI *ai, CDrawArgs *arg2) {
    f32 fade = 1.0f;
    CVector3 *color;
    FillAlpha *alpha;

    if (ai->m_ModeTime < 15.0f) {
        color = (CVector3 *) gBlendState_m_Color;
        color->z = 0;
        color->x = 255.0f;
        color->y = 255.0f;
        fade = 1.0f - ai->m_ModeTime * (1.0f / 15.0f);
    } else {
        color = (CVector3 *) gBlendState_m_Color;
        color->x = color->y = color->z = 0.0f;
        if (ai->m_ModeTime < 90.0f) {
            fade = (ai->m_ModeTime - 15.0f) * (1.0f / 75.0f);
        }
    }
    gBlendState = 3;
    alpha = (FillAlpha *) &gBlendState_m_Color[3];
    alpha->alphaMax = 255.0f;
    alpha->alpha = fade * 210.0f;
    CGameObjectInstance__DrawModelImmediate(obj, ai, arg2);
    gBlendState = 0;
}

s32 Cinema_PathPlaying();                                /* extern */

/* The third argument is not read (EnemyAI_SleepIfOffscreen passes its own on). */
void EnemyAI_CheckOffscreen(CGameObjectInstance *arg0, CAI *arg1, s32 unused) {
    if (CScene__GetInstanceSavedBit(&gScene, arg0) == 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    }
    if (Cinema_PathPlaying() != 0) {
        arg1->m_dwFlags |= 0x200;
    }
}

void EnemyAI_SleepIfOffscreen(CGameObjectInstance *arg0, CAI *arg1, s32 arg2) {
    if ((CScene__GetInstanceSavedBit(&gScene, arg0) == 0) && (arg1->m_Mode == 0)) {
        EnemyAI_CheckOffscreen(arg0, arg1, arg2);
        CAI__SetMode(arg0, arg1, 2);
    }
}

void EnemyAI_EventNull3(void) {

}

void EnemyAI_EventNull4(void) {

}

s32 Cinema_RequestForObject(sptr, sptr);

void EnemyAI_EventPlayCinema(s32 arg0, s32 arg1, s32 arg2) {
    Cinema_RequestForObject(arg0, arg2);
}

void EnemyAI_EventPlayCinemaRemote(s32 arg0) {
    Cinema_RequestForObject(0, arg0);
}

/* What the last collision test hit, gCollisionResults.m_pInstance (its first word), read as a
 * pointer variable: the member access loads the struct's address first and does not match. */
#define sHit (*(CGameObjectInstance **) &gCollisionResults)

void EnemyAI_CheckGrab(CGameObjectInstance *arg0, CAI *arg1) {
    void *owner;

    if ((arg1->m_Health != 0) && (sHit != NULL) && (owner = arg1->m_pPathTarget, !(arg1->m_ModeFlags & 0x1000)) && (sHit == owner) && (sHit->header.m_pIntelligence->m_Type == 5)) {
        arg1->m_Enemy.m_pGrabbed = sHit; /* pointer-wide on the host (m_VarBuffer[2] is a u32) */
        Throwable_SetModeAnim(sHit, &sHit->m_AI, arg0);
        CAI__SetMode(arg0, arg1, 0x15);
    }
}

s32 EnemyAnim_WaitForTrigger(CGameObjectInstance *arg0, sptr arg1) {
    u16 kind;
    CEnemyIntelligence *sub;

    kind = arg0->m_nTypeFlag;
    sub = &arg0->header.m_pIntelligence->m_Enemy;
    if (kind == 0x1F9 || kind == 0x385) {
        switch (sub->m_StartAnim) {
            case 0x2B16:
            case 0x2B20:
            case 0x2B2A:
            case 0x2B34:
            case 0x2B3E:
                break;
            default:
                return EnemyAnim_Idle_AF6F0(arg0, (CAI *) arg1);
        }
    }
    return sub->m_StartAnim;
}

void EnemyMode_InteractiveIdle_Update(CGameObjectInstance *obj, CAI *ai) {
    CAITargets info;
    f32 angle;

    AI_UpdateTargets(obj, ai, &info, 0, 1);
    angle = 0.2617994249f; /* 15 deg */
    if ((u8) ai->m_ModeBefore == 1 || (u8) ai->m_ModeBefore == 2) {
        angle = 1.0471977f; /* 60 deg */
    }
    AI_TurnClamped(obj, ai, angle, info.m_Sight.m_Angle);
    if (ai->m_AnimRepeatedCount != 0 || ai->m_Action == 1) {
        CPathTrack__Advance(&ai->m_PathTrack);
        CAI__SetMode(obj, ai, 10);
    }
}

/* The point turned to is one CVector3 by value; the ROM loads its three words through the
 * interior symbols gEnemyAlertPoint_y/_z (a struct argument shares one base register and does
 * not match), so the N64 build calls AI_AngleToPoint as taking three floats, which on MIPS go in
 * the same registers. */
#ifdef TARGET_PORT
#define AlertPointAngle(self) AI_AngleToPoint(self, gEnemyAlertPoint)
#else
#define AlertPointAngle(self) \
    ((f32 (*)(CGameObjectInstance *, f32, f32, f32)) AI_AngleToPoint)(self, gEnemyAlertPoint.x, gEnemyAlertPoint_y, gEnemyAlertPoint_z)
#endif

/* AI callback: turns toward the point gEnemyAlertPoint, then calls EnemyMode_Move_Update
 * once 0xC7 reaches 5. */
void EnemyMode_Alert_Update(CGameObjectInstance *self, CAI *ai) {
    AI_TurnClamped(self, ai, TurnRate(self, ai), AlertPointAngle(self));
    if (ai->m_AnimRepeatedCount >= 5) {
        /* FAKEMATCH: the identical branches are merged by cross-jumping; the
         * extra use of self only gives it the register priority it has in the
         * ROM ($s0 for self, $s1 for ai). */
        if (self->header.m_pIntelligence != NULL) {
            EnemyMode_Move_Update(self, ai);
        } else {
            EnemyMode_Move_Update(self, ai);
        }
    }
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS EnemyMode_Move_Update receives EnemyMode_TakeCover_Update's own argument
 * registers (pass-through); the host passes them. */
void EnemyMode_TakeCover_Update(void * pt0, void * pt1) {
    EnemyMode_Move_Update(pt0, pt1);
}

void EnemyMode_Evade_Update(CGameObjectInstance *obj, CAI *ai) {
    u16 anim;

    obj->m_dwFlags |= 0x20000;
    anim = ai->m_AnimType;
    if ((anim >= 0x206C && anim <= 0x2070) || (anim >= 0x2076 && anim <= 0x207A) ||
        (anim >= 0x2080 && anim <= 0x2084)) {
        if (AI_IsAimedAt(obj, ai, ai->m_pSightTarget, 0.2617994249f /* 15 deg */)) {
            ai->m_ModeTime = 0;
        } else if (ai->m_ModeTime > 30.0f) {
            CAI__SetMode(obj, ai, 10);
        }
    } else if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(obj, ai, 10);
    }
}

/* AI callback: in modes 0x11-0x12 sets 0x120 from the intensity (0..255 ->
 * 60..15); clears AI flag 0x4000000. */
void EnemyMode_Attack_Enter(CGameObjectInstance *self, CAI *ai) {
    f32 level;

    if (ai->m_Mode == 0x11 || ai->m_Mode == 0x12) {
        level = ai->m_Aggression;
        level *= (1.0f / 255);
        level = level * -45.0f + 60.0f;
        ai->m_Enemy.m_AttackDelay = level;
    }
    ai->m_dwFlags &= ~0x4000000;
}

/* AI callback: clears AI flag 0x4000000 and actor flag 0x20000; sets the
 * timer from the intensity (0..255 -> 15..45). */
void EnemyMode_Combo_Enter(CGameObjectInstance *self, CAI *ai) {
    f32 level;

    ai->m_dwFlags &= ~0x4000000;
    self->m_dwFlags &= ~0x20000;
    level = ai->m_Aggression;
    level *= (1.0f / 255);
    level = level * 30.0f + 15.0f;
    ai->m_ConditionTimer = level;
}

void EnemyMode_ComboEnd_Update(sptr arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 0xA);
    }
}

void EnemyMode_PickupThrowObject_Enter(sptr arg0, CAI *arg1) {
    sptr temp_a0;

    /* the grabbed throwable: pointer-wide m_pGrabbed, its CAI at the host offset */
    temp_a0 = (sptr) arg1->m_Enemy.m_pGrabbed;
    CAI__SetMode(temp_a0, (sptr) &((CGameObjectInstance *) temp_a0)->m_AI, 1);
}

/* AI state: turn towards the target with the 10000 animation, the turn rate
 * depending on the stance; release the grabbed actor near the end of it. */


void EnemyMode_PickupThrowObject_Update(CGameObjectInstance *self, CAI *ai) {
    CAITargets t;
    u8 *held = (u8 *) ai->m_Enemy.m_pGrabbed;
    f32 rate;
    sptr anim;

    anim = CGameObjectInstance__PickRandomAnim(self, 10000, -1);
    if (self->m_CurrentAnimation.m_nAnimIndex == anim) {
        AI_UpdateTargets(self, ai, &t, 0, 1);
        rate = 0.26179942f;
        if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
            rate = 1.0471977f;
        }
        AI_TurnClamped(self, ai, rate * 1.5f, t.m_Sight.m_Angle);
        if (held != NULL && self->m_CurrentAnimation.m_nFrames * 0.7f <= self->m_CurrentAnimation.m_AnimationFrame) {
            CAI__SetMode(held, &((CGameObjectInstance *) held)->m_AI, 2); /* N64 +0x140 */
            ai->m_Enemy.m_pGrabbed = NULL;
        }
    }
    if (ai->m_AnimRepeatedCount) {
        CAI__SetMode(self, ai, 10);
    }
}

void EnemyMode_DropObject_Enter(void) {

}

void EnemyMode_DropObject_Update(sptr arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 0xA);
    }
}

void EnemyMode_TeleportOut_Update(sptr arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 0x18);
    }
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS GameObject_IsLeashed receives EnemyMode_TeleportIn_Enter's own argument
 * registers (pass-through); the host passes them. */
void EnemyMode_TeleportIn_Enter(CGameObjectInstance *self, CAI *ai) {
    f32 radius;
    f32 dist;
    f32 angle;
    CVector3 pos;

    if (GameObject_IsLeashed(self)) {
        radius = GameObject_GetLeashRadius(self);
    } else {
        radius = 2048.0f;
    }
    dist = Random_Blend(0.0f, radius);
    angle = Random_Blend(-3.141593f, 3.141593f);
    pos.x = ai->m_vLeashPos.x + dist * func_002974C0(angle);
    pos.y = ai->m_vLeashPos.y;
    pos.z = ai->m_vLeashPos.z + dist * func_002982F0(angle);
    Collision_MoveInstance(self, pos, (sptr) ai->m_pModeCurrent->m_pData);
    ai->m_vDesiredPos = self->header.m_vPos;
}

void EnemyMode_TeleportIn_Update(sptr arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, ai, 0xA);
    }
}

void EnemyMode_CloakActivate_Enter(CGameObjectInstance *arg0, CAI *arg1) {
    f32 temp_f20;
    f32 temp_f21;

    temp_f20 = 1.5e2f;
    temp_f21 = 3.0e2f;
    arg1->m_Enemy.m_Timer = Random_Blend(temp_f20, temp_f21);
    arg1->m_Enemy.m_Timer2 = Random_Blend(temp_f20, temp_f21);
}

void EnemyMode_CloakActivate_Update(sptr arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount >= 4) {
        CAI__SetMode(arg0, arg1, 0xA);
    }
}

void EnemyMode_GroundToAir_Update(sptr arg0, CAI *arg1) {
    ((s8 *)arg1->m_VarBuffer)[0x133 - 0x10C] = 1;
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 4);
    }
}

void EnemyMode_GroundToUnderwater_Update(sptr arg0, CAI *arg1) {
    arg1->m_Enemy.m_BehaviorIndex = 2;
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 4);
    }
}

void EnemyMode_AirToGround_Update(CGameObjectInstance *obj, CAI *ai) {
    ai->m_Enemy.m_BehaviorIndex = 0;
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(obj, ai, 4);
    }
}

void EnemyMode_AirToUnderwater_Update(sptr arg0, CAI *ai) {
    ((s8 *)&ai->m_VarBuffer[9])[3] = 2;
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, ai, 4);
    }
}

void EnemyMode_UnderwaterToGround_Update(sptr arg0, CAI *arg1) {
    ((s8 *) arg1->m_VarBuffer)[0x27] = 0;
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 4);
    }
}

void EnemyMode_UnderwaterToAir_Update(CGameObjectInstance *obj, CAI *ai) {
    ai->m_Enemy.m_BehaviorIndex = 1;
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(obj, ai, 4);
    }
}

void EnemyAI_BobToTargetHeight(CGameObjectInstance *self, CAI *ai) {
    CAITargets t;
    f32 v;

    if (ai->m_Mode == 0x3C) {
        EnemyMode_DeathMelt_Update(self, ai);
    }
    AI_UpdateTargets(self, ai, &t, 1, 0);
    ai->m_ConditionTimer += gFrameIncrement * 15.0f;
    t.m_Path.m_vDelta.y += func_002982F0(ai->m_ConditionTimer * 0.017453294f) * 61.44f + 51.199997f;
    v = Math_Approach(self->m_vVelocity.y, t.m_Path.m_vDelta.y, 10.24f);
    self->m_vVelocity.y = v;
    if (v > 40.96f) {
        self->m_vVelocity.y = 40.96f;
    } else if (v < -40.96f) {
        self->m_vVelocity.y = -40.96f;
    }
    if (t.m_Path.m_vPos.y <= self->header.m_vPos.y + self->m_vVelocity.y) {
        self->m_vVelocity.y = t.m_Path.m_vDelta.y;
        if (ai->m_Mode != 0x3C) {
            CAI__SetMode(self, ai, 0x3C);
        }
    }
}

/* Raises the aggression by 25, up to 255. */
void EnemyMode_ReactionExplosion_Enter(sptr arg0, CAI *arg1) {
    s32 v = arg1->m_Aggression;
    v += 0x19;
    if (v >= 0x100) {
        v = 0xFF;
    }
    arg1->m_Aggression = v;
}

void EnemyMode_ReactionExplosion_Update(sptr arg0, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, ai, 0x1E);
    }
}

/* AI callback: raises the 0xC8 level by 10 (at most 255) and sets the timer
 * from it, 0..255 -> 240..120. The float temporaries (240 passed through two
 * variables) are what gives the ROM its alternating $f0/$f1 allocation. */
void EnemyMode_ReactionFallDown_Enter(void *self, CAI *ai) {
    s32 level = ai->m_Aggression;
    f32 t;
    f32 scale;
    f32 v;

    scale = 1.0f / 255.0f;
    level += 10;
    if (level >= 0x100) {
        level = 0xFF;
    }
    ai->m_Aggression = level;
    v = ai->m_Aggression * scale;
    t = v;
    t *= -120.0f;
    v = 240.0f;
    scale = v;
    v = t + scale;
    ai->m_ConditionTimer = v;
}

void EnemyMode_ReactionFallDown_Update(sptr arg0, CAI *arg1) {
    f32 temp_f0;

    if (arg1->m_ConditionTimer > 240.0f) {
        arg1->m_ConditionTimer = 240.0f;
    }
    temp_f0 = arg1->m_ConditionTimer - gFrameIncrement;
    arg1->m_ConditionTimer = temp_f0;
    if (temp_f0 <= 0.0f) {
        CAI__SetMode(arg0, arg1, 0x1E);
    }
}

void EnemyMode_ReactionGetup_Update(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(obj, ai, 0xA);
    }
}

/* AI reaction (same code as EnemyAI_OnHit case 10): raises the intensity by 10
 * (at most 255) and delays the next reaction by 105..60 frames. */
void EnemyMode_ReactionBlinded_Enter(void *actor, CAI *ai) {
    s32 v = ai->m_Aggression;
    f32 level;

    v += 10;
    if (v >= 0x100) {
        v = 0xFF;
    }
    ai->m_Aggression = v;
    level = ai->m_Aggression;
    level *= (1.0f / 255);
    level = level * -45.0f + 105.0f;
    ai->m_ConditionTimer += level;
}

typedef struct {
    /* 0x00 */ char pad[0x64];
    /* 0x64 */ f32 v;
} Obj64;

/* Has the shape of an AI step function, stepFunc(pThis, pAI) in LibTEngine:
 * clamp a timer at 0x64, count it down, and change mode when it runs out. */
void EnemyMode_ReactionBlinded_Update(void *self, Obj64 *o) {
    if (o->v > 105.0f) {
        o->v = 105.0f;
    }
    o->v -= gFrameIncrement;
    if (o->v <= 0.0f) {
        CAI__SetMode(self, o, 10);
    }
}

/* Raises the aggression by 10, up to 255, and clears the condition timer. */
void EnemyMode_ReactionShocked_Enter(sptr arg0, CAI *arg1) {
    s32 v = arg1->m_Aggression;
    v += 0xA;
    if (v >= 0x100) {
        v = 0xFF;
    }
    arg1->m_Aggression = v;
    arg1->m_ConditionTimer = 0;
}

void DamageEvent_Set(DamageEvent *event, void *target, s32 damage, f32 radius, s32 flags, void *shield);
void CAI__CallHurt(CGameObjectInstance *self, CAI *ai, DamageEvent *event);

void EnemyMode_ReactionShocked_Update(CGameObjectInstance *self, CAI *ai) {
    DamageEvent event;

    if (ai->m_ConditionTimer > 300.0f) {
        ai->m_ConditionTimer = 300.0f;
    }
    DamageEvent_Set(&event, NULL, gFrameIncrement * 34.133335f, 51.199997f, 0x100000, NULL);
    CAI__CallHurt(self, ai, &event);
    Sound__Play(0x227, self->header.m_vPos, 0, -1);
    ai->m_ConditionTimer -= gFrameIncrement;
    if (ai->m_ConditionTimer <= 0.0f) {
        CAI__SetMode(self, ai, 10);
    }
}

/* AI callback: raises the intensity by 20 (at most 255) and sets the timer
 * from it, 0..255 -> 75..60 (same shape as EnemyMode_ReactionBoreClunk_Enter). */
void EnemyMode_ReactionBore_Enter(void *actor, CAI *ai) {
    s32 v = ai->m_Aggression;
    f32 level;

    v += 20;
    if (v >= 0x100) {
        v = 0xFF;
    }
    ai->m_Aggression = v;
    level = ai->m_Aggression;
    level *= (1.0f / 255);
    level = level * -15.0f + 75.0f;
    ai->m_ConditionTimer = level;
}

void EnemyMode_ReactionBore_Update(CGameObjectInstance *self, CAI *ai) {
    CEnemyIntelligence *data = &self->header.m_pIntelligence->m_Enemy;

    if (ai->m_ConditionTimer > 75.0f) {
        ai->m_ConditionTimer = 75.0f;
    }
    ai->m_ConditionTimer -= gFrameIncrement;
    if (ai->m_ConditionTimer <= 0.0f) {
        ai->m_Health = 0;
        TotemDefense_AddKill();
        if (data->m_SideDeathModel != -1) {
            CAI__SetMode(self, ai, 0x38);
        } else {
            CAI__SetMode(self, ai, 0x34);
        }
    }
}

/* AI reaction (like EnemyAI_OnHit case 11, but setting the delay): raises the
 * intensity by 20 (at most 255) and sets the reaction delay to 75..60 frames. */
void EnemyMode_ReactionBoreClunk_Enter(void *actor, CAI *ai) {
    s32 v = ai->m_Aggression;
    f32 level;

    v += 20;
    if (v >= 0x100) {
        v = 0xFF;
    }
    ai->m_Aggression = v;
    level = ai->m_Aggression;
    level *= (1.0f / 255);
    level = level * -15.0f + 75.0f;
    ai->m_ConditionTimer = level;
}

void EnemyMode_ReactionBoreClunk_Update(sptr arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, arg1, 0xA);
    }
}

void EnemyMode_WoundedArm_Update(sptr arg0, CAI *arg1) {
    if (arg1->m_AnimRepeatedCount != 0) {
        EnemyMode_Move_Update(arg0, (sptr) arg1);
    }
}

void EnemyMode_FeignDeath_Enter(void) {

}

void EnemyMode_FeignDeath_Update(sptr arg0, CAI *ai) {
    s32 var_a2;

    if (ai->m_AnimRepeatedCount != 0) {
        var_a2 = 0x3C;
        if (ai->m_Health != 0) {
            var_a2 = 0x1E;
        }
        CAI__SetMode(arg0, ai, var_a2);
    }
}

/* De-faked in round 21 (j4): the N64 draft tested the CAI pointer in two identical arms (which
 * kept the beq's delay slot empty) and had a separate host body; the test on mode 0x35 first gives
 * the ROM's layout, so the N64 and the host share this body. */

/* The mode-table animation picker of the gib deaths (modes 0x35/0x36): the definition's gib
 * model, + 1 in mode 0x35. */
s32 EnemyAnim_ExtremeDeath(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_ModeBefore == 0x35) {
        return obj->header.m_pIntelligence->m_Enemy.m_GibModel + 1;
    }
    return obj->header.m_pIntelligence->m_Enemy.m_GibModel;
}

s16 EnemyAnim_PfmDeath(CGameObjectInstance *arg0) {
    return arg0->header.m_pIntelligence->m_Enemy.m_DeathAnims[0];
}

s16 EnemyAnim_HeadBlownOffDeath(CGameObjectInstance *arg0) {
    return arg0->header.m_pIntelligence->m_Enemy.m_DeathAnims[1];
}

s16 EnemyAnim_LeftArmBlownOffDeath(CGameObjectInstance *obj) {
    return obj->header.m_pIntelligence->m_Enemy.m_DeathAnims[2];
}

s16 EnemyAnim_RightArmBlownOffDeath(CGameObjectInstance *arg0) {
    return arg0->header.m_pIntelligence->m_Enemy.m_DeathAnims[3];
}

s16 EnemyAnim_BodyHoleDeath(CGameObjectInstance *arg0) {
    return arg0->header.m_pIntelligence->m_Enemy.m_DeathAnims[4];
}

void EnemyMode_PlayerRide_Update(CGameObjectInstance *self, CAI *ai) {
    CPlayer *player = (CPlayer *)ai->m_Enemy.m_pGrabbed->m_AI.m_pParent;

    self->m_dwFlags &= ~0x20000;
    self->m_dwFlags &= ~0x10000;
    ai->m_pfDraw = 0;
    if (ai->m_Mode != 0x2E || ai->m_AnimRepeatedCount != 0) {
        if (player->m_Mode == 0x21) {
            CAI__SetMode(self, ai, 0x2C);
        } else if (player->m_Mode == 0x22) {
            CAI__SetMode(self, ai, 0x2D);
        }
    }
}

#include "structs/camera.h"

/* Places a projectile at its owner's muzzle: inherits the shooter's rotation,
 * yaws it by its own angle and pushes it 10.24 units along the result. */


void CMtxF__TransformPoint(CMtxF *m, CVector3 *in, CVector3 *out);

void Projectile_PlaceAtMuzzle(CGameObjectInstance *self, CAI *ai) {
    CMtxF m;
    CVector3 dir;
    CQuaternion yaw;
    CQuaternion rot;
    CGameObjectInstance *shooter = (CGameObjectInstance *) ai->m_Enemy.m_pGrabbed;
    CPlayer *owner = (CPlayer *) shooter->m_AI.m_pParent;
    f32 s;

    CVector3__Scale(&self->m_vScale, &owner->m_vOnEnemyScale, 0.30719998f);   /* LibTEngine's name; a velocity here */
    self->m_RotY = shooter->m_RotY;
    self->m_qGround = shooter->m_qGround;
    self->header.m_vPos = owner->m_pCamera->m_vPos;
    self->header.m_pCurrentRegion = (sptr) shooter->header.m_pCurrentRegion;
    s = func_002982F0(self->m_RotY * 0.5f);
    yaw.x = 0.0f;
    yaw.y = s;
    yaw.z = 0.0f;
    gQuaternionSin = s;
    yaw.w = func_002974C0(self->m_RotY * 0.5f);
    CQuaternion__Multiply(&rot, &yaw, &self->m_qGround);
    CQuaternion__ToMatrix(&rot, &m);
    CMtxF__TransformPoint(&m, &sProjectileMuzzleOffset, &dir);
    CVector3__Scale(&dir, &dir, 10.24f);
    CVector3__Add(&self->header.m_vPos, &self->header.m_vPos, &dir);
}

#include "globals/controller.h"

/* Three override slots, 6 bytes apart (see EnemyAnim_JoypadTestPlayAnim). */

void EnemyMode_JoypadTestIdle_Update(sptr arg0, sptr arg1) {
    if ((gContPadData[1].button != 0) || (gContPadData[2].button != 0) || (gContPadData[3].button != 0)) {
        CAI__SetMode(arg0, arg1, 0x32);
    }
}

void EnemyAI_JoypadTestPlayAnimUpdate(CGameObjectInstance *obj, CAI *ai) {
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(obj, ai, 0x31);
    }
}

void EnemyAI_NukedFrozenEnter(CGameObjectInstance *obj, CAI *ai) {
    ai->m_pfDraw = EnemyAI_DrawWithBlend;
    ai->m_OnFireTimer = 75.0f;
}

void EnemyAI_NukedFrozenUpdate(sptr arg0, CAI *arg1) {
    if (arg1->m_OnFireTimer <= 0.0f) {
        AI_StopSound(arg0, (sptr) arg1);
    }
    if (arg1->m_ModeTime >= 150.0f) {
        EnemyAI_NukedFrozenRemove(arg0, (sptr) arg1);
    }
}

/* On CGameObjectInstance since round 29 (the N64 and port views, Actor, read only the position,
 * the definition and m_dwFlags). */
void EnemyAI_NukedFrozenRemove(CGameObjectInstance *self, CAI *ai) {
    CIntelligence *def;

    Actor_SpawnParticle(&self->header, 0x17, self->header.m_vPos, 0);
    TotemDefense_AddKill();
    def = self->header.m_pIntelligence;
    if (!(self->m_dwFlags & 0x80000)) {
        if (!(def->m_dwTypeFlags & 0x20)) {
            CScene__SetInstanceSavedBit(&gScene, self, 1);
        }
        GameObject_FireTriggers(self, 1, (sptr) self);
    }
    self->m_dwFlags &= ~0x100;
    self->m_dwFlags &= ~0x2000;
    AI_StopSound(self, ai);
    if (self->m_dwFlags & 0x80000) {
        CListedInstance__Free(self);
    }
}

void func_002601C4(void) {

}

/* m_VarBuffer[6] holds a float timer here. */
s32 EnemyAI_TryTeleportOut(CGameObjectInstance *arg0, CAI *arg1) {
    f32 temp_f0;
    s8 temp_v1;

    if (arg0->header.m_pIntelligence->m_Enemy.m_dwFlags & 0x40) {
        temp_f0 = *(f32 *) &arg1->m_VarBuffer[6] - gFrameIncrement;
        *(f32 *) &arg1->m_VarBuffer[6] = temp_f0;
        if (temp_f0 < 0.0f) {
            *(f32 *) &arg1->m_VarBuffer[6] = 0.0f;
        }
        if (arg1->m_Health != 0) {
            if (*(f32 *) &arg1->m_VarBuffer[6] <= 0.0f) {
                temp_v1 = arg1->m_Mode;
                if ((temp_v1 == 4) || (temp_v1 == 0xE) || (temp_v1 == 0xF) || (temp_v1 == 0x10) || (temp_v1 == 0x11) || (temp_v1 == 0x12) || (temp_v1 == 0xA)) {
                    *(f32 *) &arg1->m_VarBuffer[6] = Random_Blend(75.0f, 1.5e2f);
                    CAI__SetMode(arg0, arg1, 0x17);
                    return 1;
                }
                /* Duplicate return node #14. Try simplifying control flow for better match */
                return 0;
            }
            goto block_13;
        }
        /* Duplicate return node #14. Try simplifying control flow for better match */
        return 0;
    }
block_13:
    return 0;
}

void EnemyAI_RaiseAggression(s32 arg0, CAI *arg1, s32 arg2) {
    s32 v = arg1->m_Aggression;

    v += arg2;
    if (v >= 0x100) {
        v = 0xFF;
    }
    arg1->m_Aggression = v;
}

/* Lowers the aggression by arg2, down to 0. */
void EnemyAI_LowerAggression(s32 arg0, CAI *arg1, s32 arg2) {
    s32 var_v0;

    var_v0 = arg1->m_Aggression - arg2;
    if (var_v0 < 0) {
        var_v0 = 0;
    }
    arg1->m_Aggression = (u8) var_v0;
}

s32 EnemyAI_HasComboEndAnim(CGameObjectInstance *arg0, CAI *arg1) {
    CEnemyIntelligence *temp_a0;

    temp_a0 = &(arg0->header.m_pIntelligence)->m_Enemy;
    if (((arg1->m_Enemy.m_Behavior[0] != 0) && (temp_a0->m_wComboEndAnimFlags & 0x1F)) || ((arg1->m_Enemy.m_Behavior[1] != 0) && (temp_a0->m_wComboEndAnimFlags & 0x3E0)) || ((arg1->m_Enemy.m_Behavior[2] != 0) && (temp_a0->m_wComboEndAnimFlags & 0x7C00))) {
        return 1;
    }
    return 0;
}

/* Whether the current behaviour (m_Enemy.m_BehaviorIndex, 0-2) has an animation in the body's
 * flags at 0x7E (m_wAlertAnimFlags: five bits per behaviour); never in modes 0x20 and 0x3D.
 * One body for the N64 and the port since round 29 (the N64 text read the definition and the
 * AI at their N64 offsets, so the port had its own copy). */
s32 EnemyAI_CurrentHasComboEndAnim(CGameObjectInstance *arg0, CAI *arg1) {
    CEnemyIntelligence *p = &arg0->header.m_pIntelligence->m_Enemy;
    s8 c = arg1->m_Mode;

    if (c == 0x20 || c == 0x3D) {
        return 0;
    }
    switch (arg1->m_Enemy.m_BehaviorIndex) {
        case 0:
            return (p->m_wAlertAnimFlags & 0x1F) != 0;
        case 1:
            return (p->m_wAlertAnimFlags & 0x3E0) != 0;
        case 2:
            return (p->m_wAlertAnimFlags & 0x7C00) != 0;
    }
    return 0;
}

/* AI callback: turns toward the target, 60 degrees at a time in modes 1-2,
 * else 15. */
void EnemyAI_TurnToTarget(CGameObjectInstance *obj, CAI *ai) {
    CAITargets t;
    f32 rate;

    AI_UpdateTargets(obj, ai, &t, 0, 1);
    rate = 0.261799425f; /* 15 deg */
    if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
        rate = 1.0471977f; /* 60 deg */
    }
    /* FAKEMATCH: the identical branches are merged by cross-jumping; the extra
     * use of obj only gives it the register priority it has in the ROM ($s0
     * for obj, $s1 for ai). */
    if (obj->header.m_pIntelligence != NULL) {
        AI_TurnClamped(obj, ai, rate, t.m_Sight.m_Angle);
    } else {
        AI_TurnClamped(obj, ai, rate, t.m_Sight.m_Angle);
    }
}

void EnemyAI_TurnToPoint(sptr arg0, CAI *arg1) {
    f32 var_f20;

    var_f20 = 0.26179942f;
    if ((u32) ((u8) arg1->m_ModeBefore - 1) < 2U) {
        var_f20 = 1.0471977f;
    }
    AI_TurnClamped(arg0, arg1, var_f20, AI_AngleToPoint(arg0, gEnemyAlertPoint));
}

void EnemyAI_SaveDeadAndFireTriggers(CGameObjectInstance *arg0) {
    CIntelligence *sub = arg0->header.m_pIntelligence;

    if (!(arg0->m_dwFlags & 0x80000)) {
        if (!(sub->m_dwTypeFlags & 0x20)) {
            CScene__SetInstanceSavedBit(&gScene, arg0, 1);
        }
        GameObject_FireTriggers(arg0, 1, arg0);
    }
}

/* On CGameObjectInstance since round 29 (the N64 and port views, Actor, read only m_dwFlags). */
void EnemyAI_StopSoundAndFree(CGameObjectInstance *self, CAI *ai) {
    self->m_dwFlags &= ~0x100;
    self->m_dwFlags &= ~0x2000;
    AI_StopSound(self, ai);
    if (self->m_dwFlags & 0x80000) {
        CListedInstance__Free(self);
    }
}

/* Host body (C23): the draw callback of a melting corpse (EnemyMode 60). The N64 view reads
 * CAI::m_ModeTime at its N64 0x40 (the host CAI has two pointers before it), and the actor and
 * the draw args came in as s32 (with --high-memory the corpse's draw crashed in
 * CGameObjectInstance__DrawModelImmediate). */
void EnemyAI_DrawFadeOut(sptr arg0, CAI *ai, sptr arg2) {
    f32 var_f1;

    var_f1 = 255.0f - (ai->m_ModeTime * 8.5f);
    gBlendState = 1;
    if (var_f1 < 0.0f) {
        var_f1 = 0.0f;
    }
    gBlendState_m_Color[3] = var_f1;
    CGameObjectInstance__DrawModelImmediate(arg0, (sptr) ai, arg2);
}
