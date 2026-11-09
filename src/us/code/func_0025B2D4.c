#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/intelligence.h"
#include "structs/collision.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/ai.h"
#include "protos/ai.h"

typedef struct Object Object;

/* An object or a particle: the first byte tells which. */
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

void EnemyAI_NukedFrozenRemove(Object *obj, CAI *ai);
void AI_SetTargetOverride(Object *obj, CAI *ai, Object *owner, f32 time);
void AI_HearNoise(Object *obj, CAI *ai, CVector3 *from, s32);
void Sound__PlayPitched(s16 sound, CVector3 pos, s32, s32, f32 volume);
s32 Random_Next(void);
void AI_StopSound(Object *obj, CAI *ai);
s32 EnemyAI_PickDeathMode(Object *obj, CAI *ai, DamageEvent *dmg);
void TotemDefense_AddKill(void);
s32 EnemyAnim_PickDeath(Object *obj, CAI *ai);
s32 CGameObjectInstance__PickRandomAnim(Object *obj, s32 node, s32);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
f32 func_00298470(f32 x);
f32 CVector3__GetYaw(CVector3 *v);

/* An object's owner (0x1A8) lies inside its AI block. */
#define OWNER(o) (*(Object **)((u8 *)(o) + 0x1A8))

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
    s32 slot;
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
