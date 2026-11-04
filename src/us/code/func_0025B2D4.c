#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/intelligence.h"

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

typedef struct {
    /* 0x00 */ u8 pad0[0x65];
    /* 0x65 */ u8 flags;
} HitRegion;

typedef struct {
    /* 0x00 */ Object *source;
    /* 0x04 */ s32 amount;
    /* 0x08 */ f32 force;
    /* 0x0C */ u32 flags;
    /* 0x10 */ HitRegion *region;
} Damage;

extern s32 D_80110048;          /* level */
extern s32 D_8011001C;
extern s32 D_8011AAD8;          /* cheats */
extern u8 D_800B06A0[];         /* origin */

void func_002600E8(Object *obj, CAI *ai);
void func_002460FC(Object *obj, CAI *ai, Object *owner, f32 time);
void func_0025E52C(Object *obj, CAI *ai, CVector3 *from, s32);
void func_00243414(Object *obj, CAI *ai, s32 state);
void func_002755B0(s16 sound, CVector3 pos, s32, s32, f32 volume);
s32 func_002113A4(void);
void func_00246918(Object *obj, CAI *ai);
s32 func_0025B078(Object *obj, CAI *ai, Damage *dmg);
void func_004089C8(void);
s32 func_0025DF04(Object *obj, CAI *ai);
s32 func_00219F74(Object *obj, s32 node, s32);
void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
f32 func_00298470(f32 x);
f32 func_0020EAA0(CVector3 *v);

/* An object's owner (0x1A8) lies inside its AI block. */
#define OWNER(o) (*(Object **)((u8 *)(o) + 0x1A8))

/* AI damage: retarget the attacker, take the health, play the hurt sound and
 * pick the death, fall or flinch reaction; a knock back turns the actor to
 * face the push when the source was near enough. */
void func_0025B2D4(Object *obj, CAI *ai, Damage *dmg) {
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
    HitRegion *region;

    attacker = NULL;
    pushed = 0;
    def = obj->def;
    volume = 1.0f;
    index = ai->m_Enemy.m_BehaviorIndex;
    info = &def->m_Enemy;
    if (ai->m_Mode == 0x3D) {
        func_002600E8(obj, ai);
        return;
    }
    source = dmg->source;
    if (source != NULL) {
        switch (source->kind) {
            case 1:
                if ((source->flags & 0x300000) && (u32)(D_80110048 - 7100) <= 400) {
                    newTarget = OWNER(source);
                    if (obj->ai.m_pSightTarget != newTarget) {
                        func_002460FC(obj, ai, newTarget, 60.0f);
                        ai->m_dwFlags &= ~0x40000000;
                    }
                }
                attacker = source;
                func_0025E52C(obj, ai, &source->pos, 3);
                break;
            case 2:
                if (source->particleType == 10) {
                    if ((def->m_Enemy.m_dwFlags & 0x80) && ai->m_Health != 0) {
                        func_00243414(obj, ai, 0x1F);
                    }
                    if (info->m_dwFlags & 0x40000) {
                        dmg->amount += 0x3200;
                        ai->m_OnFireTimer += 60.0f;
                    }
                }
                owner = source->particleOwner;
                if (owner != NULL) {
                    if ((owner->flags & 0x300000) && (u32)(D_80110048 - 7100) <= 400) {
                        newTarget = OWNER(owner);
                        if (ai->m_pSightTarget != newTarget) {
                            func_002460FC(obj, ai, newTarget, 60.0f);
                            ai->m_dwFlags &= ~0x40000000;
                        }
                    }
                    func_0025E52C(obj, ai, &source->pos, 3);
                    if (owner->kind == 1) {
                        attacker = owner;
                    }
                }
                break;
            default:
                func_0025E52C(obj, ai, &source->pos, 3);
                break;
        }
        if (!D_8011001C && attacker != NULL && attacker->typeId != obj->typeId) {
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
        func_0025E52C(obj, ai, (CVector3 *)D_800B06A0, 3);
    }
    health = ai->m_Health;
    left = health - dmg->amount;
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
        if (D_8011AAD8 & 0x40) {
            volume = 2.5f;
        } else if (D_8011AAD8 & 0x20) {
            volume = 0.35f;
        }
        func_002755B0(info->m_PainSound, obj->pos, 0, -1, volume);
        ai->m_LastHurtSFXTimer = 0;
    }
    ai->m_Aggression = (ai->m_Aggression + dmg->amount < 255) ? 255 : ai->m_Aggression + dmg->amount;
    if ((dmg->flags & 0x400000) && func_002113A4() % 1000 < 500) {
        dmg->flags = 0x800000;
    }
    if ((dmg->flags & 0x100000) && func_002113A4() % 1000 < 500) {
        dmg->flags = 0x200000;
    }
    ai->m_dwFlags &= 0xFC0FFFFF;
    ai->m_dwFlags |= dmg->flags;
    if (justDied) {
        func_00246918(obj, ai);
        obj->fxFlags |= 8;
        anim = func_0025B078(obj, ai, dmg);
        if (anim == 0x34 && (dmg->flags & 0x400000)) {
            pushed = 1;
        }
        func_00243414(obj, ai, anim);
        func_004089C8();
    } else if (dead) {
        func_00246918(obj, ai);
        if (ai->m_Mode != 0x35 && ai->m_Mode != 0x3C && ai->m_AnimRepeatedCount && (dmg->flags & 0xC00000)) {
            ai->m_dwFlags = (ai->m_dwFlags & 0xFC0FFFFF) | 0x400000;
            node = func_0025DF04(obj, ai);
            if (node != -1) {
                slot = func_00219F74(obj, node, -1);
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
        region = dmg->region;
        if (region != NULL && !(ai->m_Mode >= 0x1C && ai->m_Mode < 0x23) && (region->flags & 3) == 2) {
            if (!(region->flags & 4)) {
                if ((index == 0 && (obj->def->m_Enemy.m_wLeftArmWoundAnimFlags & 0x1F)) || (index == 1 && (obj->def->m_Enemy.m_wLeftArmWoundAnimFlags & 0x3E0))
                    || (index == 2 && (obj->def->m_Enemy.m_wLeftArmWoundAnimFlags & 0x7C00))) {
                    func_00243414(obj, ai, 0x24);
                    return;
                }
            } else {
                if ((index == 0 && (obj->def->m_Enemy.m_wRightArmWoundAnimFlags & 0x1F)) || (index == 1 && (obj->def->m_Enemy.m_wRightArmWoundAnimFlags & 0x3E0))
                    || (index == 2 && (obj->def->m_Enemy.m_wRightArmWoundAnimFlags & 0x7C00))) {
                    func_00243414(obj, ai, 0x25);
                    return;
                }
            }
        }
        if (ai->m_AnimRepeatedCount && (ai->m_Mode == 0x11 || ai->m_Mode == 0x12)) {
            func_00243414(obj, ai, 4);
        } else if (ai->m_Mode != 0x33 && (dmg->flags & 0xC00000) && (info->m_dwFlags & 0x20000)) {
            func_00243414(obj, ai, 0x33);
            pushed = 1;
        }
    }
    if (pushed && source != NULL && dmg->force > 0.0f) {
        func_0020EF2C(&delta, &obj->pos, &source->pos);
        if (func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z) < dmg->force) {
            func_0020EAA0(&delta);
        }
    }
}
