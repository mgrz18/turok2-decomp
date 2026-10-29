#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct Object Object;

typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x30 - 0x4];
    /* 0x30 */ u16 hurtSound;
} ActorInfo;

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ ActorInfo info;
    /* 0x48 */ u8 pad48[0x96 - 0x48];
    /* 0x96 */ u16 headshotModes;   /* hit-region reactions, 5 bits per mode slot */
    /* 0x98 */ u16 headshotModes2;
} ActorDef;

typedef struct AI {
    /* 0x000 */ u32 flags;
    /* 0x004 */ s32 health;
    /* 0x008 */ u8 pad8[0x34 - 0x8];
    /* 0x034 */ s8 state;
    /* 0x035 */ u8 pad35[0x40 - 0x35];
    /* 0x040 */ s32 unk40;
    /* 0x044 */ u8 pad44[0x80 - 0x44];
    /* 0x080 */ Object *target;
    /* 0x084 */ u8 pad84[4];
    /* 0x088 */ Object *attacker;
    /* 0x08C */ u8 pad8C[0xC4 - 0x8C];
    /* 0x0C4 */ s16 node;
    /* 0x0C6 */ s8 nodeIndex;
    /* 0x0C7 */ s8 grounded;
    /* 0x0C8 */ u8 pain;
    /* 0x0C9 */ u8 padC9[2];
    /* 0x0CB */ s8 age;
    /* 0x0CC */ u8 padCC[0xD0 - 0xCC];
    /* 0x0D0 */ f32 burnTime;
    /* 0x0D4 */ u8 padD4[0x133 - 0xD4];
    /* 0x133 */ s8 modeIndex;
} AI;

/* An object or a particle: the first byte tells which. */
struct Object {
    /* 0x000 */ u8 kind;            /* 1: object, 2: particle */
    /* 0x001 */ u8 pad1;
    /* 0x002 */ u16 particleType;
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ u8 pad18[0xB8 - 0x18];
    /* 0x0B8 */ u16 typeId;
    /* 0x0BA */ u8 padBA[0xD4 - 0xBA];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x10C - 0xD8];
    /* 0x10C */ Object *particleOwner;
    /* 0x110 */ u8 pad110[0x140 - 0x110];
    /* 0x140 */ AI ai;
    /* 0x274 */ u8 pad274[0x27C - 0x274];
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

void func_002600E8(Object *obj, AI *ai);
void func_002460FC(Object *obj, AI *ai, Object *owner, f32 time);
void func_0025E52C(Object *obj, AI *ai, Vec3f *from, s32);
void func_00243414(Object *obj, AI *ai, s32 state);
void func_002755B0(s16 sound, Vec3f pos, s32, s32, f32 volume);
s32 func_002113A4(void);
void func_00246918(Object *obj, AI *ai);
s32 func_0025B078(Object *obj, AI *ai, Damage *dmg);
void func_004089C8(void);
s32 func_0025DF04(Object *obj, AI *ai);
s32 func_00219F74(Object *obj, s32 node, s32);
void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);
f32 func_00298470(f32 x);
f32 func_0020EAA0(Vec3f *v);

/* An object's owner (0x1A8) lies inside its AI block. */
#define OWNER(o) (*(Object **)((u8 *)(o) + 0x1A8))

/* AI damage: retarget the attacker, take the health, play the hurt sound and
 * pick the death, fall or flinch reaction; a knock back turns the actor to
 * face the push when the source was near enough. */
void func_0025B2D4(Object *obj, AI *ai, Damage *dmg) {
    Vec3f delta;
    s32 pushed;
    Object *attacker;
    Object *source;
    Object *owner;
    Object *newTarget;
    s32 left;
    ActorInfo *info;
    ActorDef *def;
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
    index = ai->modeIndex;
    info = &def->info;
    if (ai->state == 0x3D) {
        func_002600E8(obj, ai);
        return;
    }
    source = dmg->source;
    if (source != NULL) {
        switch (source->kind) {
            case 1:
                if ((source->flags & 0x300000) && (u32)(D_80110048 - 7100) <= 400) {
                    newTarget = OWNER(source);
                    if (obj->ai.target != newTarget) {
                        func_002460FC(obj, ai, newTarget, 60.0f);
                        ai->flags &= ~0x40000000;
                    }
                }
                attacker = source;
                func_0025E52C(obj, ai, &source->pos, 3);
                break;
            case 2:
                if (source->particleType == 10) {
                    if ((def->info.flags & 0x80) && ai->health != 0) {
                        func_00243414(obj, ai, 0x1F);
                    }
                    if (info->flags & 0x40000) {
                        dmg->amount += 0x3200;
                        ai->burnTime += 60.0f;
                    }
                }
                owner = source->particleOwner;
                if (owner != NULL) {
                    if ((owner->flags & 0x300000) && (u32)(D_80110048 - 7100) <= 400) {
                        newTarget = OWNER(owner);
                        if (ai->target != newTarget) {
                            func_002460FC(obj, ai, newTarget, 60.0f);
                            ai->flags &= ~0x40000000;
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
            if (attacker->def->kind == 1) {
                ai->target = attacker;
                ai->attacker = attacker;
                ai->flags |= 0x20000000;
                ai->flags |= 0x40000000;
            } else {
                ai->flags &= ~0x20000000;
                ai->flags &= ~0x40000000;
            }
        }
    } else {
        func_0025E52C(obj, ai, (Vec3f *)D_800B06A0, 3);
    }
    health = ai->health;
    left = health - dmg->amount;
    if (left < 0) {
        left = 0;
    }
    ai->health = left;
    dead = left == 0;
    justDied = 0;
    if (dead) {
        justDied = health != 0;
    }
    if ((health != 0 || (ai->state != 0x3C && !ai->grounded)) && info->hurtSound != 0 && ai->age >= 11) {
        if (D_8011AAD8 & 0x40) {
            volume = 2.5f;
        } else if (D_8011AAD8 & 0x20) {
            volume = 0.35f;
        }
        func_002755B0(info->hurtSound, obj->pos, 0, -1, volume);
        ai->age = 0;
    }
    ai->pain = (ai->pain + dmg->amount < 255) ? 255 : ai->pain + dmg->amount;
    if ((dmg->flags & 0x400000) && func_002113A4() % 1000 < 500) {
        dmg->flags = 0x800000;
    }
    if ((dmg->flags & 0x100000) && func_002113A4() % 1000 < 500) {
        dmg->flags = 0x200000;
    }
    ai->flags &= 0xFC0FFFFF;
    ai->flags |= dmg->flags;
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
        if (ai->state != 0x35 && ai->state != 0x3C && ai->grounded && (dmg->flags & 0xC00000)) {
            ai->flags = (ai->flags & 0xFC0FFFFF) | 0x400000;
            node = func_0025DF04(obj, ai);
            if (node != -1) {
                slot = func_00219F74(obj, node, -1);
                if (slot != -1) {
                    pushed = 1;
                    ai->node = node;
                    ai->nodeIndex = slot;
                    ai->grounded = 0;
                    ai->unk40 = 0;
                }
            }
        }
    } else {
        region = dmg->region;
        if (region != NULL && !(ai->state >= 0x1C && ai->state < 0x23) && (region->flags & 3) == 2) {
            if (!(region->flags & 4)) {
                if ((index == 0 && (obj->def->headshotModes & 0x1F)) || (index == 1 && (obj->def->headshotModes & 0x3E0))
                    || (index == 2 && (obj->def->headshotModes & 0x7C00))) {
                    func_00243414(obj, ai, 0x24);
                    return;
                }
            } else {
                if ((index == 0 && (obj->def->headshotModes2 & 0x1F)) || (index == 1 && (obj->def->headshotModes2 & 0x3E0))
                    || (index == 2 && (obj->def->headshotModes2 & 0x7C00))) {
                    func_00243414(obj, ai, 0x25);
                    return;
                }
            }
        }
        if (ai->grounded && (ai->state == 0x11 || ai->state == 0x12)) {
            func_00243414(obj, ai, 4);
        } else if (ai->state != 0x33 && (dmg->flags & 0xC00000) && (info->flags & 0x20000)) {
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
