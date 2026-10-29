#include "common.h"

/* Drops an actor's loot at its feet (raised to the ground height): kind 0
 * spawns the definition's object (or, for a pickup-type entry, a pool pickup
 * if it is unlocked), kind 1 a particle effect, kind 2 a pickup picked at
 * random from the weapon/ammo/item tables the definition enables. */

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    f32 x, y, z, w;
} Quatf;

typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 dropKind;
    /* 0x08 */ u8 pad8[0x14 - 0x8];
    /* 0x14 */ s16 dropId;
    /* 0x16 */ s16 dropModel;
    /* 0x18 */ Vec3f dropScale;
    /* 0x24 */ s16 dropEffect;
} ActorInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ ActorInfo info;
} ActorDef;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ s32 region;
    /* 0x14 */ ActorDef *def;
    /* 0x18 */ Vec3f vel;
    /* 0x24 */ u8 pad24[0x40 - 0x24];
    /* 0x40 */ Quatf rot;
    /* 0x50 */ f32 rotY;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x10C];
    /* 0x10C */ s32 spawned;
    /* 0x110 */ s32 drops;
} CAI;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3f pos;
} PoolActor;

typedef struct {
    s16 id;
    s16 weight;
} Pick;

typedef struct {
    s16 count;
    Pick entries[0x18];
} PickTable;

typedef struct {
    /* 0x00 */ u8 pad0[0x19];
    /* 0x19 */ u8 multiplayer;
} GameFlags;

extern GameFlags D_8011AAD8;
extern s32 D_8011001C;
extern Pick D_800B1EE0[];
extern Pick D_800B1F38[];
extern Pick D_800B1F5C[];
extern Pick D_800B1F6C[];
extern Pick D_800B1F78[];
extern Pick D_800B1F88[];
extern Pick D_800B1FE4[];
extern u8 D_800F7078[];
extern u8 D_800F8928[];
extern u8 D_80107590[];
extern u8 D_8010ABA0[];

extern f32 func_0026BE60(s32 region, f32 x, f32 z);
extern s32 *func_00225E88(void *level, s32 model);
extern s32 func_002241C0(void *level, s32 id);
extern s32 func_00262C20(s32 id);
extern PoolActor *func_00227240(void *pool, s32 *refCount, s32 id, Vec3f vel, Vec3f pos, s32 region, f32 lifetime);
extern void *func_00228108(void *world, s32 model, s32 type, void *intelligence, s32 region, Vec3f pos, f32 rotY,
                           Vec3f vel, Vec3f scale, s32 *owner);
extern s32 func_0022F350(void *sys, Actor *p, Actor *owner, s32 *ownerId, s32 target, s32 type, Vec3f dir,
                         Quatf rot, Vec3f pos, s32 region, s32 arg10, s32 flags);
extern void func_00246690(PoolActor *actor, s32 sound, Vec3f pos, s32 arg);
extern void *func_002532A8(void *players);
extern s32 func_00253C14(void *player, s32 id);
extern s32 func_00253CF0(void *player, s32 id);
extern s16 func_0026D518(Pick *table);
extern void func_0026D5DC(PickTable *t);
extern s32 func_0026D5E4(PickTable *t, s16 id, s16 weight);
extern s16 func_0026D628(PickTable *t);

/* Spawns pool pickup id at pos. */
static __inline__ void spawnPickup(Actor *actor, CAI *ai, Vec3f pos, s32 region, s32 id) {
    PoolActor *pickup;

    pickup = func_00227240(D_80107590, &ai->spawned, id, actor->vel, pos, region, 0.0f);
    if (pickup != NULL) {
        ai->drops--;
        func_00246690(pickup, 0x11D, pickup->pos, 0);
    }
}

/* Spawns the object model (an intelligence entry) at pos. */
static __inline__ void spawnObject(Actor *actor, CAI *ai, Vec3f pos, Vec3f scale, s32 region, s32 type, s32 *model) {
    if (func_00228108(D_8010ABA0, type, 0, model, region, pos, actor->rotY, actor->vel, scale, &ai->spawned)) {
        ai->drops--;
    }
}

/* Spawns the effect at pos. */
static __inline__ void spawnEffect(Actor *actor, CAI *ai, Vec3f pos, s32 region, s32 type) {
    ai->drops -= func_0022F350(D_800F8928, actor, actor, &ai->spawned, 0, type, actor->vel, actor->rot, pos, region,
                               -1, 0);
}

void func_00262D74(Actor *actor, CAI *ai) {
    Vec3f pos;
    Vec3f scale;
    PickTable picks;
    PickTable unlocked;
    ActorInfo *info;
    s32 region;
    s32 *model;
    s32 id;
    f32 ground;
    s32 weapon;
    s32 ammo;
    s32 item;
    u16 pickId; /* loaded as u16, passed as s16: the ROM's lhu/sll/sra */
    GameFlags *flags;
    Pick *e;
    void *player;

    info = &actor->def->info;
    pos = actor->pos;
    region = actor->region;
    if (region != 0) {
        ground = func_0026BE60(region, actor->pos.x, actor->pos.z);
        if (pos.y < ground) {
            pos.y = ground;
        }
    }
    switch (info->dropKind) {
    case 0:
        model = func_00225E88(D_800F7078, info->dropModel);
        switch (*model) {
        case 8:
            id = func_002241C0(D_800F7078, info->dropId);
            if (func_00262C20(id)) {
                spawnPickup(actor, ai, pos, region, id);
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 9:
        case 10:
            scale.x = info->dropScale.x;
            scale.y = info->dropScale.y;
            scale.z = info->dropScale.z;
            spawnObject(actor, ai, pos, scale, region, info->dropId, model);
            break;
        }
        break;
    case 1:
        spawnEffect(actor, ai, pos, region, info->dropEffect);
        break;
    case 2:
        weapon = -1;
        ammo = -1;
        item = -1;
        flags = &D_8011AAD8;
        if (flags->multiplayer) {
            if (info->flags & 0x20) {
                weapon = func_0026D518(D_800B1F38);
            }
            if (info->flags & 0x40) {
                ammo = func_0026D518(D_800B1F78);
            }
            if (info->flags & 0x80) {
                func_0026D5DC(&unlocked);
                for (e = D_800B1FE4; e->id != -1; e++) {
                    pickId = e->id;
                    if (func_00262C20((s16)pickId)) {
                        func_0026D5E4(&unlocked, e->id, e->weight);
                    }
                }
                item = func_0026D628(&unlocked);
            }
        } else {
            player = func_002532A8((u8 *)flags - 0x1268); /* the player list, D_80119870 */
            if (player == NULL) {
                break;
            }
            if (info->flags & 0x20) {
                func_0026D5DC(&picks);
                for (e = D_800B1EE0; e->id != -1; e++) {
                    pickId = e->id;
                    if (func_00253C14(player, (s16)pickId)) {
                        func_0026D5E4(&picks, e->id, e->weight);
                    }
                }
                weapon = func_0026D628(&picks);
            }
            if (info->flags & 0x40) {
                func_0026D5DC(&picks);
                e = D_800B1F5C;
                if (D_8011001C) {
                    e = D_800B1F6C;
                }
                for (; e->id != -1; e++) {
                    pickId = e->id;
                    if (func_00253CF0(player, (s16)pickId)) {
                        func_0026D5E4(&picks, e->id, e->weight);
                    }
                }
                ammo = func_0026D628(&picks);
            }
            if (info->flags & 0x80) {
                item = func_0026D518(D_800B1F88);
            }
        }
        func_0026D5DC(&picks);
        if (weapon != -1) {
            func_0026D5E4(&picks, weapon, 100);
        }
        if (ammo != -1) {
            func_0026D5E4(&picks, ammo, 100);
        }
        if (item != -1) {
            func_0026D5E4(&picks, item, 100);
        }
        if (picks.count != 0) {
            spawnPickup(actor, ai, pos, region, func_0026D628(&picks));
        }
        break;
    }
}
