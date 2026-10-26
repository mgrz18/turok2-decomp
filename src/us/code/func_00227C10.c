/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SYSTEM/802624A0_10.CPP, func_80262CE0_de, with Turok 2's layouts. */

#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct IntrusiveList {
    /* 0x00 */ void *head;
    /* 0x04 */ void *tail;
    /* 0x08 */ s32 prevOffset;
    /* 0x0C */ s32 nextOffset;
    /* 0x10 */ s32 count;
} IntrusiveList;

typedef struct SceneEffect {
    /* 0x000 */ u8 pad0[0x290];
    /* 0x290 */ s32 *ref;
    /* 0x294 */ u8 pad294[4];
} SceneEffect;                          /* 0x298 bytes */

typedef struct SceneEffectPool {
    /* 0x0000 */ SceneEffect effects[0x20];
    /* 0x5300 */ IntrusiveList freeEffects;
    /* 0x5314 */ IntrusiveList activeEffects;
} SceneEffectPool;

/* The animation header: what this reads of it. */
typedef struct AnimInfo {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ u16 intelligence;
    /* 0x1E */ u16 angle;
} AnimInfo;

/* The world: what this reads of it. */
typedef struct World {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 base;
    /* 0x18 */ u8 pad18[0x40 - 0x18];
    /* 0x40 */ s32 *table;
} World;

extern World D_800F7078;
extern s32 D_800B6D54;                  /* effects enabled */
extern s32 D_800B6D00;
extern s32 D_8011001C;
extern char D_800A6070[];               /* "anim object index" */
extern char D_800A6084[];               /* "anim object info" */

s32 func_00224180(World *world, s32 id);
s32 func_00201848(s32 *table, s32 base, s32 index);
s32 *func_0020367C(s32 heap, s32 src, s32 size, s32 type, s32 arg4, s32 callback, char *name, s32 arg7);
s32 func_0020185C(s32 table, s32 base, s32 index, s32 *size);
void *func_002017D4(s32 table, s32 index);
s32 func_00225E88(World *world, s32 index);
f32 func_0026D210(s32 angle);
void func_0020EF60(Vec3 *out, Vec3 *in, f32 angle);
void func_002051F4(s32 heap, s32 *block);
void CList__Remove(IntrusiveList *list, void *node);
void CList__AddHead(IntrusiveList *list, void *node);
void func_00219BC0(SceneEffect *effect, s32 id, s32 variant, s32 unused, s32 count, s32 sound, s32 owner,
                   f32 scale, Vec3 position, s32 flags, Vec3 color, Vec3 direction, s32 arg12);
void func_0021E5D4(SceneEffect *effect);

/* Takes a node from the free list when allowed (always with D_8011001C, otherwise while fewer than
 * three are active), moves it to the active list and attaches the counted reference. */
static __inline__ SceneEffect *take_effect(SceneEffectPool *scene, s32 *ref) {
    SceneEffect *effect;

    if (D_8011001C == 0 && (u32)scene->activeEffects.count >= 3) {
        return NULL;
    }
    effect = scene->freeEffects.head;
    if (effect != NULL) {
        CList__Remove(&scene->freeEffects, effect);
        CList__AddHead(&scene->activeEffects, effect);
        effect->ref = ref;
        if (ref != NULL) {
            *ref += 1;
        }
    }
    return effect;
}

static __inline__ SceneEffect *spawn_effect(SceneEffectPool *scene, s32 id, s32 variant, s32 count, s32 owner,
                                            Vec3 position, f32 scale, Vec3 direction, Vec3 color, s32 *ref) {
    SceneEffect *effect;

    if (D_800B6D54 == 0 || count == 0) {
        return NULL;
    }
    effect = take_effect(scene, ref);
    if (effect == NULL) {
        return NULL;
    }
    func_00219BC0(effect, id, variant, D_800B6D00, count, -1, owner, scale, position, 0x80000, color, direction, 0);
    func_0021E5D4(effect);
    return effect;
}

/* Spawns an effect of the given type in a scene: loads the type's animation index and header
 * ("anim object index", "anim object info"), takes its intelligence from the header and turns
 * color by the header's angle, then spawns it (see spawn_effect). Returns the effect or NULL. */
SceneEffect *func_00227C10(SceneEffectPool *scene, s32 variant, s32 owner, Vec3 position, f32 scale,
                           Vec3 direction, Vec3 color, s32 *ref) {
    SceneEffect *effect;
    AnimInfo *info;
    s32 *index;
    s32 *block;
    s32 id;
    s32 model;
    s32 src;
    s32 count;
    s32 size;

    effect = NULL;
    id = func_00224180(&D_800F7078, variant);
    if (id != -1) {
        src = func_00201848(D_800F7078.table, D_800F7078.base, id);
        index = func_0020367C(0, src, 0x18, 0, 0, 0, D_800A6070, 1);
        if (index != NULL) {
            block = func_0020367C(0, func_0020185C(*index, src, 0, &size), size, 0, 0, 0, D_800A6084, 1);
            if (block != NULL) {
                info = func_002017D4(*block, 0);
                count = func_00225E88(&D_800F7078, info->intelligence);
                func_0020EF60(&color, &color, func_0026D210(info->angle));
                model = id;     /* a copy: the ROM allocates it apart from id */
                effect = spawn_effect(scene, model, variant, count, owner, position, scale, direction, color, ref);
                func_002051F4(0, block);
            }
            func_002051F4(0, index);
        }
    }
    return effect;
}
