/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SYSTEM/802624A0_10.CPP, func_80262CE0_de, with Turok 2's layouts. */

#include "common.h"
#include "globals/strings.h"
#include "structs/vector.h"
#include "structs/world.h"
#include "structs/list.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "globals/camera.h"
#include "protos/list.h"
#include "protos/iset.h"
#include "protos/cache.h"
#include "protos/scene.h"

/* The animation header: what this reads of it. */
typedef struct AnimInfo {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ u16 intelligence;
    /* 0x1E */ u16 angle;
} AnimInfo;
f32 HalfToFloat(s32 angle);
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 angle);
extern void CGameObjectInstance__Construct(CGameObjectInstance *obj, s32 model, s32 type, s32 unused, void *intelligence,
                          s32 sound, void *region, f32 rotY, CVector3 pos, s32 flags, CVector3 scale,
                          CVector3 velocity, s32 extra);
extern void CGameObjectInstance__InitAIType(CGameObjectInstance *obj);

/* Takes a node from the free list when allowed (always with gScene_m_bStreamAll, otherwise while fewer than
 * three are active), moves it to the active list and attaches the counted reference. */
static __inline__ CListedInstance *take_effect(CInstancePool *scene, s32 *ref) {
    CListedInstance *effect;

    if (gScene.m_bStreamAll == 0 && (u32)scene->m_ActiveList.m_nCount >= 3) {
        return NULL;
    }
    effect = scene->m_FreeList.m_pHead;
    if (effect != NULL) {
        CList__Remove(&scene->m_FreeList, effect);
        CList__AddHead(&scene->m_ActiveList, effect);
        effect->m_pOwnerCount = ref;
        if (ref != NULL) {
            *ref += 1;
        }
    }
    return effect;
}

static __inline__ CListedInstance *spawn_effect(CInstancePool *scene, s32 id, s32 variant, sptr count, sptr owner,
                                            CVector3 position, f32 scale, CVector3 direction, CVector3 color, s32 *ref) {
    CListedInstance *effect;

    if (gPersistentDataLoaded == 0 || count == 0) {
        return NULL;
    }
    effect = take_effect(scene, ref);
    if (effect == NULL) {
        return NULL;
    }
    CGameObjectInstance__Construct((CGameObjectInstance *)effect, id, variant, gDefaultVisBits, (void *)count, -1, (void *)owner, scale,
                                   position, 0x80000, color, direction, 0);
    CGameObjectInstance__InitAIType((CGameObjectInstance *)effect);
    return effect;
}

/* Spawns an effect of the given type in a scene: loads the type's animation index and header
 * ("anim object index", "anim object info"), takes its intelligence from the header and turns
 * color by the header's angle, then spawns it (see spawn_effect). Returns the effect or NULL. */
CListedInstance *CInstancePool__SpawnAnimInstance(CInstancePool *scene, s32 variant, sptr owner, CVector3 position, f32 scale,
                           CVector3 direction, CVector3 color, s32 *ref) {
    CListedInstance *effect;
    AnimInfo *info;
    sptr *index;
    sptr *block;
    s32 id;
    s32 model;
    s32 src;
    sptr count;
    s32 size;

    effect = NULL;
    id = CScene__FindObjectType(&gScene, variant);
    if (id != -1) {
        src = CISet__GetROMAddress(gScene.m_pObjects, gScene.m_ObjectsKey, id);
        index = CCache__Request(0, src, 0x18, 0, 0, 0, "anim object index", 1);
        if (index != NULL) {
            block = CCache__Request(0, CISet__GetROMAddressAndSize(*index, src, 0, &size), size, 0, 0, 0, "anim object info", 1);
            if (block != NULL) {
                info = CISet__GetBlock(*block, 0);
                count = CScene__GetIntelligence(&gScene, info->intelligence);
                CVector3__Scale(&color, &color, HalfToFloat(info->angle));
                model = id;     /* a copy: the ROM allocates it apart from id */
                effect = spawn_effect(scene, model, variant, count, owner, position, scale, direction, color, ref);
                CCache__Unlock(0, block);
            }
            CCache__Unlock(0, index);
        }
    }
    return effect;
}

/* CInstancePool__Alloc without the owner counter. */
static __inline__ CGameObjectInstance *take_free_instance(CInstancePool *world) {
    CGameObjectInstance *node;

    if (gScene.m_bStreamAll != 0 || (u32)world->m_ActiveList.m_nCount < 3) {
        node = (CGameObjectInstance *)world->m_FreeList.m_pHead;
        if (node != NULL) {
            CList__Remove(&world->m_FreeList, node);
            CList__AddHead(&world->m_ActiveList, node);
            ((CListedInstance *)node)->m_pOwnerCount = 0;
        }
        return node;
    }
    return NULL;
}

/* Only the vectors are copied into it; the frame layout needs them in one
 * aggregate. */
typedef struct {
    /* 0x00 */ u8 unk00[0x14];
    /* 0x14 */ CVector3 pos;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ CVector3 velocity;
    /* 0x30 */ CVector3 scale;
} SpawnInfo;

CGameObjectInstance *CInstancePool__SpawnCopy(CInstancePool *world, CGameObjectInstance *src) {
    s32 kind = src->header.m_Type;
    s32 type = (u16)src->m_nTypeFlag;
    void *intelligence = src->header.m_pIntelligence;
    void *region = src->header.m_pCurrentRegion;
    f32 rotY = src->m_RotY;
    SpawnInfo d;
    s32 model;
    CGameObjectInstance *obj;

    d.pos = src->header.m_vPos;
    d.velocity = src->m_vVelocity;
    d.scale = src->m_vScale;
    model = kind;
    if (gPersistentDataLoaded == 0) {
        return NULL;
    }
    if (intelligence == NULL) {
        return NULL;
    }
    if ((model == -1 && (model = CScene__FindObjectType(&gScene, type)) == -1)
        || (obj = take_free_instance(world)) == NULL) {
        return NULL;
    }
    CGameObjectInstance__Construct(obj, model, type, gDefaultVisBits, intelligence, -1, region, rotY, d.pos, 0x80000, d.scale,
                  d.velocity, 0);
    CGameObjectInstance__InitAIType(obj);
    return obj;
}

void CListedInstance__Free(CListedInstance *arg0) {
    s32 *temp_v1;

    temp_v1 = arg0->m_pOwnerCount;
    arg0->m_Instance.m_AI.m_Health = 0;
    if (temp_v1 != NULL) {
        *temp_v1 -= 1;
    }
    CList__Remove(&gScene.m_InstancePool.m_ActiveList, arg0);
    CList__AddTail(&gScene_m_InstancePool_m_FreeList, arg0);
}

#include "protos/game.h"

/* CInstancePool__Alloc */
static __inline__ CGameObjectInstance *take_free_instance_228108(CInstancePool *world, s32 *owner) {
    CGameObjectInstance *node;

    if (gScene.m_bStreamAll != 0 || (u32)world->m_ActiveList.m_nCount < 3) {
        node = world->m_FreeList.m_pHead;
        if (node != NULL) {
            CList__Remove(&world->m_FreeList, node);
            CList__AddHead(&world->m_ActiveList, node);
            ((CListedInstance *)node)->m_pOwnerCount = owner;
            if (owner != NULL) {
                *owner += 1;
            }
        }
        return node;
    }
    return NULL;
}

CGameObjectInstance *CInstancePool__Spawn(void *world, s32 model, s32 type, void *intelligence, void *region,
                                   CVector3 pos, f32 rotY, CVector3 velocity, CVector3 scale, s32 *owner) {
    CGameObjectInstance *obj;

    if (gPersistentDataLoaded == 0) {
        return NULL;
    }
    if (intelligence == NULL) {
        return NULL;
    }
    if (model == -1) {
        model = CScene__FindObjectType(&gScene, type);
        if (model == -1) {
            return NULL;
        }
    }
    obj = take_free_instance_228108(world, owner);
    if (obj == NULL) {
        return NULL;
    }
    CGameObjectInstance__Construct(obj, model, type, gDefaultVisBits, intelligence, -1, region, rotY, pos, 0x80000, scale,
                  velocity, 0);
    CGameObjectInstance__InitAIType(obj);
    return obj;
}

struct SceneEffectPool;
/* Turok 2 effect record: 0x298 bytes (Rage Wars 0x2F8), most fields 0x2C-0x60 earlier. */
typedef struct Effect_func_80262E88_de Effect_func_80262E88_de;
struct Effect_func_80262E88_de {
    char pad0[0xBC];
    f32 min[3];         /* 0xBC (0xE8 in Rage Wars) */
    f32 max[3];         /* 0xC8 */
    s32 flags;          /* 0xD4 (0x100) */
    char padD8[0x144 - 0xD8];
    s32 busy;           /* 0x144 (0x174) */
    char pad148[0x28C - 0x148];
    struct Effect_func_80262E88_de *next; /* 0x28C (0x2EC) */
    s32 *ref;           /* 0x290 (0x2F0) */
    char pad294[4];
};
struct SceneEffectPool {
    Effect_func_80262E88_de effects[0x20];
    CList freeEffects;   /* 0x5300 */
    CList activeEffects; /* 0x5314 */
};

static __inline__ s32 overlaps(Effect_func_80262E88_de *effect, f32 *max) {
    if (max[0] > effect->min[0] && max[-3] < effect->max[0] && max[2] > effect->min[2] &&
        max[-1] < effect->max[2] && max[1] > effect->min[1] && max[-2] < effect->max[1]) {
        return 1;
    }
    return 0;
}

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/world.h"
#include "globals/camera.h"
/* The port (round 9 x1): the pool by member (CInstancePool, CListedInstance: the N64 view's
 * 0xBC/0xC8 are m_vMin/m_vMax, 0xD4 m_dwFlags, 0x144 m_AI.m_Health, 0x28C m_pNext, 0x290
 * m_pOwnerCount, 0x5314 the active list). Through the N64 view the host never found the pool's
 * active instances: the generators' spawns (the totem defense's waves, enemies thrown in by a
 * generator anywhere) were never filed into the frame's lists, so their AI never ran (they
 * stood in mode 2 at the spawn point) and a dead one never gave its generator slot back. */
void CInstancePool__UpdateVisibility(struct SceneEffectPool *scene_, sptr arg1) {
    CInstancePool *scene = (CInstancePool *) scene_;
    CListedInstance *effect;
    CListedInstance *next;
    CGameObjectInstance *o;
    f32 *max = &gCameraVisibleBox.m_vMax.x;

    for (effect = scene->m_ActiveList.m_pHead; effect != NULL; effect = next) {
        next = effect->m_pNext;
        o = &effect->m_Instance;
        if (max[0] > o->m_vMin.x && max[-3] < o->m_vMax.x && max[2] > o->m_vMin.z && max[-1] < o->m_vMax.z &&
            max[1] > o->m_vMin.y && max[-2] < o->m_vMax.y) {
            CScene__FileInstance((struct CScene *) arg1, o);
            if (effect->m_pOwnerCount != NULL && o->m_AI.m_Health == 0) {
                *effect->m_pOwnerCount -= 1;
                effect->m_pOwnerCount = NULL;
            }
        } else {
            o->m_AI.m_Health = 0;
            o->m_dwFlags &= ~0x200;
            if (effect->m_pOwnerCount != NULL) {
                *effect->m_pOwnerCount -= 1;
            }
            CList__Remove(&scene->m_ActiveList, effect);
            CList__AddTail(&scene->m_FreeList, effect);
        }
    }
}
#else
void CInstancePool__UpdateVisibility(struct SceneEffectPool *scene, sptr arg1) {
    Effect_func_80262E88_de *effect;
    Effect_func_80262E88_de *next;
    f32 *max;
    effect = scene->activeEffects.m_pHead;
    if (effect == 0) {
        return;
    }
    do {
        max = &gCameraVisibleBox.m_vMax.x;
    next_effect:
        next = effect->next;
        if (overlaps(effect, max)) {
            CScene__FileInstance(arg1, effect);
            if (effect->ref != 0 && effect->busy == 0) {
                *effect->ref -= 1;
                effect->ref = 0;
            }
        } else {
            effect->busy = 0;
            effect->flags &= ~0x200;
            if (effect->ref != 0) {
                *effect->ref -= 1;
            }
            CList__Remove(&scene->activeEffects, effect);
            CList__AddTail(&scene->freeEffects, effect);
        }
        effect = next;
    } while (0);
    if (effect != 0) {
        goto next_effect;
    }
}
#endif

/* Takes a free object (when fewer than three are active, or always while
 * gScene_m_bStreamAll is set), makes it active and counts it in *arg1. */
CListedInstance *CInstancePool__Alloc(CInstancePool *arg0, s32 *arg1) {
    CListedInstance *node;

    if (gScene.m_bStreamAll != 0 || (u32) arg0->m_ActiveList.m_nCount < 3) {
        node = arg0->m_FreeList.m_pHead;
        if (node != 0) {
            CList__Remove(&arg0->m_FreeList, node);
            CList__AddHead(&arg0->m_ActiveList, node);
            node->m_pOwnerCount = arg1;
            if (arg1 != NULL) {
                *arg1 += 1;
            }
        }
        return node;
    }
    return 0;
}

#include "structs/camera.h"

/* Retires an effect: drops the owner count and moves it back to the free list. */
void CInstancePool__Free(CInstancePool *world, CListedInstance *arg1) {
    s32 *temp_v1;

    temp_v1 = arg1->m_pOwnerCount;
    arg1->m_Instance.m_AI.m_Health = 0;
    if (temp_v1 != NULL) {
        *temp_v1 -= 1;
    }
    CList__Remove(&world->m_ActiveList, arg1);
    CList__AddTail(&world->m_FreeList, arg1);
}
